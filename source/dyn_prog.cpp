#include <algorithm>
#include <limits>

#include "dyn_prog.h"

unsigned int naive_rod_cutting(const std::vector<int>& p, unsigned int length, std::vector<unsigned>& sol) {
	if (length == 0) return 0;
	unsigned ret = p[length];
	for (unsigned i = 1; i < length; ++i) {
		ret = std::max(ret, p[i] + naive_rod_cutting(p, length - i, sol));
	}
	return ret;
}

unsigned int less_naive_rod_cutting(const std::vector<int>& p, unsigned int n, std::vector<unsigned>& sol) {
	std::vector<unsigned> r(n + 1);
	for (unsigned i = 0; i < n + 1; ++i) {
		unsigned max = p[i];
		for (unsigned j = 0; j < i; ++j) {
			max = std::max(max, p[i - j] + r[j]);
			if (max < p[i - j] + r[j]) {
				max = p[i - j] + r[j];
				sol[i] = i - j;
			}
		}
		r[i] = max;
	}
	return r[n];
}

unsigned int matrix_order(const std::vector<dummy_matrix>& mtx,
						  Matrix<int>& sol) {
	Matrix<int> m(mtx.size(), mtx.size());
	//m(i,j) = min(m(i,k) + m(k+1,j) + mtx[i].height*mtx[j].length*mtx[k].length
	for (unsigned i = 0; i < mtx.size(); ++i) m(i, i) = 0;
	for (unsigned l = 1; l < mtx.size(); ++l) {
		for (unsigned j = l, i = 0; j < mtx.size(); ++i, ++j) {
			unsigned min = std::numeric_limits<unsigned>::max();
			for (unsigned k = i; k < j; ++k) {
				if (min > std::min(min, m(i, k) + m(k + 1, j) +
								   mtx[i].height*mtx[j].length*mtx[k].length)) {
					min = std::min(min, m(i, k) + m(k + 1, j) +
								   mtx[i].height*mtx[j].length*mtx[k].length);
					sol(i, j) = k;
				}
			}
			m(i, j) = min;
		}
	}
	return m(0, mtx.size() - 1);
}

unsigned stupid_lcs(char* a, unsigned a_n, char* b, unsigned b_n) {
	if (a_n == 0 || b_n == 0) return 0;
	if (a[a_n - 1] == b[b_n - 1]) return stupid_lcs(a, a_n - 1, b, b_n - 1) + 1;
	return std::max(stupid_lcs(a, a_n, b, b_n - 1), stupid_lcs(a, a_n, b, b_n - 1));
	}

Matrix<int> lcs(char* a, unsigned a_n, char* b, unsigned b_n) {
	Matrix<int> c(a_n + 1, b_n + 1);
	for (unsigned i = 0; i < a_n + 1; ++i) c(i, 0) = 0;
	for (unsigned i = 1; i < b_n + 1; ++i) c(0, i) = 0;
	for (unsigned i = 1; i < a_n + 1; ++i) {
		for (unsigned j = 1; j < b_n + 1; ++j) {
			if (a[i - 1] == b[j - 1]) c(i,j) = c(i - 1, j - 1) + 1;
			else c(i,j) = std::max(c(i, j - 1), c(i - 1, j));
		}
	}
	return c;
}

unsigned lcs_length(char* a, unsigned a_n, char* b, unsigned b_n) {
	Matrix<int> c(2, b_n + 1);
	for (unsigned i = 0; i < a_n + 1; ++i) c(i%2, 0) = 0;
	for (unsigned i = 1; i < b_n + 1; ++i) c(0, i) = 0;
	for (unsigned i = 1; i < a_n + 1; i = (i + 1) % 2) {
		for (unsigned j = 1; j < b_n + 1; ++j) {
			if (a[i - 1] == b[j - 1]) c(i,j) = c((i - 1), j - 1) + 1;
			else c(i,j) = std::max(c(i, j - 1), c((i - 1), j));
		}
	}
	return c(a_n%2, b_n);
}

std::string getLcs(Matrix<int>& c,char* a, unsigned a_n, char* b, unsigned b_n) {
	std::string ret;
	for (unsigned i = a_n, j = b_n; true;) {
		if (i == 0 || j == 0) return ret;
		if (a[i - 1] == b[j - 1]) {
			ret.push_back(a[i - 1]);
			--i, --j;
		}
		else if (c(i, j) == c(i - 1, j)) --i;//these last to conditions seem a bit
		else --j;							 //shaky, but they do seem to work.
	}
}

std::vector<int> longest_increasing_subsecquence(const std::vector<int>& a) {
	std::vector<unsigned> len(a.size(), 1);
	std::vector<int> prev(a.size(), -1);
	for (unsigned i = 1, length = a.size(); i < length; ++i) {
		unsigned max_len = 0;
		for (unsigned j = 0; j < i; ++j) {
			if (a[i] > a[j] && max_len < len[j] + 1) {
				max_len = len[j] + 1;
				prev[i] = j;
			}
		}
		len[i] = max_len;
	}
	unsigned max_pos = 0;
	for (unsigned i = 0, max_len = 0; i < a.size(); ++i) {
		if (max_len < len[i]) {
			max_pos = i;
			max_len = len[i];
		}
	}
	std::vector<int> ret_vec;
	for (unsigned i = max_pos; i != -1; i = prev[i]) {
		ret_vec.insert(ret_vec.begin(),a[i]);
	}
	return ret_vec;
}

std::vector<int> nlogn_lis(const std::vector<int>& a) {
	std::vector<int> last;
	last.push_back(a[0]);
	for (unsigned i = 1; i < a.size(); ++i) {
		if (last.back() < a[i]) {
			last.push_back(a[i]);
		}
		else {
			//O(log n)
			std::vector<int>::iterator j = std::lower_bound(last.begin(), std::prev(last.end()), a[i]);
			*j = a[i];
		}
	}
	return last;
}

int getWeight(const std::vector<float>& prob,
			  const std::vector<float>& leaf_prob,
			  Matrix<float>& w,
			  const unsigned i_pos,
			  const unsigned j_pos) {
	float ret = 0;
	for (unsigned i = i_pos; i < j_pos + 1; ++i) {
		ret += prob[i];
		ret += leaf_prob[i];
	}
	ret += leaf_prob[j_pos + 1];

	return ret;
}
void optimal_bst(const std::vector<float>& p,
				 const std::vector<float>& q,
				 Matrix<float>& e,
				 Matrix<float>& w) {
	e = Matrix<float>(p.size() + 1, p.size());
	w = Matrix<float>(p.size() + 1, p.size());
	Matrix<float> root(p.size(), p.size());

	for (unsigned i = 1; i < p.size() + 1; ++i) {
		e(i, i - 1) = q[i - 1];
		w(i, i - 1) = q[i - 1];
	}
	for (unsigned l = 1; l < p.size() + 1; ++l) {
		for (unsigned i = 1; i < p.size() - l + 2; ++i) {
			unsigned j = i + l - 1;
			e(i, j) = std::numeric_limits<float>::max();
			w(i, j) = w(i, j - 1) + p[j] + q[j];
			for (unsigned r = i; r < j + 1; ++r) {
				float t = e(i, r - 1) + e(r + 1, j) + w(i, j);
				if (t < e(i, j)) {
					e(i, j) = t;
					root(i, j) = r;
				}
			}
		}
	}

}

unsigned longest_palidrome(char* a, unsigned n) {
	Matrix<unsigned> len(n,n);
	for (unsigned i = 0; i < n; ++i) {
		for (unsigned j = 0; j <= i; ++j) {
			if (j == i) len(j, i) = 1;
			else if (a[j] == a[i]) len(j,i) = len(j + 1, i - 1) + 2;
			else len(j,i) = std::max(len(j, i -1), len(j + 1, i));
		}
	}
	unsigned max = 0;
	for (unsigned i = 0; i < n; ++i) {
		for (unsigned j = 0; j <= i; ++j) {
			max = std::max(max, len(j, i));
		}
	}
	return max;
}