
#include "tchar.h"
#include "iostream"
#include "locale.h"
#include <windows.h>
#include <cstdlib>
#include <stdio.h>

//Виконав: студент групи ПЗСпз-16-1 Цуканов Олександр Олегович

//Лабораторна робота №1

//Назва роботи:
//Розробка універсальних додатків для різних типів кодувань символьної інформації.

//Мета роботи:
//Навчитися опрацьовувати тексти для обох типів кодування(ASCII кодування та UNICODE
//кодування), при чому сама програма не повинна залежати від обраного способу.

//Хід роботи:

//1. Складіть програму для програмної перевірки типу кодування, заданого за замовчуванням
//(визначте довжину в байтах типу TCHAR).

//макрос для універсальної об’яви типу даних і їх ініціалізації
#ifdef UNICODE
typedef wchar_t TCHAR;
#define	TEXT(a)	L##a
#else
typedef char TCHAR;
#define TEXT(a)	a
#endif

TCHAR symbol = TEXT('x');
TCHAR array[] = TEXT("This is array");

#ifdef UNICODE
#define	_tcslen	wcslen
#else
#define	_tcslen	strlen
#endif

int w_cmp(const void* a, const void* b){

	if (*(wchar_t*)a <*(wchar_t*)b) return -1;
	if (*(wchar_t*)a == *(wchar_t*)b) return 0;
	if (*(wchar_t*)a >*(wchar_t*) b) return 1;
}

int main(){

	// 1. Визначаємо режим
	TCHAR UniversalChar = TEXT('a'), UniversalArray[] = TEXT("abc");
	printf("sizeof (UniversalChar) = %d sizeof (UniversalArray) = %d\n",
		sizeof(UniversalChar), sizeof(UniversalArray));
	
	/* 2. Визначте тип кодування по заданих макросах в командному рядку.Для цього визначимо
	командний рядок для трансляції програми :
	Properties→C/C++→Command Line
	Результат:
	/GS /analyze- /W3 /Zc:wchar_t /ZI /Gm /Od /sdl /Fd"Debug\vc120.pdb"
	/fp:precise /D "WIN32" /D "_DEBUG" 
	/D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /Oy- /MDd 
	/Fa"Debug\" /EHsc /nologo /Fo"Debug\" /Fp"Debug\OS.pch" 
	Усі змінні, які задаються при трансляції, задаються параметром /D. Якщо визначено змінну
	UNICODE, то використовується режим UNICODE, якщо така змінна не задана, то
	використовується режим ASCII.
	В даному випадку визначено змінну UNICODE, а отже – використовується режим UNICODE.
	*/
	
	/*
	3. Перемикніть режим завдання символу на протилежний.

	Для перемикання режиму з UNICODE в режим ASCII використовую:
	Properties→General→Character Set→Use Unicode character Set

	Для перемикання з режиму ASCII в режим UNICODE використовую:
	Properties→General→Character Set→Use Multi-Byte Character Set
	*/

	/*
	4. Після перемикання режиму знову перевірте тип символу за замовчуванням та командний
	рядок. (Після трансляції!!!)

	Результат:
	/GS /analyze- /W3 /Zc:wchar_t /ZI /Gm /Od /sdl /Fd"Debug\vc120.pdb" /fp:precise /D "WIN32" 
	/D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /Oy- /MDd /Fa"Debug\" /EHsc /nologo 
	/Fo"Debug\" /Fp"Debug\OS.pch"
	*/
	
	/*
	5. Задайте ПІБ членів своєї сім’її в ASCII та виведіть задані значення. Для виведення
	букв кирилиці необхідно встановити локальні режими.
	*/
	
	setlocale(LC_ALL, "Ukrainian");

	char Mother[] = "Цуканова Ольга Володимерівна\n";
	char Father[] = "Цуканов Олег Олександрович\n\n";
	printf(Mother);
	printf(Father);
	

	/*
	6. Переведіть задані рядки в  UNICODE  за допомогою функції(MultiByteToWideChar).
	
	7. Виведіть отриманий масив. Перевірте можливість виведення кожним з 2-х способів:
	функція _tprintf, якщо встановлено локальні режими; функція MessageBox
	*/
	wchar_t MotherUNICODE[sizeof(Mother)];	
	MultiByteToWideChar(CP_ACP, 0, Mother, sizeof(Mother), MotherUNICODE, sizeof(Mother));
	wprintf(L"Unicode мама %s\n", MotherUNICODE);
	MessageBox(NULL, MotherUNICODE, _T("Мама"), MB_OK);

	wchar_t FatherUNICODE[sizeof(Father)];
	MultiByteToWideChar(CP_ACP, 0, Father, sizeof(Father), FatherUNICODE, sizeof(Father));
	wprintf(L"Unicode папа %s\n", FatherUNICODE);
	MessageBox(NULL, FatherUNICODE, _T("Папа"), MB_OK);

	/*
	8. Виконайте упорядкування масиву рядків, заданих в UNICODE. Для сортування
	використовувати універсальну стандартну функцію qsort:
	*/

	wchar_t wSort[] = L"Я ИДУ ДОМОЙ";
	wprintf(L"Default: %s\n", wSort);
	qsort(wSort, wcslen(wSort), sizeof(wchar_t), w_cmp);
	wprintf(L"Sorted: %s\n\n", wSort);

	//9. Виконайте зворотне перетворення масиву з Unicode в ASCII.
	//10. Виведіть отриманий результат

	char MotherASCII[sizeof(Mother)];
	WideCharToMultiByte(CP_ACP, 0, MotherUNICODE, sizeof(Mother), MotherASCII, sizeof(Mother), NULL, NULL);
	printf("ASCII мама %s\n", MotherASCII);

	char FatherASCII[sizeof(Father)];
	WideCharToMultiByte(CP_ACP, 0, FatherUNICODE, sizeof(Father), FatherASCII, sizeof(Father), NULL, NULL);
	printf("ASCII папа %s\n", FatherASCII);

	/*
	Контрольні запитання і завдання

	1.	Навіщо створювати універсальні додатки для роботи з ASCII, UNICODE?

	Для забезпеченням універсальності та кроссплатформіннсоті програми.
	
	2.	Як забезпечити універсальність об’явлення символу?

	Ми можемо використовувати макроси. 

	3.	Які функції використовуються для введення та виведення універсальних типів даних?

	Наприклад, для визначення довжини рядка (в символах), типу тексту, перетворення типу
	кодування тексту тощо.

	4.	Як встановити локальні характеристики для виведення інформації російською
	(українською) мовами?

     Для цього використовуються	файл заголовку і функція: #include <locale.h>

	5.	Що треба змінити в програмі для упорядкування масиву ASCII символів?

	wchar_t на char.

	Висновки:

	У ході данної ЛР, я Навчився працювати з кодеровками ANSI та UTF-8 та використовувати їх на практиці.

	*/

	system("pause");
	return 0;
}
