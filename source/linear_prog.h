#ifndef GUARD_LINEAR_PROG_H
#define GUARD_LINEAR_PROG_H

#include <vector>

#include "matrix.h"

double linear_prog_solve(const std::vector<double>& obj,
						 const std::vector<double>& b,
						 const Matrix<double>& a,
						 std::vector<double>& result);
//max v + x*obj 
//subject to a*x <= b

#endif