// lab2(Static).cpp : Defines the entry polong for the console application.
//

#include <windows.h>
#include <iostream>
#include <RSA.h>

//Використання статичної бібліотеки
using namespace std;


long main()
{
	
	myRSA my(3,7);

	int a = my.Crypt(3);
	int b = my.Decrypt(a);

	cout << a << endl;
	cout << b << endl;

	return 0;
}

