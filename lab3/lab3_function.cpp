#include "stdafx.h"

/*
4. Складіть функцію для дослідження різних режимів
використання функції MessageBox. В функції для дослідження
повинні використовуватися усі наведені в довідникові іконки
повідомлень, які формуються. Перевірити можливість виведення
рядків російською, українською та англійською мовою для
режимів ASCII, UNICODE.
*/
void investigationMB()
{
	_TCHAR* titleEn = _T("Title");
	_TCHAR* messageEn = _T("Hello world!");
	_TCHAR* titleUa = _T("Заголовок");
	_TCHAR* messageUa = _T("Привіт мир!");
	_TCHAR* titleRu = _T("Заглавие");
	_TCHAR* messageRu = _T("Привет мир!");
	MessageBox(0, messageEn, titleEn, MB_ABORTRETRYIGNORE | MB_ICONHAND);
	MessageBox(0, messageEn, titleEn, MB_OK | MB_ICONINFORMATION);
	MessageBox(0, messageEn, titleEn, MB_YESNO | MB_ICONQUESTION);
	MessageBox(0, messageUa, titleUa, MB_CANCELTRYCONTINUE | MB_ICONERROR);
	MessageBox(0, messageRu, titleRu, MB_OKCANCEL | MB_ICONWARNING);
}

/*
5. Забезпечте можливість виведення рядків з
символьною та числовою інформацією.
*/
void extensionMB(_TCHAR* str = NULL, double x = NULL)
{
	if (str)
	{
		MessageBox(0, str, _T("The example string"), MB_OK | MB_ICONINFORMATION);
	}
	if (x)
	{
		_TCHAR buff[MAX_PATH];
		_stprintf_s(buff, _T("The number is: %g"), x);
		MessageBox(0, buff, _T("The message has number"), MB_OK | MB_ICONINFORMATION);
	}
}

/*
6. Визначте режими роботи консолі для пристроїв
введення/виведення (скласти функцію, яка виводить ці  режими).
*/
void showConsoleMode()
{
	DWORD ModeIn;
	DWORD ModeOut;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hIn != INVALID_HANDLE_VALUE && hOut != INVALID_HANDLE_VALUE)
	{
		GetConsoleMode(hIn, &ModeIn);
		GetConsoleMode(hOut, &ModeOut);
		_TCHAR* strIn = _T("");
		_TCHAR* strOut = _T("");
		SetConsoleMode(hIn, ModeIn | (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
		WriteConsole(hOut, _T("\nINPUT MODE:"), _tcslen(_T("\nINPUT MODE:")), 0, 0);
		if (ModeIn & ENABLE_LINE_INPUT == ENABLE_LINE_INPUT)
		{
			strIn = _T("\n ENABLE_LINE_INPUT");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_ECHO_INPUT == ENABLE_ECHO_INPUT)
		{
			strIn = _T("n ENABLE_ECHO_INPUT");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_PROCESSED_INPUT == ENABLE_PROCESSED_INPUT)
		{
			strIn = _T("\n ENABLE_PROCESSED_INPUT");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_WINDOW_INPUT == ENABLE_WINDOW_INPUT)
		{
			strIn = _T("\n ENABLE_WINDOW_INPUT");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_MOUSE_INPUT == ENABLE_MOUSE_INPUT)
		{
			strIn = _T("\n ENABLE_MOUSE_INPUT");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}

		if (ModeIn & ENABLE_INSERT_MODE == ENABLE_INSERT_MODE)
		{
			strIn = _T("\n ENABLE_INSERT_MODE");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_QUICK_EDIT_MODE == ENABLE_QUICK_EDIT_MODE)
		{
			strIn = _T("\n ENABLE_QUICK_EDIT_MODE");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_EXTENDED_FLAGS == ENABLE_EXTENDED_FLAGS)
		{
			strIn = _T("\n ENABLE_EXTENDED_FLAGS");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}
		if (ModeIn & ENABLE_AUTO_POSITION == ENABLE_AUTO_POSITION)
		{
			strIn = _T("\n ENABLE_AUTO_POSITION");
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}

		if (ModeIn == 0)
		{
			WriteConsole(hOut, strIn, _tcslen(strIn), 0, 0);
		}

		WriteConsole(hOut, _T("\nOUTPUT MODE:"), _tcslen(_T("\nOUTPUT MODE:")), 0, 0);
		if (ModeOut & ENABLE_WRAP_AT_EOL_OUTPUT == ENABLE_WRAP_AT_EOL_OUTPUT)
		{
			strOut = _T("\n ENABLE_WRAP_AT_EOL_OUTPUT");
			WriteConsole(hOut, strOut, _tcslen(strOut), 0, 0);
		}
		if (ModeOut & ENABLE_PROCESSED_OUTPUT == ENABLE_PROCESSED_OUTPUT)
		{
			strOut = _T("\n ENABLE_PROCESSED_OUTPUT");
			WriteConsole(hOut, strOut, _tcslen(strOut), 0, 0);
		}
		if (ModeIn == 0)
		{
			WriteConsole(hOut, strOut, _tcslen(strOut), 0, 0);
		}
	}
}

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
bool getPass(_TCHAR* pass, bool vis)
{
	BOOL Console = AllocConsole();
	BOOL res = FALSE;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD Mode;
	DWORD Count;

	if (hIn != INVALID_HANDLE_VALUE && hOut != INVALID_HANDLE_VALUE)
	{
		_TCHAR sym = _T('*');
		res = GetConsoleMode(hIn, &Mode);
		if (vis)
		{
			SetConsoleMode(hIn, Mode &~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT)); 
		}
		else
		{
			SetConsoleMode(hIn, Mode | (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
		}
		res = WriteConsole(hOut, _T("\nSet password, please\n"), _tcslen(_T("\nSet password, please\n")), &Count, 0);
		if (!vis)
		{
			res = ReadConsole(hIn, pass, MAX_PATH, &Count, 0);
		}
		else
		{
			_TCHAR passRepeat[MAX_PATH];
			int i = 0;
			for (;; ++i)
			{
				res = ReadConsole(hIn, &pass[i], 1, &Count, 0);
				if (pass[i] == _T('\r'))
				{
					break;
				}
				WriteConsole(hOut, &sym, 1, &Count, 0);
			}
			// confirm password
			WriteConsole(hOut, _T("\nConfirm password\n"), _tcslen(_T("\nConfirm password\n")), &Count, 0);
			int j = 0;
			for (;; ++j)
			{
				res = ReadConsole(hIn, &passRepeat[j], 1, &Count, 0);
				if (passRepeat[j] == _T('\r'))
				{
					break;
				}
				WriteConsole(hOut, &sym, 1, &Count, 0);
			}
			_TCHAR nLine = _T('\n');
			WriteConsole(hOut, &nLine, 1, 0, 0);
			
			if (i != j)
			{
				return FALSE;
			}
			while (i--)
			{
				if (pass[i] != passRepeat[i])
				{
					return FALSE;
				}
			}


			if (Console)
			{
				FreeConsole();
			}

			return TRUE;
		}
	}

	return res;
};

bool checkPass(_TCHAR* pass)
{
	_TCHAR* s = _T(" =-_={}[]()*/!@#$%^&~.,<>|");
	size_t sLen = _tcslen(s);
	int mask = 0;
	_TCHAR t;
	size_t passLen = _tcslen(pass);
	if (passLen < 8)
	{
		return false;
	}
	for (int i = 0; i<passLen; ++i)
	{
		t = pass[i];
		if (t >= _T('A') && t <= _T('Z'))
		{
			mask |= 1;
		}
		else
		{
			if (t >= _T('a') && t <= _T('z'))
			{
				mask |= 2;
			}
			else
			{
				if (t >= _T('0') && t <= _T('9'))
				{
					mask |= 4;
				}
				else
				{
					if (_tcschr(s, t))
					{
						mask |= 8;
					}
				}
			}
		}
	}
	return (mask != 15) ? FALSE : TRUE;
}

/*
8. Складіть функції для введення – виведення цілих чисел,
використовуючи функції введення – виведення для консолі;
*/
int inputNumber()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn != INVALID_HANDLE_VALUE)
	{
		DWORD dwCount = MAX_PATH;
		_TCHAR buff[MAX_PATH];
		SetConsoleMode(hIn, (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
		ReadConsole(hIn, &buff, MAX_PATH, &dwCount, 0);

		return _tstoi(buff);
	}
}

void outputNumber(int num)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut)
	{
		_TCHAR buff[MAX_PATH];
		_stprintf_s(buff, _T("%i\n"), num);
		WriteConsole(hOut, buff, _tcslen(buff), 0, 0);
	}
}