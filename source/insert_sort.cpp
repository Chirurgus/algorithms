#ifndef GUARD_INSERT_SORT_CPP
#define GUARD_INSERT_SORT_CPP

#include <vector>
#include <iterator>

#include "insert_sort.h"
	
using namespace std;

template<class T>
void insertSortInternal(vector<T>& a) {
	for (vector<T>::size_type i = 1; i < a.size(); ++i) {
		for (vector<T>::size_type j = i; j != 0 && a[j] < a[j - 1]; --j) {
			T tmp = a[j];
			a[j] = a[j - 1];
			a[j - 1] = tmp;
		}
	}
}

template<class T>
void insertSort(vector<T>& a) {
	//insertSortInternal(a);
	insertSort(a.begin(), a.end());
}

template<class I>
void insertSort(I begin, I end) {
	typedef typename std::iterator_traits<I>::value_type vecTy;

	for (I i = begin; i != end; ++i) {
		I min = i;
		for (I j = i + 1; j != end; ++j) {
			if (*min > *j) {
				min = j;
			}
		}
		vecTy tmp = *min;
		*min = *i;
		*i = tmp;
	}
}
#endif