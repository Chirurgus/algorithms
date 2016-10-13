#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include <string>

#include "bench.h"
#include "matrix.h"
#include "big_int_number_theory.h"
#include "big_int.h"
/*
#include "number_theory.h"
#include <random>
*/
#include <map>
using namespace std;
void randomized_division_testing() {
	for (Big_int::size_type i = 0; i < 1000000; ++i) {
		Big_int t {get_rand_big_int(64, false)};
		Big_int u {get_rand_big_int(58, false)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		cout << t_int << '\n';
		if (u_int == 0) continue;
		if ((t / u) != (t_int / u_int)) {
			cout << "FOUND ONE!\n";
			ofstream os("error.txt", ios::app);
			os << t_int << " / " << u_int <<
				" != " << t_int / u_int << '\n';
			os.close();
		}
		if ((t % u) != (t_int % u_int)) {
			cout << "FOUND ONE!\n";
			ofstream os("error.txt", ios::app);
			os << t_int << " % " << u_int <<
				" != " << t_int / u_int << '\n';
			os.close();
		}
	}
}
void randomized_multiplication_testing() {
	for (Big_int::size_type i = 0; i < 1000000; ++i) {
		Big_int t {get_rand_big_int(32)};
		Big_int u {get_rand_big_int(32)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t * u) != (t_int * u_int)) {
			cout << "FOUND ONE!\n";
			ofstream os("error.txt", ios::app);
			os << t_int << " * " << u_int <<
				" != " << t_int / u_int << '\n';
			os.close();
		}
	}
}
void randomized_addition_testing() {
	for (Big_int::size_type i = 0; i < 1000000; ++i) {
		Big_int t {get_rand_big_int(63)};
		Big_int u {get_rand_big_int(63)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t + u) != (t_int + u_int)) {
			cout << "FOUND ONE!\n";
			ofstream os("error.txt", ios::app);
			os << t_int << " + " << u_int <<
				" != " << t_int / u_int << '\n';
			os.close();
		}
	}
}
void randomized_substraction_testing() {
	for (Big_int::size_type i = 0; i < 1000000; ++i) {
		Big_int t {get_rand_big_int(63)};
		Big_int u {get_rand_big_int(62)};
		unsigned long long t_int {t.to_ull()};
		unsigned long long u_int {u.to_ull()};
		if ((t - u) != (t_int - u_int)) {
			cout << "FOUND ONE!\n";
			ofstream os("error.txt", ios::app);
			os << t_int << " - " << u_int <<
				" != " << t_int / u_int << '\n';
			os.close();
		}
	}
}

Big_int pow(const Big_int& a, const Big_int::size_type& pwr) {
	Big_int ret {1};
	for (Big_int::size_type i = 0; i < pwr; ++i) {
		ret *= a;
	}
	return ret;
}

int main(int argc, char** argv) {
	//unsigned long long prime = 2305843009213693951;// == 2^61 - 1
	randomized_division_testing();
	randomized_addition_testing();
	randomized_substraction_testing();
	randomized_multiplication_testing();
	/*
	unsigned long long prime = 1997;// == 2^61 - 1
	unsigned long long base = 2;
	Big_int tmp {pow(Big_int{base}, prime) % 1997};
	cout << (tmp).to_ull() << '\n';
	cout << modular_exponentiation(Big_int {base}, Big_int {prime}, Big_int {prime}).to_ull() << '\n';
	cout << 2 / 200 << ' ' << 2 % 200 << '\n';
	*/
	/*
	cout << (Big_int{9223372036854775808ull} /
			 Big_int{2305843009213693951ull}).to_ull() << '\n';
	cout << 9223372036854775808 / 2305843009213693951 << '\n';
	cout << sizeof(unsigned long long) << '\n';
	*/
	//cout << modular_exponentiation(2, 179426549, 179426549) << '\n';
	/*
	default_random_engine gen;
	uniform_int_distribution<unsigned long long> rand(0, numeric_limits<unsigned long long>::max());
	unsigned long long ull = rand(gen);
	ull |= (0x1 << 63);
	ull |= 0x1;
	unsigned long long i {0};
	while (!miller_rabin_prime_test(ull, 1)) {
		cout << "another one failed" << i << '\n';
		ull += 2;
		++i;
	}
	cout << "found one!\n" << ull << '\n';
	*/	
}