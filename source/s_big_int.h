#ifndef GUARD_S_BIG_INT_H
#define GUARD_S_BIG_INT_H

#include "big_int.h"

class s_Big_int {
public:
	friend std::ostream& operator<<(std::ostream& os,
									const s_Big_int& n);
	friend std::istream& operator>>(std::istream& is,
									s_Big_int& n);

	s_Big_int() = default;
	s_Big_int(const Big_int& i): num {i}, sign {0} {}
	s_Big_int(long long i);
	
	s_Big_int& operator+=(const s_Big_int& v);
	s_Big_int& operator-=(const s_Big_int& v);
	s_Big_int& operator*=(const s_Big_int& v);
	s_Big_int& operator/=(const s_Big_int& v);
	s_Big_int& operator%=(const s_Big_int& v);
	
	Big_int to_big_int() const;

	explicit operator bool() const;

	bool operator<(const s_Big_int& v) const;
	bool operator==(const s_Big_int& v) const;
private:
	Big_int num {};
	bool sign {false};//true == negative
};

s_Big_int operator+(const s_Big_int& u,
						   const s_Big_int& v);
s_Big_int operator-(const s_Big_int& u,
						   const s_Big_int& v);
s_Big_int operator*(const s_Big_int& u,
						   const s_Big_int& v);
s_Big_int operator/(const s_Big_int& u,
						   const s_Big_int& v);
s_Big_int operator%(const s_Big_int& u,
						   const s_Big_int& v);

bool operator>(const s_Big_int& u, const s_Big_int& v);
bool operator<=(const s_Big_int& u, const s_Big_int& v);
bool operator>=(const s_Big_int& u, const s_Big_int& v);
bool operator!=(const s_Big_int& u, const s_Big_int& v);

s_Big_int operator-(const s_Big_int& u);
s_Big_int operator+(const s_Big_int& u);

#endif