 

#include "stdafx.h"
#include "lab3_function.h"

int main()
{
	/*
	2. Складіть функцію для визначення окремих полів
	по значенню коду помилки;
	*/
	int errorCode, errorNum, sysCode, type, errorImp;
	CHECK_ERROR(0x216D, errorNum, sysCode, type, errorImp);

	/*
	3. Складіть функцію, яка формує код помилки по заданих
	значеннях окремих полів;
	*/
	CREATE_ERROR(errorNum, sysCode, type, errorImp, errorCode);
	_tprintf_s(_T("The error code is - %d\n"), errorCode);

	/*
	4. Складіть функцію для дослідження різних режимів
	використання функції MessageBox. В функції для дослідження
	повинні використовуватися усі наведені в довідникові іконки
	повідомлень, які формуються. Перевірити можливість виведення
	рядків російською, українською та англійською мовою для
	режимів ASCII, UNICODE.
	*/
	_tsetlocale(LC_ALL, _T("Russian"));
	investigationMB();

	/*
	5. Забезпечте можливість виведення рядків з 
	символьною та числовою інформацією.
	*/
	extensionMB(_T("Some text"), 3.1415);
	
	/*
	6. Визначте режими роботи консолі для пристроїв 
	введення/виведення (скласти функцію, яка виводить ці  режими).
	*/
	showConsoleMode();
	
	/*
	7. Складіть функцію для введення пароля. 
	Вимоги до функції введення пароля і перевірки його якості:
	 - функцію можна використовувати для візуальних та консольних 
	   додатків;
	 - перед введенням паролю має бути запрошення до введення паролю;
	 - забезпечте 2 режими роботи (з відображенням та без 
	   відображення символів, які вводяться). Якщо символ не 
	   виводиться, замість нього має бути *;
	 - якщо режим введення без відображення, то необхідно 
	   забезпечити повторення паролю. Після введення необхідно 
	   порівняти обидва паролі;
	 - пароль вважається хорошим, якщо його довжина не менш 
	   ніж 8 символів										
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
	8. Складіть функції для введення – виведення цілих чисел,
	використовуючи функції введення – виведення для консолі;
	*/
	_tprintf_s(_T("%s"), _T("Input number, please\n"));
	int inNum = inputNumber();
	_tprintf_s(_T("%s"), _T("You have entered:\n"));
	outputNumber(inNum);
	return 0;
}
