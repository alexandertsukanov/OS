

#include "stdafx.h"
#include "Strsafe.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

/*
1.	Скласти макроси для спрощення використання функції CreateProcess
для случав негайного й відкладеного запуску програми.
*/
#define SIMPLE_CREATE_PROCESS(app, pi, prior, interval) \
{ \
	if (interval > 0) Sleep(interval); \
	STARTUPINFO si; \
	memset(&si, 0, sizeof(STARTUPINFO)); \
	si.cb = sizeof(STARTUPINFO); \
	TCHAR CurrentPath[MAX_PATH]; \
	GetCurrentDirectory(MAX_PATH, CurrentPath); \
	BOOL b = CreateProcess(0, app, 0, 0, false, prior, 0, CurrentPath, &si, &pi); \
	if (!b) { \
		LPVOID lpMsgBuf; \
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL); \
		MessageBox(NULL, (const TCHAR *)lpMsgBuf, _T("GetLastError"), MB_OK | MB_ICONINFORMATION); \
		LocalFree(lpMsgBuf); \
	} \
}
#define CLOSE_PROCESS(pi) \
{ \
	WaitForSingleObject(pi.hProcess, INFINITE); \
	CloseHandle(pi.hThread); \
	CloseHandle(pi.hProcess); \
}



// Printing the error if any

void printError(TCHAR* msg)
{

	DWORD eNum;

	TCHAR sysMsg[256];

	TCHAR* p;

	eNum = GetLastError();



	// FormatMessageW - unicode, FormatMessageA - ANSI

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,

		NULL, eNum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language

		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null

	p = sysMsg;

	while ((*p > 31) || (*p == 9))

		++p;

	do { *p-- = 0; }

	while ((p >= sysMsg) && ((*p == '.') || (*p < 33)));

	// Display the message...

	printf("\n WARNING: %S failed with error %d (%s)\n", msg, eNum, sysMsg);

}


BOOL ListProcessModules(int dwPID)

{

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

	MODULEENTRY32 me32;



	// Take a snapshot of all modules in the specified process.

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);



	if (hModuleSnap == INVALID_HANDLE_VALUE)

	{

		printError(_T("CreateToolhelp32Snapshot()"));

		return (FALSE);

	}



	// Set the size of the structure before using it.

	me32.dwSize = sizeof(MODULEENTRY32);



	// Retrieve information about the first module, and exit if unsuccessful




	if (!Module32First(hModuleSnap, &me32))

	{

		printError(_T("Module32First()")); // Show cause of failure

		CloseHandle(hModuleSnap); // Must clean up the snapshot object

		return (FALSE);

	}



	// Now walk the module list of the process, and display information about each module

	do

	{

		printf("\n\n MODULE NAME = %S", me32.szModule);

		printf("\n executable = %S", me32.szExePath);

		printf("\n process ID = %u", me32.th32ProcessID);

		printf("\n ref count (global) = 0x%04X", me32.GlblcntUsage);

		printf("\n ref count (process) = 0x%04X", me32.ProccntUsage);

		printf("\n base address = 0x%p", me32.modBaseAddr);

		printf("\n base size = %d\n", me32.modBaseSize);

	} while (Module32Next(hModuleSnap, &me32));



	// Do not forget to clean up the snapshot object.

	CloseHandle(hModuleSnap);

	return (TRUE);

}


int main()
{
	/*
	Скласти 3 програми. Перша програма має запустити
	текстовий редактор і створити текстові файли в заданій
	папці. Друга програма для всіх створених першою програмою
	файлів визначає їх розмір, кількість рядків і довжину кожного
	рядка. Третя програма запускає по черзі спочатку першу, а
	потім другу програму. Для другої програми має бути встановлений
	нижчий пріоритет ніж у інших програм.
	*/
	// Get project dir path
	TCHAR CurrentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, CurrentPath);
	TCHAR filePath[MAX_PATH];
	StringCchCopy(filePath, MAX_PATH, CurrentPath);
	StringCchCat(filePath, MAX_PATH, _T("\\create_text_files.exe"));
	PROCESS_INFORMATION pi;
	// Run the first programm
	SIMPLE_CREATE_PROCESS(filePath, pi, NORMAL_PRIORITY_CLASS, 0);
	CLOSE_PROCESS(pi);
	StringCchCopy(filePath, MAX_PATH, CurrentPath);
	StringCchCat(filePath, MAX_PATH, _T("\\get_files_data.exe"));
	// Run the second programm
	SIMPLE_CREATE_PROCESS(filePath, pi, BELOW_NORMAL_PRIORITY_CLASS, 0);
	CLOSE_PROCESS(pi);
	std::system("pause");

	/*
	3.6 За допомогою цих функцій для кожного процесу визначити:
	його ім’я;
	DLL, які використовуються процесом;
	Кількість потоків для процесу
	*/

	HANDLE hHelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hHelp == INVALID_HANDLE_VALUE)
		return 1;
	// Выделение памяти под структуру с информацией
	PROCESSENTRY32 ProcessEntry32 = { sizeof(ProcessEntry32) };
	//BOOL b;
	// Определение первого процесса
	BOOL b = Process32First(hHelp, &ProcessEntry32);
	while (b) {
		printf("\n******************************************\n");
		_tprintf(_T("szExeFile = %s, "), ProcessEntry32.szExeFile);
		_tprintf(_T("cntThreads = %d, "), ProcessEntry32.cntThreads);
		_tprintf(_T("cntUsage = %d\n"), ProcessEntry32.cntUsage);
		// List the modules associated with this process
		ListProcessModules(ProcessEntry32.th32ProcessID);
		printf("\n******************************************\n");
		//Для всех остальных процессов
		b = Process32Next(hHelp, &ProcessEntry32);
		if (!b) 	break;

	}

	CloseHandle(hHelp);

	return 0;
}