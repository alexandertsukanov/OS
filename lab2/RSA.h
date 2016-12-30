#include <cmath>

//1.	Визначите необхідні функції для реалізації алгоритму RSA.Визначите серед цих функцій ті, які повинні використовуватись в зовнішніх програмах;
//2.	Визначите умову по виклику для функцій, якщо відомо, що функції треба використовувати в С, С++ файлах і вони повинні бути максимально надійними;

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
		~myRSA();
	
		long result();

		long Crypt(long M);

		long Decrypt(long M);
	
		//Внутрінні функції
	private:

		long findCOP();

		long findNOD();

		bool isPrime(long n);

		bool RelativelyPrime(long a, long b);


	};

	 extern "C" _declspec(dllexport) myRSA getmyRSA(long a, long b);

