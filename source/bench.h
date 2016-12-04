#ifndef GUARD_BENCHMARK_H
#define GUARD_BEMCHMARK_H

#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ios>
#include <climits>
#include <math.h>


long double benchmark (void testfunc(), const unsigned num, std::ostream &os);

long double bench_time(void testfunc());

std::ostream& printBenchResults(const long double results, std::ostream& os);

#endif