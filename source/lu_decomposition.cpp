
#include <limits>
#include <iostream>

#include "lu_decomposition.h"

std::ostream& operator<< (std::ostream& o, const Matrix<int>& A) {
	unsigned c = 0;
	for (unsigned i = 0; i < A.GetHeight(); ++i) {
		for (unsigned j = 0; j < A.GetWidth(); ++j) {
			if (j == c) o << " ";
			o << A(i, j) << " ";
		}
		c++;
		o << std::endl;
	}
	return o;
}


std::vector<int> lin_sys_equation_solver(Matrix<int> A, std::vector<int> b) {
	inplace_lu_decomposition(A);
	std::vector<int> x(b.size());
	for (unsigned i = 0; i < x.size(); ++i) {
		int sum = 0;
		for (unsigned j = 0; j < i; ++j) {
			sum += A(i, j) * x[j];
		}
		x[i] = (b[i] - sum);//no division because all diaganol elemts of L are = to 1
	}
	//now we have y such that Ux=y (y is stored in x);
	b = x;
	for (unsigned i = x.size() - 1; i-- > 0;){
		int sum = 0;
		for (unsigned j = x.size() - 1; j > i; --j) {
			sum += A(i, j) * x[j];
		}
		x[i] = (b[i] - sum)/A(i,i);
	}
	return x;
}

void inplace_lu_decomposition(Matrix<int>& A) {
	for (Matrix<int>::size_type n = 0; n < A.GetHeight(); ++n) {
		int pivot = A(n, n);
		for (Matrix<int>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			for (Matrix<int>::size_type j = n + 1; j < A.GetWidth(); ++j) {
				A(i, j) = A(i, j) - (A(i, n)*A(n, j)) / pivot;
			}
		}
		for (Matrix<int>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			A(i, n) = A(i, n) / pivot;
		}
	}
	std::cout << A << std::endl;
}

std::vector<Matrix<int>::size_type> inplace_lup_decomposition(Matrix<int>& A) {
	std::vector<Matrix<int>::size_type> p(A.GetHeight());
	for (unsigned i = 0; i < p.size(); ++i) p[i] = i;
	for (Matrix<int>::size_type n = 0; n < A.GetHeight(); ++n) {
		Matrix<int>::size_type max_pos = n;
		int max = std::numeric_limits<int>::min();
		for (Matrix<int>::size_type i = max_pos; i < A.GetHeight(); ++i) {
			if (A(i, n) > max) {
				max_pos = i;
				max = A(i, n);
			}
		}
		Matrix<int>::size_type tmp = p[n];
		p[n] = p[max_pos];
		p[max_pos] = tmp;
		for (Matrix<int>::size_type i = n; i < A.GetWidth(); ++i) {
			int tmp = A(n, i);
			A(n, i) = A(max_pos, i);
			A(max_pos, i) = tmp;
		}
		if (A(n, n) == 0) { //do nothing, but this means that i'll be dividng by 0
		}
		int pivot = A(n, n);
		for (Matrix<int>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			for (Matrix<int>::size_type j = n + 1; j < A.GetWidth(); ++j) {
				A(i, j) = A(i, j) - (A(i, n)*A(n, j)) / pivot;
			}
		}
		for (Matrix<int>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			A(i, n) = A(i, n) / pivot;
		}
	}
	std::cout << A << std::endl;
	return p;
}
//oh boy this screen is pretty big