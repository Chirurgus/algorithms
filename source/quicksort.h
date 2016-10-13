#ifndef GURAD_QUICKSORT_H
#define GUARD_QUICKSORT_H

#include <vector>
#include <random>
#include <iterator>

#include "insert_sort.h"

template<class T>
void quicksort(std::vector<T>& a);

#include "quicksort.cpp"

#endif