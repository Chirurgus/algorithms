#include "linear_prog.h"

#include <assert.h>

#ifndef NDEBUG
#include <iostream>
template<class T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
	for (Matrix<T>::size_type i = 0, i_max = m.GetHeight();
		 i < i_max;
		 ++i) {
		for (Matrix<T>::size_type j = 0, j_max = m.GetWidth();
			 j < j_max;
			 ++j) {
			os << m(i, j) << ' ';
		}
		os << std::endl;
	}
	return os;
}
#endif
void pivot(Matrix<double>& m,
		   Matrix<double>::size_type row,
		   Matrix<double>::size_type col) {
	double factor = 1 / m(row, col);
	for (Matrix<double>::size_type i = 0, i_max = m.GetWidth();
		 i < i_max;
		 ++i) {
		m(row, i) *= factor;
	}
	for (Matrix<double>::size_type i = 0, i_max = m.GetHeight();
		 i < i_max;
		 ++i) {
		if (i == row) continue;
		factor = m(i, col);
		for (Matrix<double>::size_type j = 0, j_max = m.GetWidth();
			 j < j_max;
			 ++j) {
			m(i, j) -= m(row, j) * factor;
		}
	}
}

void phase1_lin_prog_solve(Matrix<double>& t) {
	const Matrix<double>::size_type t_height = t.GetHeight();
	const Matrix<double>::size_type t_width = t.GetWidth();
	while (true) {
			Matrix<double>::size_type pivot_column {0};
			//double max_coef {0};
			for (Matrix<double>::size_type i = 0, i_max = t_width - 1;
				 i < i_max;
				 ++i) {
				if (t(t_height - 1, i) > t(t_height - 1, pivot_column)) {
					pivot_column = i;
				}
			}
			if (t(t_height - 1, pivot_column) <= 0.1e-10) {
				t(t_height - 1, pivot_column) = 0;
				break;
			}
			//no positive coefficients in objective function
			//we are done

			Matrix<double>::size_type pivot_row {0};
			double min_ratio {std::numeric_limits<double>::max()};
			for (Matrix<double>::size_type i = 0, i_max = t_height - 2;
				 i < i_max;
				 ++i) {
				if (t(i, pivot_column) <= 0) continue;
				if (t(i, t_width - 1) / t(i, pivot_column) < min_ratio) {
					pivot_row = i;
				}
			}
			pivot(t, pivot_row, pivot_column);
	#ifndef NDEBUG
		std::cout << "Pivoting(row=" << pivot_row << ",col=" << 
			pivot_column << ")\n";
		std::cout << t;
		std::cout << std::endl;
	#endif
		}
}
void phase2_lin_prog_solve(Matrix<double>& t) {
	const Matrix<double>::size_type t_height = t.GetHeight();
	const Matrix<double>::size_type t_width = t.GetWidth();
	while (true) {
		Matrix<double>::size_type pivot_column {0};
		//double max_coef {0};
		for (Matrix<double>::size_type i = 0, i_max = t_width - 1;
			 i < i_max;
			 ++i) {
			if (t(t_height - 1, i) > t(t_height - 1, pivot_column)) {
				pivot_column = i;
			}
		}
		if (t(t_height - 1, pivot_column) <= 0.1e-10) {
			t(t_height - 1, pivot_column) = 0;
			break;
		}
		//no positive coefficients in objective function
		//we are done

		Matrix<double>::size_type pivot_row {0};
		double min_ratio {std::numeric_limits<double>::max()};
		for (Matrix<double>::size_type i = 0, i_max = t_height - 1;
			 i < i_max;
			 ++i) {
			if (t(i, pivot_column) <= 0) continue;
			if (t(i, t_width - 1) / t(i, pivot_column) < min_ratio) {
				pivot_row = i;
				min_ratio = t(i, t_width - 1) / t(i, pivot_column);
			}
		}
		pivot(t, pivot_row, pivot_column);
#ifndef NDEBUG
		std::cout << "Pivoting(row=" << pivot_row << ",col=" <<
			pivot_column << ")\n";
		std::cout << t;
		std::cout << std::endl;
#endif
	}
}

bool column_is_identity(const Matrix<double>& m,
						const Matrix<double>::size_type& col) {
	bool one_is_present {false};
	for (Matrix<double>::size_type i = 0, i_max = m.GetHeight();
		 i < i_max;
		 ++i) {
		double val {m(i,col)};
		if (val != 0) {
			if (val != 1) {
				return false;
			}
			if (one_is_present) {
				return false;
			}
			else {
				one_is_present = true;
			}
		}
	}
	return true;
}

bool column_is_memberwise_positive(const Matrix<double>& m,
								   const Matrix<double>::size_type& col) {
	for (Matrix<double>::size_type i = 0, i_max = m.GetHeight();
		 i < i_max;
		 ++i) {
		if (m(i, col) < 0) return false;
	}
	return true;
}

//max v + obj*x
//subect to s = b - a*x iff a*x <= b
//arguments assumed to be in standard form
double linear_prog_solve(const std::vector<double>& obj,
						 const std::vector<double>& b,
						 const Matrix<double>& a,
						 std::vector<double>& result) {
	assert(obj.size() == a.GetWidth());
	assert(b.size() == a.GetHeight());
	//construct simplex tableau
	const Matrix<double>::size_type t_height {a.GetHeight() + 2};
	const Matrix<double>::size_type 
		t_width {a.GetWidth() + a.GetHeight() + 2};
	Matrix<double> t(t_height, t_width, 0.0);
	for (Matrix<double>::size_type i = 0, i_max = a.GetHeight();
		 i < i_max;
		 ++i) {
		for (Matrix<double>::size_type j = 1, j_max = a.GetWidth() + 1;
			 j < j_max;
			 ++j) {
			t(i, j) = a(i, j - 1);
		}
		t(i, a.GetWidth() + 1 + i) = 1;
		t(i, t_width - 1) = b[i];
	}
	for (Matrix<double>::size_type i = 1, i_max = obj.size() + 1;
		 i < i_max;
		 ++i) {
		t(t_height - 2, i) = obj[i - 1];
	}
	for (Matrix<double>::size_type i = 0, i_max = a.GetHeight();
		 i < i_max;
		 ++i) {
		t(i, 0) = -1;
	}
	t(t_height - 1, 0) = -1;
	//finished constructing phase I tableau
#ifndef NDEBUG
	std::cout << "Phase I tableau finished:\n";
	std::cout << t << std::endl;
#endif
	if (!column_is_memberwise_positive(t, t_width - 1)) {
	//solve Phase I problem
		Matrix<double>::size_type pivot_row {0};
		for (Matrix<double>::size_type i = 0, i_max = t_height;
			 i < i_max;
			 ++i) {
			if (t(i, t_width - 1) < t(pivot_row, t_width - 1)) {
				pivot_row = i; 
			}
		}
		pivot(t, pivot_row, 0);
#ifndef NDEBUG
		std::cout << "Solving phase I problem\n";
		std::cout << "Pivoting(row=" << pivot_row << ",col=" << 
			0 << ")\n";
		std::cout << t;
		std::cout << std::endl;
#endif
		phase1_lin_prog_solve(t);
	}
	//copy simplex sub-tableau for phase 2 problem
	Matrix<double> t2(t_height - 1, t_width - 1);
	for (Matrix<double>::size_type i = 0, i_max = t_height - 1;
		 i < i_max;
		 ++i) {
		for (Matrix<double>::size_type j = 1, j_max = t_width;
			 j < j_max;
			 ++j) {
			t2(i, j - 1) = t(i, j);
		}
	}
#ifndef NDEBUG
	std::cout << "Phase II tableau finished:\n";
	std::cout << t2 << std::endl;
#endif
	//now solve Phase II problem;
	phase2_lin_prog_solve(t2);
#ifndef NDEBUG
	std::cout << "Phase II finished:\n";
	std::cout << t2;
	std::cout << std::endl;
#endif

	//now construct the answer
	Matrix<double>::size_type x_vector_size {obj.size()};
	result.clear();
	result.resize(x_vector_size);
	for (std::vector<double>::size_type i {0}, i_max {result.size()};
		 i < i_max;
		 ++i) {
		double x {};
		if (column_is_identity(t2, i)) {
			for (Matrix<double>::size_type j = 0, j_max = t2.GetHeight();
				 j < j_max;
				 ++j) {
				if (t2(j, i) == 1) {
					x = t2(j, t2.GetWidth() - 1);
					break;
				}
			}
		}
		else {
			x = 0;
		}
		result[i] = x;
	}

	return -t2(t2.GetHeight() - 1, t2.GetWidth() - 1);
}
