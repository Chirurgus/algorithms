#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ios>
#include <climits>
#include <math.h>

#include "bench.h"

long double benchmark(void testfunc(), const unsigned num = 3, std::ostream &os = std::cout) {
	std::vector<long double> time_val(num);
	
	for (auto i = 0; i < num; i++) {
		time_val[i] = bench_time(testfunc);
	}

	long double sum = 0;
	for (auto i = 0; i < num; i++) {
		sum += time_val[i];
	}

	long double average_time_ms = sum /(num * 1000000);

	printBenchResults(average_time_ms, os);

	return average_time_ms;
}

long double bench_time(void testfunc()) {
		std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
		testfunc();
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		return (end - begin).count();
}

std::ostream& printBenchResults(const long double results, std::ostream& os) {
	std::streamsize stream_size_tmp = os.precision();
	os << "Average time: "
		<< std::setprecision(log10(LLONG_MAX) + 1)
		<< results
		<< std::setprecision(stream_size_tmp)
		<< "ms." << std::endl;
	return os;
}