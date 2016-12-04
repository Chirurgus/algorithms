#ifndef GUARD_LARGEST_SUB_ARRAY_CPP
#define GUARD_LARGEST_SUB_ARRAY_CPP

#include <vector>
#include <iterator>
#include <limits>

#include "largest_sub_array.h"

using namespace std;

template<class I> 
typename iterator_traits<I>::value_type lsaCrossover(I& begin, const I mid, I& end) {
	typedef typename iterator_traits<I>::value_type Ty;
	Ty left_sum = numeric_limits<Ty>::min();
	Ty sum = 0;
	I start = mid - 1;
	for (I i = mid; i != begin;) {
		--i;
		sum += *i;
		if (sum > left_sum) {
			left_sum = sum;
			start = i;
		}
	}
	Ty right_sum = numeric_limits<Ty>::min();
	sum = 0;
	I finish = mid;
	for (I i = mid; i != end; ++i) {
		sum += *i;
		if (sum > right_sum) {
			right_sum = sum;
			finish = i;
		}
	}
	// Returning the results
	begin = start;
	end = finish;

	return right_sum + left_sum;
}

template<class I> 
typename iterator_traits<I>::value_type lsaInternal(I& begin, I& end) {
	if (end - begin == 1) {
		return *begin;
	}

	I mid = begin + ((end - begin) / 2);

	typedef typename iterator_traits<I>::value_type Ty;

	I left_begin = begin, left_end = mid;
	Ty left_sum = lsaInternal(left_begin, left_end);

	I right_begin = mid, right_end = end;
	Ty right_sum = lsaInternal(right_begin, right_end);

	I crossover_begin = begin, crossover_mid = mid, crossover_end = end;
	Ty crossover_sum = lsaCrossover(crossover_begin, crossover_mid, crossover_end);

	Ty ret_sum;
	if (left_sum > right_sum) {
		if (left_sum > crossover_sum) {
			ret_sum = left_sum;
			begin = left_begin;
			end = left_end;
		}
		else {
			ret_sum = crossover_sum;
			begin = crossover_begin;
			end = crossover_end;
		}
	}
	else if (right_sum > crossover_sum) {
		ret_sum = right_sum;
		begin = right_begin;
		end = right_end;
	}
	else {
		ret_sum = crossover_sum;
		begin = crossover_begin;
		end = crossover_end;
	}
	
	return ret_sum;
}

template<class T>
T largest_sub_array_recurrsive(vector<T>& a) {
	return lsaInternal(a.begin(), a.end());
}
template<class T>
T largest_sub_array_brute(const std::vector<T>& a) {
	typedef typename vector<T>::const_iterator I;

	I start, finish;
	T max_sum = numeric_limits<T>::min();
	/* this function also finds the start and finish of the max array */
	
	for (I i = a.begin(); i != a.end(); ++i) {
		T sum = 0;
		for (I j = i; j != a.end(); ++j) {
			sum += *j;
			if (sum > max_sum) {
				max_sum = sum;
				start = i;
				finish = j;
			}
		}
	}
	return max_sum;
}

template<class T>
T largest_sub_array_dynamic(vector<T>& a) {
	T max_array = numeric_limits<T>::min();
	T current_size = numeric_limits<T>::min();
	size_t current_start = 0;

	for (size_t i = 0; i < a.size(); ++i) {
		if (current_size + a[i] > current_size) {
			current_size += a[i];
		}
		if (current_size > max_size) {
			max_size = current_size;
		}
	}
}
#endif