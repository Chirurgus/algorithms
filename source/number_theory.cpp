#ifndef GUARD_NUMBER_THEORY_CPP
#define GAURD_NUMBER_THEORY_CPP

#include <math.h>
#include <limits>
#include <random>

#include "number_theory.h"

/* !!!  This assumes a/b == floor(a/b) for the integer type of choice !!! */
unsigned long long euclid_gcd(unsigned long long a, unsigned long long b) {
	while (b != 0) {
		unsigned long long tmp = a;
		a = b;
		b = tmp % a;
	}
	return a;
}

unsigned long long extended_euclid(const unsigned long long a,
				   const unsigned long long b,
					unsigned long long & x, unsigned long long & y) {
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	unsigned long long d {extended_euclid(b, a % b , x, y)};
	unsigned long long tmp {x};
	x = y;
	y = tmp - (a / b) * y;//Asumes a/b == floor(a/b)
	return d;
}

bool modular_equation(const unsigned long long a,
					  const unsigned long long b,
					  const unsigned long long n, 
					  unsigned long long& result,
					  unsigned long long& period) {
	unsigned long long x {};
	unsigned long long y {};
	unsigned long long d {extended_euclid(a, n, x, y)};
	if (d % b != 0) {
		return false;
	}
	result = (x*(b / d)) % n;
	period = (n / d);
	return true;
}

bool modular_equation(const unsigned long long a,
					  const unsigned long long b,
					  const unsigned long long n,
					  unsigned long long & result) {
	unsigned long long tmp {};
	return modular_equation(a,b,n,result, tmp);
}

unsigned long long modular_exponentiation(unsigned long long a,
						  unsigned long long b,
						  unsigned long long n) {
	a = a % n;
	unsigned long long ret {1};
	for (; b > 0; b >>= 1) {
		if (b & 0x1) {
			ret = (ret*a) % n;
		}
		a = (a*a) % n;
	}
	return ret;
}

unsigned long long RSA(const RSA_key& k, const unsigned long long M) {
	return modular_exponentiation(M, k.e, k.n);
}

bool non_primality_witness(const unsigned long long a, const unsigned long long n) {
	long long int t {0};
	unsigned long long u {n - 1};//u is even
	while (u % 2 == 0) {
		u /= 2;
		++t;
	}
	unsigned long long x {modular_exponentiation(a, u, n)};
	for (long long int i = 0; i < t; ++i) {
		unsigned long long prev {x};
		x = x*x % n;
		if (x == 1 && prev != 1 && prev != n - 1) {
			return true;
		}
	}
	if (x != 1) {
		return true;
	}
	return false;
}

bool miller_rabin_prime_test(const unsigned long long n, const int s) {
	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned long long> rand(2, n - 1);
	for (int i = 0; i < s; ++i) {
		if (non_primality_witness(rand(generator), n)) {
			return false;
		}
	}
	return true;
}

#endif
