#pragma once
/*
2. Складіть функцію для визначення окремих полів
по значенню коду помилки;
*/
/*
errorCode - the error code
errorNum  - the error number
sysCode   - the error system code
type      - the error type (0 - system / 1 - user)
errorImp  - the error impotence
*/
#define CHECK_ERROR(errorCode, errorNum, sysCode, type, errorImp) \
{ \
	errorNum = ((1<<16)-1) & errorCode; \
	sysCode = (errorCode >> 16) & ((1 << 12) - 1); \
	type = (errorCode >> 29) & 1; \
	errorImp = (errorCode >> 30) & 3; \
}

/*
3. Складіть функцію, яка формує код помилки по заданих
значеннях окремих полів;
*/
#define CREATE_ERROR(errorNum, sysCode, type, errorImp, errorCode) \
{ \
	errorCode = errorNum | (sysCode << 16) | (type << 29) | (errorImp << 30); \
}

void investigationMB();
void extensionMB(_TCHAR*, double);
void showConsoleMode();
bool checkPass(_TCHAR*);
bool getPass(_TCHAR*, bool);
int inputNumber();
void outputNumber(int);