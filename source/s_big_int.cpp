#include "s_big_int.h"

s_Big_int::s_Big_int(long long i) {
	if (i < 0) {
		num = Big_int {unsigned long long (-i)};
		sign = true;
	}
	else {
		num = Big_int {unsigned long long (i)};
		sign = false;
	}
}

s_Big_int & s_Big_int::operator+=(const s_Big_int & v) {
	if (sign != v.sign) {
		if (num >= v.num) {
			num -= v.num;
		}
		else {
			num = v.num - num;
			sign = v.sign;
		}
	}
	else {
		num += v.num;
	}
	return *this;
}

s_Big_int & s_Big_int::operator-=(const s_Big_int & v) {
	return *this += -v;
}

s_Big_int & s_Big_int::operator*=(const s_Big_int & v) {
	sign = sign - v.sign;
	num *= v.num;
	return *this;
}

s_Big_int & s_Big_int::operator/=(const s_Big_int & v) {
	sign = sign - v.sign;
	num /= v.num;
	return *this;
}

s_Big_int & s_Big_int::operator%=(const s_Big_int & v) {
	num %= v.num;
	if (sign && num) {//if num == 0 we are done
		*this += v.num;
	}
	return *this;
}

Big_int s_Big_int::to_big_int() const {
	return num;
}

s_Big_int::operator bool() const {
	return *this != 0;
}

bool s_Big_int::operator<(const s_Big_int & v) const {
	if (sign != v.sign) {
		if (num == 0 && v.num == 0) {
			return false;
		}
		return sign;
	}
	else {
		if (sign) {
			return num > v.num;
		}
		else {
			return num < v.num;
		}
	}
}

bool s_Big_int::operator==(const s_Big_int & v) const {
	if (sign == v.sign) {
		return num == v.num;
	}
	else {
		if (num == 0 && v.num == 0) {
			return true;
		}
		return false;
	}
}

std::ostream & operator<<(std::ostream & os, const s_Big_int & n) {
	return os << (n.sign ? '-' : '+') << n.num.to_ull();
}

std::istream & operator >> (std::istream & is, s_Big_int & n) {
	// TODO: insert return statement here
	return is;
}

s_Big_int operator+(const s_Big_int & u, const s_Big_int & v) {
	s_Big_int ret {u};
	return ret += v;
}

s_Big_int operator-(const s_Big_int & u, const s_Big_int & v) {
	s_Big_int ret {u};
	return ret -= v;
}

s_Big_int operator*(const s_Big_int & u, const s_Big_int & v) {
	s_Big_int ret {u};
	return ret *= v;
}

s_Big_int operator/(const s_Big_int & u, const s_Big_int & v) {
	s_Big_int ret {u};
	return ret /= v;
}

s_Big_int operator%(const s_Big_int & u, const s_Big_int & v) {
	s_Big_int ret {u};
	return ret %= v;
}

bool operator>(const s_Big_int & u, const s_Big_int & v) {
	return !(u < v || u == v);
}

bool operator<=(const s_Big_int & u, const s_Big_int & v) {
	return !(u > v);
}

bool operator>=(const s_Big_int & u, const s_Big_int & v) {
	return !(u < v);
}

bool operator!=(const s_Big_int & u, const s_Big_int & v) {
	return !(u == v);
}

s_Big_int operator-(const s_Big_int & u) {
	return u * -1;
}

s_Big_int operator+(const s_Big_int & u) {
	return s_Big_int {u};
}
