

#include "stdafx.h"
#include <windows.h>

#define PHILOSOPHERS_NUM 7
#define STEP 13
#define MIN 100
#define MAX 200

CRITICAL_SECTION cs[PHILOSOPHERS_NUM];
HANDLE hSemaphore;

DWORD WINAPI ThreadPh(VOID *t);

int _tmain()
{
	for (int cnt = 0; cnt < PHILOSOPHERS_NUM; cnt++) {
		InitializeCriticalSection(&cs[cnt]);
	}

	HANDLE hPh[PHILOSOPHERS_NUM];
	hSemaphore = CreateSemaphore(NULL, PHILOSOPHERS_NUM - 1, PHILOSOPHERS_NUM - 1, _T("semaphore"));

	for (int cnt = 1; cnt <= PHILOSOPHERS_NUM; cnt++) {
		hPh[cnt - 1] = CreateThread(0, 0, ThreadPh, (VOID *)cnt, 0, NULL);
	}

	WaitForMultipleObjects(PHILOSOPHERS_NUM, hPh, true, INFINITE);

	for (int cnt = 0; cnt < PHILOSOPHERS_NUM; cnt++) {

		DeleteCriticalSection(&cs[cnt]);
	}

	CloseHandle(hSemaphore);

	return 0;
}

DWORD WINAPI ThreadPh(VOID *t)
{
	int phNum = (int) t;

	CRITICAL_SECTION* leftFork = &cs[ phNum - 1 ];
	CRITICAL_SECTION* rightFork = &cs[ (phNum) % PHILOSOPHERS_NUM ];

	for (int cnt = 0; cnt < STEP; cnt++) {

		printf("Philosopher %d is thinking\n", phNum);

		Sleep(MIN + rand() % (MAX - MIN));

		printf("Philosopher %d is starving\n", phNum);

		WaitForSingleObject(hSemaphore, INFINITE);

		EnterCriticalSection(leftFork);

		ReleaseSemaphore(hSemaphore, 1, NULL);

		printf("Philosopher %d is taking left fork\n", phNum);

		EnterCriticalSection(rightFork);

		printf("Philosopher %d is taking right fork\n", phNum);

		printf("Philosopher %d is eating\n", phNum);

		Sleep(MIN + rand() % (MAX - MIN));

		printf("Philosopher %d is full\n", phNum);

		LeaveCriticalSection(rightFork);
		LeaveCriticalSection(leftFork);
	}

	return 0;
}
