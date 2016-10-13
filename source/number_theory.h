#ifndef GAURD_NUMBER_THEORY_H
#define GUARD_NUMBER_THEORY_H

unsigned long long euclid_gcd(unsigned long long a,
				  unsigned long long b);
unsigned long long extended_euclid(const unsigned long long a,
					   const unsigned long long b,
						unsigned long long& x,
					   unsigned long long &y);

bool modular_equation(const unsigned long long a,
					  const unsigned long long b,
					  const unsigned long long n, 
					  unsigned long long& result,
					  unsigned long long& period);
bool modular_equation(const unsigned long long a,
					  const unsigned long long b,
					  const unsigned long long n, 
					  unsigned long long& result);
//computes a^b mod n
unsigned long long modular_exponentiation(const unsigned long long a,
							  const unsigned long long b,
							  const unsigned long long n);
//works only for primes
struct RSA_key {
	unsigned long long e;
	unsigned long long n;
};

unsigned long long RSA(const RSA_key& k, const unsigned long long M);

bool miller_rabin_prime_test(const unsigned long long n, const int s);


#endif