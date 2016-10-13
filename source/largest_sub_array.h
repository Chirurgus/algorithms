#ifndef GUARD_LARGEST_SUB_ARRAY_H
#define GUARD_LARGEST_SUB_ARRAY_H

#include <vector>

template<class T>
T largest_sub_array_recurrsive(std::vector<T>& a);

template<class T>
T largest_sub_array_brute(std::vector<T>& a);

#include "largest_sub_array.cpp"

#endif