#include <cmath>

//Проект для створення динамічної бібліотеки
//Класс RSA

class myRSA
{
	long p;
	long q;
	long n;
	long eiler;
	long E;
	long D;

	//Функції для зовнішіх програм.
public:

	_declspec(dllexport) 	myRSA(long P, long Q);

	_declspec(dllexport)  long Crypt(long M);

	_declspec(dllexport)  long Decrypt(long M);

	//Внутрінні функції
private:

	long findCOP();

	long findNOD();

	bool isPrime(long n);

	bool RelativelyPrime(long a, long b);


};

extern "C" _declspec(dllexport)  myRSA getmyRSA(long a, long b);

