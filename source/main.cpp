#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include <string>
#include <map>

#include "bench.h"
#include "matrix.h"
#include "big_int_number_theory.h"
#include "big_int.h"
#include "big_int_test.h"

#include "s_big_int.h"
/*
#include "number_theory.h"
#include <random>
*/
using namespace std;

void test1() {
	Big_int u{534589782348923923ull};
	Big_int v {4384293859237492ull};
	for (unsigned i = 0; i < 100000; ++i) {
		Big_int tmp {u - v};
	}
}
void test_addition() {
	randomized_addition_testing(1000);
}
void test_substraction() {
	randomized_substraction_testing(1000);
}
void test_multiplication() {
	randomized_multiplication_testing(1000);
}
void test_division_mod() {
	randomized_division_testing(1000);
}
void test_right_shift() {
	randomized_right_shift_testing(1000);
}
void test_left_shift() {
	randomized_left_shift_testing(1000);
}

int main(int argc, char** argv) {
	auto m {1729ull};
	RSA_tester(m, 512);

	/*
	write_to_file_test();
	/*
	cout << "base 2**32:\n";
	benchmark(test_division_mod, 5, std::cout);
	benchmark(test_addition, 5, std::cout);
	benchmark(test_substraction, 5, std::cout);
	benchmark(test_multiplication, 5, std::cout);
	benchmark(test_right_shift, 5, std::cout);
	benchmark(test_left_shift, 5, std::cout);
	*/
}