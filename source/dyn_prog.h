#ifndef GUARD_DYN_PROG_H
#define GUARD_DYN_PROG_H

#include <vector>

#include "matrix.h"

unsigned int naive_rod_cutting(const std::vector<int>& p, unsigned int length, std::vector<unsigned>& sol);
unsigned int less_naive_rod_cutting(const std::vector<int>& p, unsigned int n, std::vector<unsigned>& sol);

struct dummy_matrix {
	unsigned height;
	unsigned length;
};
unsigned int matrix_order(const std::vector<dummy_matrix>& mtx,
						  Matrix<int>& sol);

unsigned stupid_lcs(char* a, unsigned a_n, char* b, unsigned b_n);
Matrix<int> lcs(char* a, unsigned a_n, char* b, unsigned b_n);
unsigned lcs_length(char* a, unsigned a_n, char* b, unsigned b_n);
std::string getLcs(Matrix<int>& c,char* a, unsigned a_n, char* b, unsigned b_n);
std::vector<int> longest_increasing_subsecquence(const std::vector<int>& a);
std::vector<int> nlogn_lis(const std::vector<int>& a);


void optimal_bst(const std::vector<float>& prob,
				 const std::vector<float>& leaf_prob,
				 Matrix<float>& e,
				 Matrix<float>& w);//FUCK THIS

unsigned longest_palidrome(char* a, unsigned n);
#endif