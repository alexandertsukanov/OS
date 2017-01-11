// fifth_lab.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#ifdef UNICODE
typedef std::wstring String;
#define _tcscout  std::wcout
#else
typedef std::string String;
#define _tcscout  std::cout
#endif

/*
4. Визначить вільний дисковий простір для усіх пристроїв
(функція GetDiskFreeSpaceEx);
*/
int getFreeSpace(TCHAR * disk)
{
	DWORDLONG FreeBytesAvailable = 0;
	DWORDLONG TotalNumberOfBytes;
	DWORDLONG TotalNumberOfFreeBytes;

	BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceEx(
		disk,					  // directory name
		(PULARGE_INTEGER)&FreeBytesAvailable,     // bytes available to caller
		(PULARGE_INTEGER)&TotalNumberOfBytes,     // bytes on disk
		(PULARGE_INTEGER)&TotalNumberOfFreeBytes  // free bytes on disk
	);

	if (GetDiskFreeSpaceFlag != 0)
	{
		std::cout << "	Free Bytes Available = " << (unsigned long)FreeBytesAvailable
			<< "( " << double(unsigned long(FreeBytesAvailable)) / 1024 / 1000
			<< " Mb )" << std::endl;
		std::cout << "	Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes
			<< "( " << double(unsigned long(TotalNumberOfBytes)) / 1024 / 1000
			<< " Mb )" << std::endl;
		std::cout << "	Total Number Of Free Bytes = " << (unsigned long)TotalNumberOfFreeBytes
			<< "( " << double(unsigned long(TotalNumberOfFreeBytes)) / 1024 / 1000
			<< " Mb )" << std::endl;
	}
	else
	{
		std::cout << "	Not Present (GetDiskFreeSpace)" << std::endl;
	}

	return FreeBytesAvailable;
}

/*
5. За допомогою функції GetVolumeInformation визначить 
серійний номер диску, його ім’я та ім’я файлової системи;
*/
unsigned long getDiskInfo(TCHAR * disk)
{
	TCHAR VolumeNameBuffer[MAX_PATH];
	TCHAR FileSystemNameBuffer[MAX_PATH];
	unsigned long VolumeSerialNumber;

	BOOL GetVolumeInformationFlag = GetVolumeInformation(
		disk,
		VolumeNameBuffer,
		MAX_PATH,
		&VolumeSerialNumber,
		NULL, //&MaximumComponentLength,
		NULL, //&FileSystemFlags,
		FileSystemNameBuffer,
		MAX_PATH
	);

	if (GetVolumeInformationFlag != 0)
	{
		_tcscout << _T("	Volume Name is ") << VolumeNameBuffer << std::endl;
		_tcscout << _T("	Volume Serial Number is ") << VolumeSerialNumber << std::endl;
		_tcscout << _T("	File System is ") << FileSystemNameBuffer << std::endl;
		return VolumeSerialNumber;
	}
	else std::cout << "	Not Present (GetVolumeInformation)" << std::endl;

	return 0;
}

/*
6. Реалізувати функцію копіювання файлу, яка спочатку 
перевіряє наявність необхідного диску і можливість 
виконання операції запису на нього, потім аналізує наявність 
необхідного вільного місця, і тільки потім виконує запис 
даних на диск. При читанні файлу та його записуванні 
виконувати шифрування (розшифрування) файлу з використанням 
операції складення по модулю 2 з серійним номером відповідного 
диску. Якщо файли на одному диску - шифрування – розшифрування 
не виконувати. Написати функцію для читання записів цього файлу.
*/
int safeCopyFile(TCHAR * sourceFile, TCHAR * targetPath, bool decode = true, bool encode = true)
{
	if (GetFileAttributes(sourceFile) == DWORD(-1))
	{
		_tcscout << _T("File ") << sourceFile << _T(" not found") << std::endl;
		return 0;
	}

	HANDLE hSource = CreateFile(
		sourceFile,
		GENERIC_READ,
		0,
		0,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hSource == INVALID_HANDLE_VALUE)
	{
		_tcscout << _T("File ") << sourceFile << _T(" can\'t be open") << std::endl;
		return 0;
	}

	DWORDLONG lpFileSize = 0;

	GetFileSizeEx(hSource, (PLARGE_INTEGER)&lpFileSize);

	// Is target exist and is it have free space ?
	if (getFreeSpace(targetPath) < lpFileSize)
	{
		_tcscout << _T("File ") << sourceFile << _T(" can\'t be copy to ");
		_tcscout << targetPath << std::endl;
		return 0;
	}

	// Get files disks
	TCHAR sourceDisk[MAX_PATH], targetDisk[MAX_PATH];
	DWORD  cchBufferLength = MAX_PATH;
	GetVolumePathName(
		sourceFile,
		sourceDisk,
		cchBufferLength
	);

	GetVolumePathName(
		targetPath,
		targetDisk,
		cchBufferLength
	);

	// get source file name (if "sourceFile" - full path to file)
	TCHAR *fileName;
	TCHAR fullSourcePath[MAX_PATH], fullTargetFile[MAX_PATH];
	GetFullPathName(sourceFile, MAX_PATH, fullSourcePath, &fileName);
	StringCchCopy(fullTargetFile, MAX_PATH, targetPath);
	StringCchCat(fullTargetFile, MAX_PATH, fileName);

	// Check create file
	HANDLE hTarget = CreateFile(fullTargetFile, GENERIC_WRITE, 0, 0,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hTarget == INVALID_HANDLE_VALUE)
	{
		_tcscout << _T("Error - ") << GetLastError() << std::endl;
		return 0;
	}

	// if the disk is the same and don't need decoding data
	// or don't need decoding/encoding - simple copying file
	if ((targetDisk[0] == sourceDisk[0] && encode && decode) || (!decode && !encode))
	{
		CloseHandle(hSource);
		CloseHandle(hTarget);
		bool copyRes = CopyFile(
			fullSourcePath,
			fullTargetFile,
			false
		);

		if (!copyRes)
		{
			_tcscout << _T("Error - ") << GetLastError() << std::endl;
			return 0;
		}
	}
	//if the disk is different
	else
	{
		unsigned long sourceSerialNumber = getDiskInfo(sourceDisk);
		unsigned long targetSerialNumber = getDiskInfo(targetDisk);
		TCHAR buff[sizeof(TCHAR)];
		DWORD Count;
		BOOL bs, bt;
		SetFilePointer(hSource, 0, 0, FILE_BEGIN);
		SetFilePointer(hTarget, 0, 0, FILE_BEGIN);
		unsigned long tmpSSN = sourceSerialNumber, tmpTSN = targetSerialNumber;
		int sKey, tKey;
		do {
			// get keys for encodind/decoding from the disk serial number 
			if (tmpSSN < 1)
			{
				tmpSSN = sourceSerialNumber;
				tmpTSN = targetSerialNumber;
			}
			sKey = tmpSSN % 10;
			tKey = tmpTSN % 10;
			// read the source file
			bs = ReadFile(hSource, &buff, sizeof(TCHAR), &Count, 0);
			// If need decoding data
			if (decode)
			{
				// decoding
				*buff ^= sKey;
			}
			// If need encoding data
			if (encode)
			{
				// encoding
				*buff ^= tKey;
			}
			// write the target file
			bt = WriteFile(hTarget, &buff, sizeof(TCHAR), &Count, 0);
			lpFileSize -= Count;
			tmpSSN /= 10;
			tmpTSN /= 10;
		} while (bs && bt && lpFileSize > 0);

		SetEndOfFile(hTarget);
		CloseHandle(hSource);
		CloseHandle(hTarget);
	}
	return 0;
}

int main()
{
	/*
	1. Визначить кількість пристроїв, які встановлені
	(функція  GetLogicalDrives – кількість бітів, що 
	дорівнюють 1 в полі результату);
	*/
	int n;
	char dd[4];
	DWORD dr = GetLogicalDrives();
	std::cout << "Use GetLogicalDrives : " << std::endl;
	for (int i = 0; i < 26; i++)
	{
		n = ((dr >> i) & 0x00000001);
		if (n == 1)
		{
			dd[0] = char(65 + i); dd[1] = ':'; dd[2] = '\\'; dd[3] = 0;
			std::cout << dd << std::endl;
		}
	}
	std::cout << std::endl;
	/*
	2. Визначить імена встановлених пристроїв
	(функція GetLogicalDriveStrings);
	
	*/

	TCHAR disks[256];
	TCHAR *disk;
	DWORD sizebuf = 256;
	int d;
	DWORDLONG free;
	GetLogicalDriveStrings(sizebuf, disks);
	disk = disks;

	std::cout << "Use GetLogicalDriveStrings : " << std::endl;
	while (*disk)
	{

		/*
		3. Для кожного встановленого пристрою визначить його
		тип (функція GetDriveType);
		*/
		_tprintf(_T("Disk %s\n"), disk);
		d = GetDriveType(disk);

		switch (d)
		{
			/*
			4. Визначить вільний дисковий простір для усіх пристроїв
			(функція GetDiskFreeSpaceEx);
			*/
			/*
			5. За допомогою функції GetVolumeInformation визначить 
			серійний номер диску, його ім’я та ім’я файлової системи;
			*/
			case DRIVE_UNKNOWN: 
				std::cout << " UNKNOWN" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
				break;
			case  DRIVE_NO_ROOT_DIR: 
				std::cout << " DRIVE NO ROOT DIR" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
				break;
			case  DRIVE_REMOVABLE:
				std::cout << " REMOVABLE" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
				break;
			case  DRIVE_FIXED:
				std::cout << " FIXED" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
				break;
			case  DRIVE_REMOTE:
				std::cout << " REMOTE" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
				break;
			case  DRIVE_CDROM:
				std::cout << " CDROM" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
				break;
			case  DRIVE_RAMDISK:
				std::cout << " RAMDISK" << std::endl;
				free = getFreeSpace(disk);
				getDiskInfo(disk);
		}

		disk = disk + _tcslen(disk) + 1;
	}

	/*
	6. Реалізувати функцію копіювання файлу, яка спочатку
	перевіряє наявність необхідного диску і можливість
	виконання операції запису на нього, потім аналізує наявність
	необхідного вільного місця, і тільки потім виконує запис
	даних на диск. При читанні файлу та його записуванні
	виконувати шифрування (розшифрування) файлу з використанням
	операції складення по модулю 2 з серійним номером відповідного
	диску. Якщо файли на одному диску - шифрування – розшифрування
	не виконувати. Написати функцію для читання записів цього файлу.
	*/
	// Using flags decode/encode , we can get a different result
	// firstly we  encode the don't encoded file which is in
	// the project folder
	safeCopyFile(_T("test_copy.txt"), _T("E:\\"), false);
	// copy the file to other disk with decoding/encoding
	safeCopyFile(_T("E:\\test_copy.txt"), _T("F:\\"));
	// copy the file to other disk with decoding without encoding
	// and get don't encoding file
	safeCopyFile(_T("F:\\test_copy.txt"), _T("E:\\"), true, false);

    return 0;
}
