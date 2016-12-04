#ifndef GUARD_HEAPSORT_CPP
#define GUARD_HEAPSORT_CPP

#include <vector>

#include "heapsort.h"

using namespace std;

inline size_t parent(const size_t & pos) {
	return (pos - 1)/2;
}

inline size_t right(const unsigned long long & pos) {
	return pos * 2 + 2;
}

inline size_t left(const size_t & pos) {
	return pos * 2 + 1;
}

template<class T>
void maxHeapify(vector<T>& a, typename vector<T>::size_type i) {
	// O(lg n)
	while (true) {
		vector<T>::size_type  max_child =
			(a[left(i)] > a[right(i)] ? left(i) : right(i));

		if (a[max_child] > a[i]) {
			T tmp = a[i];
			a[i] = a[max_child];
			a[max_child] = tmp;

			i = max_child;
		}
		else
			break;
	}
}

template<class T>
void heapsort(vector<T>& a) {
	for (vector<T>::size_type i = a.size()/2; i-- > 0;) {
		maxHeapify(a, i);
	}

	for (vector<T>::size_type i = 0; i < a.size(); ++i) {
		T ret_int = a[0];
		a[0] = a[a.size() - i];
		a[a.size() - i] = ret_int;

		maxHeapify(a, 0);
	}
}

#endif