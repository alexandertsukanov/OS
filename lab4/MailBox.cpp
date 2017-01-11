#include "stdafx.h"
#include "MailBox.h"
#ifdef UNICODE
typedef std::wstring String;
#define _tcsifstream  std::wifstream
#else
typedef std::string String;
#define _tcsifstream   std::ifstream
#endif

// Start "For calculate MD_5"
typedef struct {
	char hash[33];//32 chars for MD5, +1 for terminating zero 
	unsigned char digest[16];
} TMD5;

typedef struct {
	ULONG         i[2];
	ULONG         buf[4];
	unsigned char in[64];
	unsigned char digest[16];
} MD5_CTX;

typedef void(__stdcall *PMD5Init)  (MD5_CTX *context);
typedef void(__stdcall *PMD5Update)(MD5_CTX *context, const unsigned char *input, unsigned int inlen);
typedef void(__stdcall *PMD5Final) (MD5_CTX *context);

PMD5Init   MD5Init = NULL;
PMD5Update MD5Update = NULL;
PMD5Final  MD5Final = NULL;

#define DBG_CHECK(OperationDesc, Value, ReturnOnFail) \
    std::cout << OperationDesc;\
    if (!HRESULT(Value))\
    {\
        std::cout << "Failed! Code: " << GetLastError() << std::endl;\
        system( "pause" );\
        return (ReturnOnFail);\
    }\
    else\
    {\
        std::cout << "OK" << std::endl;\
    }\

LPVOID MapFile_ReadOnly(LPCTSTR lpFileName, DWORD &dwSize)
{
	dwSize = 0;
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) return NULL;
	if (dwSize == 0)
		dwSize = GetFileSize(hFile, NULL);
	if (dwSize == 0)
	{
		CloseHandle(hFile);
		return NULL;
	}
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwSize, NULL);
	CloseHandle(hFile);
	if (hFileMap == NULL)
		return NULL;
	LPVOID lpMap = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, dwSize);
	CloseHandle(hFileMap);
	return lpMap;
}

bool InitMD5()
{
	HMODULE hLib = LoadLibrary(TEXT("Cryptdll.dll"));
	DBG_CHECK("Loading Cryptdll.dll: ", hLib, false);

	MD5Init = PMD5Init(GetProcAddress(hLib, "MD5Init"));
	DBG_CHECK("GetProc MD5Init: ", MD5Init, false);
	MD5Update = PMD5Update(GetProcAddress(hLib, "MD5Update"));
	DBG_CHECK("GetProc MD5Update: ", MD5Update, false);
	MD5Final = PMD5Final(GetProcAddress(hLib, "MD5Final"));
	DBG_CHECK("GetProc MD5Final: ", MD5Final, false);
	return true;
}

TMD5 GetMD5(PUCHAR Buffer, DWORD dwSize)
{
	const char HEX_CHAR_MAP[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	MD5_CTX context;
	TMD5 Result;
	MD5Init(&context);

	for (; dwSize >= 64; Buffer += 64, dwSize -= 64)
		MD5Update(&context, Buffer, 64);
	MD5Update(&context, Buffer, dwSize);

	MD5Final(&context);

	for (int i = 0; i<16; i++)
	{
		Result.hash[i * 2] = HEX_CHAR_MAP[context.digest[i] >> 4];
		Result.hash[i * 2 + 1] = HEX_CHAR_MAP[context.digest[i] & 15];
	}
	Result.hash[32] = NULL;
	memcpy(Result.digest, context.digest, 16);
	return Result;
}

// End "For calculate MD_5"

MAILBOX::MAILBOX(LPCTSTR fN, DWORD NewMaxSize)
{
	charSize = sizeof(_T('a'));
	// The directory for "Mail Box"
	CreateDirectory(MAILBOX::mailBoxDir, NULL);
	SetCurrentDirectory(MAILBOX::mailBoxDir);
	_tcscpy_s(fName, fN);
	h = CreateFile(fN, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (h == INVALID_HANDLE_VALUE) throw ERRORS();
	DWORD CommonSizeOld = 0, MessageCountsOld = 0, MaxSizeOld;
	DWORD Count = 0;
	ReadFile(h, &CommonSizeOld, sizeof(DWORD), &Count, 0);
	if (Count == 0) {// Файл новый
		CommonSize = 0; 
		MessageCounts = 0;
		 MaxSize = NewMaxSize;
	}
	else // Файл уже есть
	{
		ReadFile(h, &MessageCountsOld, sizeof(DWORD), &Count, 0);
		if (Count == 0) ERRORS();
		ReadFile(h, &MaxSizeOld, sizeof(DWORD), &Count, 0);
		if (Count == 0) ERRORS();
		CommonSize = CommonSizeOld;
		MessageCounts = MessageCountsOld;
		MaxSize = MaxSizeOld;
	}

	LONG lPart = 0, hPart = 0;// Указатель на начало файла
	SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	// Запись общего размера, количествв сообщений, максимального размера
	WriteFile(h, &CommonSize, sizeof(DWORD), &Count, 0);
	WriteFile(h, &MessageCounts, sizeof(DWORD), &Count, 0);
	WriteFile(h, &MaxSize, sizeof(DWORD), &Count, 0);
	CloseHandle(h);
}

// Добавить сообщение 
MAILBOX& MAILBOX::operator+=(LPCTSTR  Msg)
{
	int i;
	for (i = 0; Msg[i]; i++);
	if (MaxSize < CommonSize + i)
	{
		MessageBox(
			NULL,
			_T("Exceeded the maximum MailBox size"),
			_T("Error"),
			MB_OK
		);
		return *this;
	}
	CommonSize += i;
	MessageCounts += 1;
	h = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (h == INVALID_HANDLE_VALUE) {
		{
			// Вывод ошибки
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
			MessageBox(NULL, (const TCHAR *)lpMsgBuf, _T("GetLastError"), MB_OK | MB_ICONINFORMATION);
			LocalFree(lpMsgBuf);
		}
		throw ERRORS();
	}
	DWORD Count;
	WriteFile(h, &CommonSize, sizeof(DWORD), &Count, 0);
	WriteFile(h, &MessageCounts, sizeof(DWORD), &Count, 0);
	DWORD s = SetFilePointer(h, 0, 0, FILE_END);
	WriteFile(h, &i, sizeof(DWORD), &Count, 0);
	WriteFile(h, Msg, i * charSize, &Count, 0);
	CloseHandle(h);
	return *this;
}

// Удаление заданного сообщения
MAILBOX& MAILBOX::operator-=(LPCTSTR  Msg) {
	int i, len;
	for (i = 0; Msg[i]; i++); 
	len = i;
	h = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (h == INVALID_HANDLE_VALUE) throw ERRORS();
	LONG lPart = sizeof(DWORD) * 3, hPart = 0;
	DWORD CurrentLen, Count;
	BOOL b, res = false;
	TCHAR *buff;
	lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	for (i = 1; true; i++)
	{
		lPart += sizeof(DWORD);
		b = ReadFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
		if (Count != sizeof(DWORD)) {
			break;
		}
		lPart += CurrentLen * charSize;
		if (CurrentLen != len)
		{
			lPart += CurrentLen * charSize;
			hPart = 0;
			lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
		}
		else
		{
			buff = new TCHAR[CurrentLen];
			ReadFile(h, buff, CurrentLen * charSize, &Count, 0);

			for (int x = 0; x < len; x++)
			{
				if (Msg[x] == buff[x])
				{
					res = true;
				}
				else
				{
					res = false;
					break;
				}
			}

			delete[]buff;
			if (res) break;
		}
	}
	CloseHandle(h);
	if (res)
	{
		return MAILBOX::DeleteMessage(i);
	}
	else
	{
		return *this;
	}
}
// Удаление сообщения с заданным номером
MAILBOX& MAILBOX::operator-=(int number)
{
	return MAILBOX::DeleteMessage(number);
}
// Чтение сообщения с заданным номером		
bool MAILBOX::RdMsg(TCHAR *res, int number, DWORD &len, bool deleteMessage) {
	// Если номер сообщения неверный
	if (number < 0 || number > MessageCounts) return false;
	h = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (h == INVALID_HANDLE_VALUE) {
		throw ERRORS();
	}
	LONG lPart = sizeof(DWORD) * 3, hPart = 0;
	int i;
	// Пропуск сообщений 
	DWORD CurrentLen;
	DWORD Count;
	BOOL b;
	lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	for (i = 1; i < number; i++) {
		lPart += sizeof(DWORD);
		b = ReadFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
		if (!b || Count != sizeof(DWORD)) {
			CloseHandle(h);
			throw ERRORS();
		}
		lPart += CurrentLen * charSize;
		hPart = 0;
		lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	}

	LONG lPartRead = lPart, hPartRead = hPart;
	b = ReadFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
	if (!b)
	{ 
		CloseHandle(h); 
		return false; 
	}

	if (len != CurrentLen)
	{
		CloseHandle(h);
		len = CurrentLen;
		return true;
	}

	b = ReadFile(h, res, CurrentLen * charSize, &Count, 0);
	CloseHandle(h);

	if (deleteMessage)
	{
		MAILBOX::DeleteMessage(number);
	}
	return true;
}

MAILBOX MAILBOX::DeleteMessage(int number)
{
	// Если номер сообщения некорректный		
	if (number < 0 || number > MessageCounts)
		return *this;
	h = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (h == INVALID_HANDLE_VALUE) throw ERRORS();
	LONG lPart = sizeof(DWORD) * 3, hPart = 0;
	int i;
	// Пропуск предшествующих сообщений 
	DWORD CurrentLen;
	DWORD Count;
	BOOL b;
	lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	for (i = 1; i < number; i++) {
		lPart += sizeof(DWORD);
		b = ReadFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
		if (!b || Count != sizeof(DWORD)) {
			CloseHandle(h);
			throw ERRORS();
		}
		lPart += CurrentLen * charSize;
		hPart = 0;
		lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	}
	// Удаление заданного сообщения
	LONG lPartNext, hPartNext = 0;
	b = ReadFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
	lPartNext = lPart + sizeof(DWORD) + CurrentLen * charSize;
	SetFilePointer(h, 0, 0, FILE_BEGIN);
	CommonSize -= CurrentLen;
	MessageCounts--;
	WriteFile(h, &CommonSize, sizeof(DWORD), &Count, 0);
	WriteFile(h, &MessageCounts, sizeof(DWORD), &Count, 0);
	// Перемещение остальных сообщений
	TCHAR *buff;
	while (1) {
		lPartNext = SetFilePointer(h, lPartNext, &hPartNext, FILE_BEGIN);
		ReadFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
		lPartNext += sizeof(DWORD);
		if (!Count)break;
		buff = new TCHAR[CurrentLen];
		ReadFile(h, buff, CurrentLen * charSize, &Count, 0);
		lPartNext += CurrentLen * charSize;

		lPart = SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
		WriteFile(h, &CurrentLen, sizeof(DWORD), &Count, 0);
		lPart += sizeof(DWORD);
		WriteFile(h, buff, CurrentLen * charSize, &Count, 0);
		lPart += CurrentLen * charSize;
		delete[]buff;
	}
	SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	SetEndOfFile(h);
	CloseHandle(h);
	return *this;
}

int MAILBOX::cleanMailBoxes()
{
	h = CreateFile(fName, GENERIC_READ | GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (h == INVALID_HANDLE_VALUE) throw ERRORS();
	DWORD Count;
	LONG lPart = 0, hPart = 0;// Указатель на начало файла
	SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	CommonSize = MessageCounts = 0;
	// Запись общего размера, количествв сообщений, максимального размера
	WriteFile(h, &CommonSize, sizeof(DWORD), &Count, 0);
	WriteFile(h, &MessageCounts, sizeof(DWORD), &Count, 0);
	lPart = sizeof(DWORD) * 3;
	SetFilePointer(h, lPart, &hPart, FILE_BEGIN);
	SetEndOfFile(h);
	CloseHandle(h);
}

int MAILBOX::GetNumberOfMail()
{
	return MessageCounts;
}

int MAILBOX::GetMd5MailBox()
{
	TMD5 MD5;

	if (!InitMD5())
		throw ERRORS();

	//Geting hash for file
	DWORD dwSize;
	LPVOID pFile = MapFile_ReadOnly(fName, dwSize);
	MD5 = GetMD5(PUCHAR(pFile), dwSize);
	UnmapViewOfFile(pFile);
	std::cout << "MD5 hash is - " << MD5.hash << std::endl;
	return 0;
}

int MAILBOX::GetNumberOfMailBoxes()
{
	int counter = 0;
	TCHAR szDir[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	StringCchCopy(szDir, MAX_PATH, MAILBOX::mailBoxDir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		throw ERRORS();
		return 0;
	}
	
	do
	{
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			counter++;
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);
	
	
	FindClose(hFind);
	return counter;
}