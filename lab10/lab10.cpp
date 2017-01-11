

#include "stdafx.h"
#include "Strsafe.h"
#include <windows.h>
#include <stdio.h>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

#ifdef UNICODE
typedef std::wstring String;
#define _tcsifstream  std::wifstream
#define _tcsofstream  std::wofstream
#define _tcscout  std::wcout
#else
typedef std::string String;
#define _tcsifstream   std::ifstream
#define _tcsofstream   std::ofstream
#define _tcscout  std::cout
#endif

// The directory for the text files
TCHAR dirPath[] = _T("C:\\test");
CRITICAL_SECTION cs;
std::queue<TCHAR*> fileList;

DWORD WINAPI ThreadSort(VOID *t);
int getFilesFromDir(unsigned __int64 i64MinCreateTime, BOOL isCheck);

int main()
{

	/*
	6. Скласти програму, в якої необхідно:
		запустити довільний текстовий редактор;
		створити в цьому редакторі 2 файли;
		Завершити роботу редактора після створення файлів;	*/

	// Определение текущего времени с учетом часового пояса
	SYSTEMTIME st;
	GetLocalTime(&st);
	FILETIME ft;
	// Преобразование времени в формат FileTime
	SystemTimeToFileTime(&st, &ft);
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	unsigned __int64 i64MinCreateTime = (((unsigned __int64)(ft.dwHighDateTime)) << 32) |
		ft.dwLowDateTime;

	CreateDirectory(dirPath, NULL);
	SetCurrentDirectory(dirPath);
	// Get system dir path
	TCHAR windir[MAX_PATH];
	GetWindowsDirectory(windir, MAX_PATH);
	StringCchCat(windir, MAX_PATH, _T("\\System32\\notepad.exe"));
	BOOL b = CreateProcess(0, windir,
		0, 0, false, 0, 0, dirPath, &si, &pi);

	if (!b)
	{
		printf("Error\n");
		return 1;
	}
	// Ждем завершения работы редактора
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	/*
	створити 2 потоки;
	у кожному потоці впорядкувати файли, створені в редакторі;
	*/

	getFilesFromDir(i64MinCreateTime, false);

	InitializeCriticalSection(&cs);

	HANDLE hThread[2];
	DWORD dwIdent;
	hThread[0] = CreateThread(0, 0, ThreadSort, 0, 0, &dwIdent);
	hThread[1] = CreateThread(0, 0, ThreadSort, 0, 0, &dwIdent);

	if (!hThread[0] || !hThread[1])
	{
		printf("CreateThread Error\n");
		return 0;
	}

	WaitForMultipleObjects(2, hThread, true, INFINITE);

	// після упорядкування обох файлів знищити потоки;
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	DeleteCriticalSection(&cs);

	// перевірити створені файли.
	printf("\nCheck files\n");

	getFilesFromDir(i64MinCreateTime, true);
	return 0;
}

int getFilesFromDir(unsigned __int64 i64MinCreateTime, BOOL isCheck)
{
	// Поиск файлов
	BOOL b = false;
	WIN32_FIND_DATA FindFileData;
	HANDLE h = FindFirstFile(_T("*.txt"), &FindFileData);
	if (h == INVALID_HANDLE_VALUE)
	{
		printf("File not found!\n");
		return 1;
	}
	TCHAR* buffer;
	do
	{
		FILETIME ftCreateFileTime = FindFileData.ftCreationTime;
		// Время создания файла
		FileTimeToLocalFileTime(&ftCreateFileTime, &ftCreateFileTime);
		unsigned __int64 i64CurFileCreateTime =
			((unsigned __int64)(ftCreateFileTime.dwHighDateTime) << 32) |
			ftCreateFileTime.dwLowDateTime;
		// Если файл создан после вызова редактора
		if (i64MinCreateTime < i64CurFileCreateTime) {

			buffer = new TCHAR[MAX_PATH];
			StringCchCopy(buffer, MAX_PATH, FindFileData.cFileName);

			if (isCheck) {
				_tcscout  << "\nFile - " <<  buffer << std::endl;
				_tcsifstream fin(buffer); 
				while (!fin.eof()) {
					String s;
					std::getline(fin, s);
					_tcscout << s << std::endl;
				}
				fin.close();
			}
			else {
				fileList.push(buffer);
			}
		}
		b = FindNextFile(h, &FindFileData);
		// Если файлов больше нет – выход из цикла
	} while (b);


	return 0;
}

DWORD WINAPI ThreadSort(VOID *t)
{
	TCHAR* buffer;
	while (true) {
		EnterCriticalSection(&cs);

		if (fileList.size() < 1)
		{
			LeaveCriticalSection(&cs);
			return 0;
		}

		buffer = new TCHAR[MAX_PATH];
		StringCchCopy(buffer, MAX_PATH, fileList.front());
		fileList.pop();
		LeaveCriticalSection(&cs);

		_tcsifstream fin(buffer);

		// на початку сортування кожного файлу видати повідомлення про початок сортування;
		_tprintf(_T("Start sort file - %s\n"), buffer);

		std::vector<String> vect;

		while (!fin.eof()) {
			String s;
			std::getline(fin, s);
			vect.push_back(s);
		}
		fin.close();

		std::sort(vect.begin(), vect.end());

		_tcsofstream fout(buffer);
		
		unsigned int vectorSize = vect.size();

		for (int i = 0; i < vectorSize; i++) {
			fout << vect[i] << std::endl;
		}
		
		fout.close();

		//наприкінці сортування кожного файлу видати повідомлення про кінець сортування;
		_tprintf(_T("Finish sort file - %s\n"), buffer);
	}

	return 0;
}