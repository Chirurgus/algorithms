
#ifndef GAURD_NUMBER_THEORY_H
#define GAURD_NUMBER_THEORY_H

#include <math.h>
#include <limits>
#include <random>

#include "big_int.h"

//works only for primes
struct RSA_key {
	Big_int e;
	Big_int n;
};
Big_int euclid_gcd(Big_int a, Big_int b) {
	while (b != 0) {
		Big_int tmp = a;
		a = b;
		b = tmp % a;
	}
	return a;
}

Big_int extended_euclid(const Big_int a,
					   const Big_int b,
						Big_int & x,Big_int & y) {
	if (b == 0) {
		x = Big_int{1};
		y = Big_int {0};
		return a;
	}
	Big_int d {extended_euclid(b, a%b , x, y)};
	Big_int tmp {x};
	x = y;
	y = tmp - (a/b) * y;
	return d;
}

bool modular_equation(const Big_int a,
					  const Big_int b,
					  const Big_int n, 
					  Big_int& result,
					  Big_int& period) {
	Big_int x {};
	Big_int y {};
	Big_int d {extended_euclid(a, n, x, y)};
	if (d%b != 0) {
		return false;
	}
	result = x*(b / d) % n;
	period = (n / d);
	return true;
}

bool modular_equation(const Big_int a,
					  const Big_int b,
					  const Big_int n,
					  Big_int & result) {
	Big_int tmp {};
	return modular_equation(a,b,n,result, tmp);
}

Big_int modular_exponentiation(Big_int a,
							  Big_int b,
							  Big_int n) {
	a = a % n;
	Big_int ret {1};
	for (; b > 0; b >>= 1) {
		if (b.inspect_bit(0)) {
			Big_int tmp = (ret*a);
			ret = tmp % n;
		}
		Big_int a_tmp = (a*a);
		a = a_tmp % n;
	}
	return ret;
}

Big_int RSA(const RSA_key& k, const Big_int M) {
	return modular_exponentiation(M, k.e, k.n);
}

bool non_primality_witness(const Big_int a, const Big_int n) {
	long long int t {0};
	Big_int u {n - 1};//u is even
	while (u% 2 == 0) {
		u /= Big_int{2};
		++t;
	}
	Big_int x {modular_exponentiation(a, u, n)};
	for (long long int i = 0; i < t; ++i) {
		Big_int prev {x};
		x = (x*x)% n;
		if (x == 1 && prev != 1 && prev != n - 1) {
			return true;
		}
	}
	if (x != 1) {
		return true;
	}
	return false;
}
bool miller_rabin_prime_test(const Big_int n, const int s) {
	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned long long> 
		rand(2, (n - 1).to_ull());
	for (int i = 0; i < s; ++i) {
		if (non_primality_witness(Big_int {unsigned long long(floor(rand(generator)))}, n)) {
			return false;
		}
	}
	return true;
}

#endif
