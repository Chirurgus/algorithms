#ifndef GUARD_INSERT_SORT_H
#define GUARD_INSERT_SORT_H

#include <vector>
#include <iterator>

template<class T>
void insertSort(std::vector<T>& a);

template<class I>
void insertSort(I begin, I end);

#include "insert_sort.cpp"

#endif