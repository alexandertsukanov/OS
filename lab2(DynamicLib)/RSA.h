#include <cmath>

//������ ��� ��������� �������� ��������
//����� RSA

class myRSA
{
	long p;
	long q;
	long n;
	long eiler;
	long E;
	long D;

	//������� ��� ������� �������.
public:

	_declspec(dllexport) 	myRSA(long P, long Q);

	_declspec(dllexport)  long Crypt(long M);

	_declspec(dllexport)  long Decrypt(long M);

	//������� �������
private:

	long findCOP();

	long findNOD();

	bool isPrime(long n);

	bool RelativelyPrime(long a, long b);


};

extern "C" _declspec(dllexport)  myRSA getmyRSA(long a, long b);

