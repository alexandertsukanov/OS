// sixth_lab.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

/*
1. Скласти програму для формування системної інформації про віртуальну
пам'ять і пояснити отримані результати;
*/
int GetMemoryInfo()
{
	// Retrieves information about the current system.
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	printf("dwPageSize = %d\n", SystemInfo.dwPageSize);
	printf("dwAllocationGranularity = %d\n", SystemInfo.dwAllocationGranularity);
	printf("lpMinimumApplicationAddress = %#x\n", SystemInfo.lpMinimumApplicationAddress);
	printf("lpMaximumApplicationAddress = %#x\n", SystemInfo.lpMaximumApplicationAddress);

	//Retrieves information about the system's current usage memory.
	MEMORYSTATUSEX MemoryStatus;
	MemoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&MemoryStatus);
	printf("ullAvailPhys = %I64x\n", MemoryStatus.ullAvailPhys);
	printf("ullAvailVirtual = %I64x\n", MemoryStatus.ullAvailVirtual);
	printf("ullAvailPageFile = %I64x\n", MemoryStatus.ullAvailPageFile);

	printf("ullullTotalPhys = %I64x\n", MemoryStatus.ullTotalPhys);
	printf("ullTotalVirtual = %I64x\n", MemoryStatus.ullTotalVirtual);
	printf("ullTotalPageFile = %I64x\n", MemoryStatus.ullTotalPageFile);

	MEMORY_BASIC_INFORMATION Buffer;
	//Retrieves a pseudo handle for the current process.
	HANDLE hProcess = GetCurrentProcess();
	DWORD dwAddress = (DWORD)SystemInfo.lpMinimumApplicationAddress;
	while (dwAddress < (DWORD)SystemInfo.lpMaximumApplicationAddress)
	{
		// Get information about a range of pages within the virtual address
		// space of  all processes
		DWORD dwSize = VirtualQueryEx(hProcess, (const void *)dwAddress,
			&Buffer, sizeof(MEMORY_BASIC_INFORMATION));

		printf("BaseAddress = %#x\n", dwAddress);
		printf("RegionSize = %u\n", Buffer.RegionSize);

		switch (Buffer.State)
		{
		case MEM_COMMIT:printf("MEM_COMMIT\n"); break;
		case  MEM_FREE: printf("MEM_FREE\n"); break;
		case  MEM_RESERVE:printf("MEM_RESERVE\n"); break;
		default:printf("UNKNOWN\n");
		}
		if (Buffer.State != MEM_FREE)
		{
			switch (Buffer.AllocationProtect)
			{
			case PAGE_READONLY: printf("PAGE_READONLY\n"); break;
			case PAGE_READWRITE: printf("PAGE_READWRITE\n"); break;
			case PAGE_EXECUTE: printf("PAGE_READWRITE\n"); break;
			case PAGE_EXECUTE_READ: printf("PAGE_EXECUTE_READ\n"); break;
			case PAGE_EXECUTE_READWRITE: printf("PAGE_EXECUTE_READWRITE\n"); break;
			case PAGE_EXECUTE_WRITECOPY: printf("PAGE_EXECUTE_WRITECOPY\n"); break;
			case PAGE_NOACCESS: printf("PAGE_NOACCESS\n"); break;
			case PAGE_WRITECOPY: printf("PAGE_WRITECOPY\n"); break;
			default: printf("UNKNOWN\n");

			}
		}
		printf("\n\n");
		dwAddress += Buffer.RegionSize;
	}

	return (int) SystemInfo.dwPageSize / 1024;
}

/*
3. Реалізувати алгоритм заміщення сторінок
*/
int found(int x, int *l, int max)
{
	for (int i = 0; i<max; i++)
		if (l[i] == x) { return(i); }
	return(-1);
}

int FIFOPageReplacementAlgorithm()
{
	std::cout << "\n\nFIFO Page Replacement Algorithm";
	std::cout << "\n\nEnter the maximum number of frames in the main memory:\t";
	int max;
	std::cin >> max;
	int *l = new int[max];
	for (int i = 0; i<max; i++)l[i] = -1;
	int a, x;
	int k = 0, c = 0, res;
	std::cout << "\n\nEnter the sequence of page requests(enter -1 to stop):\t";
	while (1)
	{
		std::cin >> x;
		if (x == -1) { std::cout << "\n\n"; break; }
		else {

			if (k<max)
			{
				if ((res = found(x, l, max)) != -1) {
					std::cout << "\n\npage " << x << " already exists in frame " << res << " in MM";
					std::cout << "\n\nNext page:\t";
				}
				else
				{
					std::cout << "\n\npage " << x << " has been allocated a frame " << k << " in MM.";
					l[k++] = x;
					std::cout << "\n\nNext page:\t";
				}
			}
			else
			{
				if ((res = found(x, l, max)) != -1) {
					std::cout << "\n\npage " << x << " already exists in frame " << res << " in MM";
					std::cout << "\n\nNext page:\t";
				}
				else {

					std::cout << "\n\npage fault has occured";
					std::cout << "\n\npage " << x << " has been allocated frame " << c << " in MM by replacing page " << l[c];
					l[c] = x;
					c = (c + 1) % max;
					std::cout << "\n\nNext page:\t";
				}
			}
		}
	}

	delete[] l;
	return 0;
}

/*
4. Реалізувати алгоритм LRU, який використовується для 4-х 
направленого кешу
*/

int found4WaySetAssocCahe(int x, int l[4][2], int strInd)
{
	for (int i = 0; i<4; i++)
		if (l[i][strInd] == x)
		{ 
			return(i);
		}
	return(-1);
}

int lru(int listLRU[2][4], int strInd, int res, bool getNext = false)
{
	int last, temp[4], cnt = 0;
	for (int i = 0; i < 4; i++) 
		temp[i] = listLRU[strInd][i];
	if (getNext)
	{
		res = listLRU[strInd][cnt];
	}
	for (int i = 0; i < 4; i++)
	{
		if (temp[i] != res)
		{
			listLRU[strInd][cnt] = temp[i];
			cnt++;
		}
		else
		{ 
			last = temp[i];
		}
	}
	cnt++;
	listLRU[strInd][3] = last;
	return last;
}

int main()
{
	/*
	1. Скласти програму для формування системної інформації про віртуальну
	пам'ять і пояснити отримані результати;
	*/
	int minMemorySize = GetMemoryInfo();

	/*
	2. Визначити мінімальний обсяг пам’яті, який можна виділити
	за допомогою функції VirtualAllocEx. Зробити висновки по
	використанню цієї функції.
	*/
	printf("The minimum amount of memory that can be identified using VirtualAllocEx is :%d KiB\n", minMemorySize);
	
	/*
	3. Реалізувати алгоритм заміщення сторінок
	*/
	FIFOPageReplacementAlgorithm();

	/*
	4. Реалізувати алгоритм LRU, який використовується для 4-х 
	направленого кешу
	*/

	int x, strInd, res, last;
	int cache4waySetAssoc[4][2] = {
		{ -1, -1 },
		{ -1, -1 },
		{ -1, -1 },
		{ -1, -1 },
	};
	int listLRU[2][4] = {
		{ 0, 1, 2, 3 },
		{ 0, 1, 2, 3 },
	};
	int j[2] = { 0, 0 };

	std::cout << "\n\nCache 4 set way replacement algorithm";
	std::cout << "\n\nEnter the sequence of page requests(enter -1 to stop):\t";

	while (true)
	{
		std::cin >> x;
		if (x == -1)
		{
			std::cout << "\n\n"; break;
		}
		else
		{
			strInd = x % 2;
			if (j[strInd]<4)
			{
				if ((res = found4WaySetAssocCahe(x, cache4waySetAssoc, strInd)) != -1) {
					std::cout << "\n\npage " << x << " already exists in cache ";
				}
				else
				{
					cache4waySetAssoc[j[strInd]][strInd] = x;
					std::cout << "\n\npage " << x << " has been allocated frame [" << j[strInd] << "][" << strInd << "]";
					j[strInd]++;
				}
			}
			else
			{
				if ((res = found4WaySetAssocCahe(x, cache4waySetAssoc, strInd)) != -1) {
					lru(listLRU, strInd, res);
					std::cout << "\n\npage " << x << " already exists in cachee ";
					std::cout << "\n\nNext page:\t";
				}
				else
				{
					std::cout << "\n\npage fault has occured";
					last = lru(listLRU, strInd, res, true);
					std::cout << "\n\npage " << x << " been allocated frame [" << last << "][" << strInd << "]";
					cache4waySetAssoc[last][strInd] = x;

				}
			}
		}

		std::cout << "\n\nCurrent cache is:\t";
		for (int i = 0; i < 4; i++)
		{
			for (int inI = 0; inI < 2; inI++)
			{
				std::cout << cache4waySetAssoc[i][inI];
				if (inI == 0) std::cout << ":";
			}
			std::cout << "\t";
		}
		std::cout << std::endl;
		std::cout << "\n\nNext page(enter -1 to stop):\t";
	}
	
	return 0;
}

