#ifndef GUARD_LU_DECOMPOSITION_H 
#define GUARD_LU_DECOMPOSITION_H

#include <vector>
#include <iostream>
#include <string>
#include <math.h>

#include "matrix.h"

//solves Ax=b for x
//reminders for self
//Ax=b and A=LU => LUx=b and let y = Ux => Ly=b
//findig y is easy, since L is a lower triangular matrix
//then we have Ux=y whitch is just as easy
std::vector<int> lin_sys_equation_solver(Matrix<int> A, std::vector<int> b);
void inplace_lu_decomposition(Matrix<int>& A);
std::vector<Matrix<int>::size_type> inplace_lup_decomposition(Matrix<int>& A);


template<class T>
void inplace_lu_decomposition(Matrix<T>& A) {
	for (Matrix<T>::size_type n = 0; n < A.GetHeight(); ++n) {
		T pivot = A(n, n);
		for (Matrix<T>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			for (Matrix<T>::size_type j = n + 1; j < A.GetWidth(); ++j) {
				A(i, j) = A(i, j) - (A(i, n)*A(n, j)) / pivot;
			}
		}
		for (Matrix<T>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			A(i, n) = A(i, n) / pivot;
		}
	}
}
template<class T>
std::vector<typename Matrix<T>::size_type> inplace_lup_decomposition(Matrix<T>& A) {
	std::vector<Matrix<T>::size_type> p(A.GetHeight());
	for (unsigned i = 0; i < p.size(); ++i) p[i] = i;
	for (Matrix<T>::size_type n = 0; n < A.GetHeight(); ++n) {
		Matrix<T>::size_type max_pos = n;
		T max = std::numeric_limits<int>::min();
		for (Matrix<T>::size_type i = max_pos; i < A.GetHeight(); ++i) {
			if (std::abs(A(i, n)) > max) {
				max_pos = i;
				max = std::abs(A(i, n));
			}
		}
		Matrix<T>::size_type tmp = p[n];
		p[n] = p[max_pos];
		p[max_pos] = tmp;
		for (Matrix<T>::size_type i = n; i < A.GetWidth(); ++i) {
			T tmp = A(n, i);
			A(n, i) = A(max_pos, i);
			A(max_pos, i) = tmp;
		}
		if (A(n, n) == 0) { //do nothing, but this means that i'll be dividng by 0
		}
		T pivot = A(n, n);
		for (Matrix<T>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			for (Matrix<T>::size_type j = n + 1; j < A.GetWidth(); ++j) {
				A(i, j) = A(i, j) - (A(i, n)*A(n, j)) / pivot;
			}
		}
		for (Matrix<T>::size_type i = n + 1; i < A.GetHeight(); ++i) {
			A(i, n) = A(i, n) / pivot;
		}
	}
	return p;
}

template<class T>
std::vector<T> lin_sys_equation_solver(Matrix<T> A, std::vector<T> b) {
	inplace_lu_decomposition(A);
	std::cout << A << std::endl;
	std::vector<T> x(b.size());
	for (unsigned i = 0; i < x.size(); ++i) {
		double sum = 0;
		for (unsigned j = 0; j < i; ++j) {
			sum += A(i, j) * x[j];
		}
		x[i] = (b[i] - sum);//no division because all diaganol elemts of L are = to 1
	}
	//now we have y such that Ux=y (y is stored in x);
	std::cout << x << std::endl;
	b = x;
	for (unsigned i = x.size(); i-- > 0;) {
		double sum = 0;
		for (unsigned j = x.size() - 1; j > i; --j) {
			sum += A(i, j) * x[j];
		}
		x[i] = (b[i] - sum) / A(i, i);
	}
	return x;
}

template<class T>
Matrix<T> lu_inverse(Matrix<T> A) {
	inplace_lu_decomposition(A);
	Matrix<T> ret(A.GetHeight(), A.GetWidth());
	for (unsigned i = 0; i < A.GetWidth(); ++i) {
		std::vector<T> b(A.GetHeight(), 0);
		b[i] = 1;
		//use b as storage
		b = lin_sys_equation_solver(A, b);
		for (unsigned j = 0; j < A.GetHeight(); ++j) {
			ret(j, i) = b[j];
		}
	}
	return ret;
}
#endif