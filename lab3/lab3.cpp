 

#include "stdafx.h"
#include "lab3_function.h"

int main()
{
	/*
	2. ������� ������� ��� ���������� ������� ����
	�� �������� ���� �������;
	*/
	int errorCode, errorNum, sysCode, type, errorImp;
	CHECK_ERROR(0x216D, errorNum, sysCode, type, errorImp);

	/*
	3. ������� �������, ��� ����� ��� ������� �� �������
	��������� ������� ����;
	*/
	CREATE_ERROR(errorNum, sysCode, type, errorImp, errorCode);
	_tprintf_s(_T("The error code is - %d\n"), errorCode);

	/*
	4. ������� ������� ��� ���������� ����� ������
	������������ ������� MessageBox. � ������� ��� ����������
	������ ����������������� �� ������� � ��������� ������
	����������, �� ����������. ��������� ��������� ���������
	����� ���������, ���������� �� ���������� ����� ���
	������ ASCII, UNICODE.
	*/
	_tsetlocale(LC_ALL, _T("Russian"));
	investigationMB();

	/*
	5. ���������� ��������� ��������� ����� � 
	���������� �� �������� �����������.
	*/
	extensionMB(_T("Some text"), 3.1415);
	
	/*
	6. �������� ������ ������ ������ ��� �������� 
	��������/��������� (������� �������, ��� �������� ��  ������).
	*/
	showConsoleMode();
	
	/*
	7. ������� ������� ��� �������� ������. 
	������ �� ������� �������� ������ � �������� ���� �����:
	 - ������� ����� ��������������� ��� ��������� �� ���������� 
	   �������;
	 - ����� ��������� ������ �� ���� ���������� �� �������� ������;
	 - ���������� 2 ������ ������ (� ������������ �� ��� 
	   ����������� �������, �� ���������). ���� ������ �� 
	   ����������, ������ ����� �� ���� *;
	 - ���� ����� �������� ��� �����������, �� ��������� 
	   ����������� ���������� ������. ϳ��� �������� ��������� 
	   �������� ������ �����;
	 - ������ ��������� �������, ���� ���� ������� �� ���� 
	   �� 8 �������										
	*/
	_TCHAR pass[MAX_PATH];

	// if the second argument is true - to hide the input symbols, 
	// if false - to show them
	while (!getPass(pass, true))
	{
		_tprintf_s(_T("%s"), _T("You make a mistake, try again, please"));
	}

	if (checkPass(pass))
	{
		_tprintf_s(_T("%s"), _T("This is a strong password\n"));
	}
	else
	{
		_tprintf_s(_T("%s"), _T("This is a weak password\n"));
	}

	/*
	8. ������� ������� ��� �������� � ��������� ����� �����,
	�������������� ������� �������� � ��������� ��� ������;
	*/
	_tprintf_s(_T("%s"), _T("Input number, please\n"));
	int inNum = inputNumber();
	_tprintf_s(_T("%s"), _T("You have entered:\n"));
	outputNumber(inNum);
	return 0;
}
