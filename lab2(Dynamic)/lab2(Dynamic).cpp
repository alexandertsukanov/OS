// lab2(Static).cpp : Defines the entry polong for the console application.
//

#include <windows.h>
#include <iostream>
#include <RSA.h>


using namespace std;
typedef myRSA(*fnPointer)(long, long);
//������������ �������� ��������
long main()
{ 

//������ �����

	myRSA my(3,7);
	
 int a = my.Crypt(3);
 int b = my.Decrypt(a);
 cout << a << endl;
 cout << b << endl;


 //������ �������������� �����

	//HINSTANCE load = LoadLibrary(L"C:\\Users\\Alexander\\Projects\\C++\\OS\\Debug\\lab2.dll");
	//if (load == NULL){

	//	cout << "No file\n";

	//}
	//else{
	//	cout << "Dll loaded\n";
	//}
	////�������� ������� �� ������ getmyRSA 
	//	fnPointer pointer = (fnPointer)GetProcAddress(load, "getmyRSA");	
	//	cout << pointer << endl;
	//	myRSA my = pointer(3, 7);
	//	int a = my.Crypt(3);
	//	int b = my.Decrypt(a);
	//	cout << a << endl;
	//	cout << b << endl;
	//	FreeLibrary(load);
	return 0;
}

