#include <cmath>

//1.	��������� �������� ������� ��� ��������� ��������� RSA.��������� ����� ��� ������� �, �� ������ ����������������� � ������� ���������;
//2.	��������� ����� �� ������� ��� �������, ���� �����, �� ������� ����� ��������������� � �, �++ ������ � ���� ������ ���� ����������� ��������;

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

		myRSA(long P, long Q);
		~myRSA();
	
		long result();

		long Crypt(long M);

		long Decrypt(long M);
	
		//������� �������
	private:

		long findCOP();

		long findNOD();

		bool isPrime(long n);

		bool RelativelyPrime(long a, long b);


	};

	 extern "C" _declspec(dllexport) myRSA getmyRSA(long a, long b);

