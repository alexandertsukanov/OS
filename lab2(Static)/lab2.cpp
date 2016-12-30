// lab2(Static).cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "RSA.h"
//Робимо імпорт з статичної бібліотеки
//5.	Реалізуйте головну програму для статичної бібліотеки;

using namespace std;
typedef myRSA*(*fnPointer)();

long _tmain(long argc, _TCHAR* argv[])
{
	HINSTANCE load = LoadLibrary(L"C:\\Users\\Alexander\\Projects\\C++\\OS\\OS\\Debug\\lab2(Dynamic).dll");
	if (load == NULL){

		cout << "No file\n";

	}
	else{

		cout << "Dll loaded\n";
	}
	
		fnPointer pointer = (fnPointer)GetProcAddress(load, "getmyRSA");
		
		
		cout << pointer;
	/*rsa = RSACreator();*/
	
	/*int a = funct();*/
	/*cout << r;*/

	//myRSA *my = (myRSA*) (GetProcAddress( hi, "Create(3, 7)"));
	//my->Crypt(1);
	//
	//int a = (int) my;
	//printf("%d\n", a);
	/*myRSA rsa(3, 7);*/
	
	//long Number = 2;
	//long EncryptedNumber = rsa.Crypt(Number);
	//long UnencryptedNumber = rsa.Decrypt(EncryptedNumber);
	
	
	//printf("%d\n", UnencryptedNumber);

	return 0;
}

