#ifndef GUARD_BIG_INT_H
#define GUARD_BIG_INT_H

#include <vector>
#include <cmath>
#include <random>
#include <iostream>

//Arbitrary size unsiged integer for use in 
//number-theoretic algorithms
//Stored in a usngied char vector in little-endian format

//Magic numbers
constexpr size_t char_bit_size = sizeof(unsigned char)*8;
constexpr size_t word_type_size = sizeof(unsigned char) * char_bit_size;
class Big_int {
public:
	using word_type = unsigned char;//assumed to be usigned
	using size_type = std::vector<word_type>::size_type;
	//assumed to be at lest 2*sizeof(word_type);
	using long_word_type = int;

	friend std::ostream& operator<<(std::ostream& os, const Big_int& n);
	friend std::istream& operator>>(std::istream& is, Big_int& n);

	Big_int()
		:num(1, 0) {}
	Big_int(unsigned char c)
		:num(1, c) {}
	Big_int(int i);
	Big_int(unsigned long long l);
	Big_int(size_type l): Big_int {static_cast<unsigned long long>(l)} {}

	Big_int(const Big_int& i): num(i.num) {}

	Big_int& operator=(const Big_int& i) = default;

	Big_int(Big_int&& mi): num (std::move(mi.num)) {}
	Big_int& operator=(Big_int&& mi) {
		num = std::move(mi.num);
		return *this;
	}

	~Big_int() {}



	// Exception Classes
	class Bad_int {};

	Big_int operator+(const Big_int& rhs) const;
	Big_int operator-(const Big_int& rhs) const;
	Big_int& operator*=(const Big_int& rhs);
	Big_int operator/(Big_int rhs) const;
	Big_int operator%(const Big_int& rhs) const;

	Big_int operator&(const Big_int& rhs) const;
	Big_int operator|(const Big_int& rhs) const;
	Big_int operator^(const Big_int& rhs) const;

	Big_int& operator>>=(const size_type rhs);
	Big_int& operator<<=(const size_type rhs);
	
	bool operator<(const Big_int& rhs) const;
	bool operator==(const Big_int& rhs) const;

	unsigned long long to_ull() const;

	//these two have HORRIBLE performance
	//	use only as shorthand for bit ops
	//	for extracting bits
	bool inspect_bit(const size_type sz) const;
	void set_bit(const size_type sz, const bool b);

	std::pair<Big_int, Big_int> division_with_rest(const Big_int& lhs,
												   const Big_int& rhs) const;
private:
	
	void left_bit_shift();
	std::vector<word_type> num;
};

inline Big_int& operator+=(Big_int& lhs ,
						const Big_int& rhs) {
	return lhs = lhs + rhs;
}
inline Big_int& operator-=(Big_int& lhs ,
						  const Big_int& rhs) {
	return lhs = lhs - rhs;
}
inline Big_int operator*(const Big_int& lhs ,
						  const Big_int& rhs) {
	Big_int ret {lhs};
	return ret *= rhs;
}
inline Big_int& operator/=(Big_int& lhs ,
						  const Big_int& rhs) {
	return lhs = lhs / rhs;
}
inline Big_int& operator%=(Big_int& lhs ,
						  const Big_int& rhs) {
	return lhs = lhs % rhs;
}

inline Big_int operator<<(const Big_int& lhs,
							const Big_int::size_type& rhs) {
	Big_int ret {lhs};
	return ret <<= rhs;
}
inline Big_int operator>>(const Big_int& lhs,
							const Big_int::size_type& rhs) {
	Big_int ret {lhs};
	return ret >>= rhs;
}
inline bool operator!=(const Big_int& lhs,
					   const Big_int& rhs) {
	return !(lhs == rhs);
}
inline bool operator>=(const Big_int& lhs,
					   const Big_int& rhs) {
	return !(lhs < rhs);
}
inline bool operator<=(const Big_int& lhs,
					   const Big_int& rhs) {
	return lhs < rhs || lhs == rhs;
}
inline bool operator>(const Big_int& lhs,
					  const Big_int& rhs) {
	return !(lhs <= rhs);
}

/* Other Helper Functions */
Big_int get_rand_big_int(const Big_int::size_type bit_size,
						const bool strict = true);

#endif
