
#include "RSA.h"
#include <cmath>
#include <stdio.h>


//Реалізація 


myRSA::myRSA(long P, long Q){
	p = P;
	q = Q;
	n = p * q;
	eiler = (p - 1) * (q - 1);
	E = myRSA::findCOP();
	D = myRSA::findNOD();
}

long myRSA::Crypt(long M){
	M = (long)pow(M, E) % n;
	return M;
};

long myRSA::Decrypt(long M){
	M = (long)pow(M, D) % n;
	return M;
};

long myRSA::findCOP(){
	long maxdivider = 0;
	long number = 0;
	long lowest = 0;
	for (long i = 1; i < eiler; i++)
	{
		if (isPrime(i) && i > 1){
			if (RelativelyPrime(i, eiler)){
				return i;
			}
			else{
				continue;
			}
		}
	}
	return 0;
}

long myRSA::findNOD(){

	for (long i = 0; i < eiler; i++)
	{
		if ((i * E) % eiler == 1 && i < eiler){
			return i;
		}
	}
}

bool myRSA::isPrime(long n){
	for (int i = 2; i < n; i++) {
		if (n%i == 0)
			return false;
	}
	return true;
}

bool myRSA::RelativelyPrime(long a, long b) {
	for (;;) {
		if (!(a %= b)) return b == 1;
		if (!(b %= a)) return a == 1;
	}
}
myRSA getmyRSA(long a, long b)
{
	return myRSA(a, b);
}


/* Відповіді на питання:






*/
