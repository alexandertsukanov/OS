#include <cmath>

//Проект для створення статичної бібліотеки
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

		myRSA(long P, long Q);

		long Crypt(long M);

		long Decrypt(long M);

		//Внутрінні функції
	private:

		long findCOP();

		long findNOD();

		bool isPrime(long n);

		bool RelativelyPrime(long a, long b);


	};

extern "C"  myRSA getmyRSA(long a, long b);

