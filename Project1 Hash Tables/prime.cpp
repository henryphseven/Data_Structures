#include "prime.h"

//source: https://en.wikipedia.org/wiki/Primality_test
bool is_prime(int n){

	if(n <= 1)
	   return 0;
	else if(n <= 3)
	   return 1;
	else if(n%2 == 0 || n%3 == 0)
	   return 0;
	int i = 5;
	while(i*i <= n){

		if(n%i == 0 || n%(i + 2) == 0)
		       return 0;

		i = i + 6;
	}

	return 1;
}

int prime_ceil(int N){ //return a prime number >= N

	if(N%2 == 0) N = N + 1;

	while(!is_prime(N)) N = N + 2;

	return N;
}

int prime_floor(int N){ //return a prime number <= N

	if(N%2 == 0) N = N - 1;

	while(!is_prime(N)) N = N - 2;

	return N;
}
