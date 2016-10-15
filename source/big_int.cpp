#include "big_int.h"

#include <limits>
#include <algorithm>
#include <utility>
#include <limits>

#include <time.h>

#ifndef NDEBUG
#include <iostream>
#endif // !NDEBUG


// Helper functions

//This function calculates the sum of l and r
//returns the most least signifiact bits that fit into word_type(unsigned char)
//and sets left_over if any more bits carry over
Big_int::word_type adder(Big_int::word_type l, Big_int::word_type r,
						 bool carry_bit = false, bool* left_over = nullptr) {
	long long int i{r + l + carry_bit};
	Big_int::word_type ret {static_cast<Big_int::word_type>(i)};
	bool ret_bit {false};
	if (ret < i) {
		//overflow
		ret_bit = true;
	}
	if (left_over) {
		*left_over = ret_bit;
	}
	return ret;
}



// !Helper functions

Big_int::Big_int(int i)
	:Big_int(static_cast<unsigned long long>(i)) {}

Big_int::Big_int(unsigned long long l)
	:num(sizeof(unsigned long long), {}) {
	for (int i = 0; i < sizeof(unsigned long long); ++i, l >>= char_bit_size) {
		unsigned long long mask {0x1};
		word_type w {0};
		for (int j = 0; j < char_bit_size; ++j, mask <<= 1) {
			w |= l & mask;
		}
		num[i] = w;
	}
}

Big_int Big_int::operator+(const Big_int& rhs) const {
	std::vector<word_type> vec {};
	bool carry_bit {false};

	auto rhs_sz = rhs.num.size();
	auto lhs_sz = num.size();
	vec.reserve(rhs_sz > lhs_sz ? rhs_sz : lhs_sz);

	size_type i {0};
	for (; i < rhs_sz && i < lhs_sz; ++i) {
		vec.push_back(adder(rhs.num[i], num[i], carry_bit, &carry_bit));
	}
	for (; i < rhs_sz; ++i) {
		vec.push_back(adder(rhs.num[i], 0, carry_bit, &carry_bit));
	}
	for (; i < lhs_sz; ++i) {
		vec.push_back(adder(num[i], 0, carry_bit, &carry_bit));
	}
	if (carry_bit) {
		vec.push_back(1);
	}
	Big_int ret {};
	ret.num.swap(vec);
	return ret;
}

Big_int Big_int::operator-(const Big_int & rhs) const {
	Big_int ret {0};
	bool carry_bit {0};
	size_type i_max = 
		char_bit_size *
		(rhs.num.size() < num.size() ? num.size() : rhs.num.size());
	for (size_type i = 0; i < i_max; ++i) {
		if (carry_bit) {
			if ((*this).inspect_bit(i)) {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 1);
					carry_bit = 1;
				}
				else {
					ret.set_bit(i, 0);//redundant
					carry_bit = 0;
				}
			}
			else {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 0);//redudndant
					carry_bit = 1;
				}
				else {
					ret.set_bit(i, 1);
					carry_bit = 1;
				}
			}
		}
		else {
			if ((*this).inspect_bit(i)) {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 0);//redudant
					carry_bit = 0;
				}
				else {
					ret.set_bit(i, 1);
					carry_bit = 0;
				}
			}
			else {
				if (rhs.inspect_bit(i)) {
					ret.set_bit(i, 1);
					carry_bit = 1;
				}
				else {
					ret.set_bit(i, 0);//redudnant
					carry_bit = 0;
				}
			}
		}
	}
	//if carry_bit == true, the resulting number is negative
	return ret;
}

Big_int& Big_int::operator*=(const Big_int & rhs) {
	/*
	Big_int ret {};
	Big_int tmp {rhs};
	for (size_type i = 0, i_max = num.size() * char_bit_size;
		 i < i_max;
		 ++i, tmp.left_bit_shift()) {
		if ((*this).inspect_bit(i)) {
			ret += tmp;
		}
	}
	return ret;
	*/
	size_type u_sz {num.size() - 1};
	while (u_sz && num[u_sz] == 0)  --u_sz;
	size_type v_sz {rhs.num.size() - 1};
	while (v_sz && rhs.num[v_sz] == 0) --v_sz;
	
	Big_int ret;
	ret.num.resize(u_sz + v_sz + 2, 0);

	
	for (size_type i = 0; i < u_sz + 1; ++i) {
		long_word_type k {0};
		for (size_type j = 0; j < v_sz + 1; ++j) {
			long_word_type tmp {num[i] * rhs.num[j] + ret.num[i+j] + k};
			k = tmp >> word_type_size;
			ret.num[i+j] = 
				(tmp << word_type_size) >> word_type_size;
		}
		ret.num[i + v_sz + 1] = k;
	}
	return *this = ret;
}

Big_int Big_int::operator/(Big_int rhs) const {
	return division_with_rest(*this, rhs).first;
}

Big_int Big_int::operator%(const Big_int & rhs) const {
	return division_with_rest(*this, rhs).second;
}

bool Big_int::operator<(const Big_int& rhs) const {
	size_type rhs_sz {rhs.num.size() - 1};
	size_type lhs_sz {num.size() - 1};
	while (rhs_sz < lhs_sz) {
		if (num[lhs_sz])
			return false;
		--lhs_sz;
	}
	while (lhs_sz < rhs_sz) {
		if (rhs.num[rhs_sz])
			return true;
		--rhs_sz;
	}
	//now lhs_sz == rhs_sz
	for (size_type i = lhs_sz + 1; i-- > 0;) {
		if (num[i] < rhs.num[i]) {
			return true;
		}
		if (num[i] > rhs.num[i]) {
			return false;
		}
	}
	//if we got here the numbers are equal
	return false;	
	//Old version
	/*
	size_type max_sz = 
		char_bit_size *
		(rhs.num.size() < num.size() ? num.size() : rhs.num.size());
	for (size_type i = max_sz; i-- > 0;) {
		if (rhs.inspect_bit(i) && !(*this).inspect_bit(i)) {
			return true;
		}
		if ((*this).inspect_bit(i) && !rhs.inspect_bit(i)) {
			return false;
		}
	}
	return false;
	*/
}

bool Big_int::operator==(const Big_int & rhs) const {
	size_type lhs_sz {num.size() - 1};
	size_type rhs_sz {rhs.num.size() - 1};
	while (lhs_sz && num[lhs_sz] == 0) --lhs_sz;
	while (rhs_sz && rhs.num[rhs_sz] == 0) --rhs_sz;
	if (rhs_sz != lhs_sz)
		return false;
	for (size_type i = rhs_sz + 1; i-- > 0;) {
		if (num[i] != rhs.num[i])
			return false;
	}
	return true;
	//Old (wierdly complicated) lazy version
	/*
	size_type max_sz = 
		char_bit_size *
		(rhs.num.size() < num.size() ? num.size() : rhs.num.size());
	for (size_type i = 0; i < max_sz; ++i) {
		if ((*this).inspect_bit(i) != rhs.inspect_bit(i)) {
			return false;
		}
	}
	return true;
	*/
}

unsigned long long Big_int::to_ull() const {
	unsigned long long ret {0};
	unsigned long long mask {0x1};
	for (size_type i = 0, i_max = sizeof(unsigned long long) * char_bit_size; i < i_max; ++i) {
		if (inspect_bit(i)) {
			ret |= mask;
		}
		mask <<= 1;
	}
	return ret;
}

bool Big_int::inspect_bit(const size_type sz) const {
		const size_type i = sz / char_bit_size;
		if (i >= num.size())
			return 0;
		return static_cast<bool>(num[i] &
								 0x1 << sz % char_bit_size);
	}

void Big_int::set_bit(const size_type sz, const bool b) {
	const size_type i = sz / char_bit_size;
	long long diff = (i + 1) - static_cast<long long>(num.size());
	if (diff > 0) {
		for (size_type j = 0; j < diff; ++j) {
			num.push_back(0);
		}
	}
	word_type tmp = num[i];
	tmp ^= (-b ^ tmp) & (1 << sz % char_bit_size);
	num[i] = tmp;
}

void Big_int::left_bit_shift() {
	size_type max = num.size() * char_bit_size - 1;
	if (inspect_bit(max)) {
		num.push_back(0x1);
	}
	for (size_type i = max; i >= 1; --i) {
		set_bit(i, inspect_bit(i - 1));
	}
	set_bit(0, 0);
	//This is actually multiple multitudes slower, dunno why
	/*
	*this <<= 1;
	*/
}

Big_int& Big_int::operator>>= (const size_type rhs){
	size_type shift = rhs / word_type_size;
	size_type in_word_shift = rhs % word_type_size;
	for (size_type i = 0; i < num.size() - shift - 1; ++i) {
		num[i] = (num[i + shift] >> in_word_shift) |
			(num[i + shift + 1] << 
			(word_type_size - in_word_shift));
	}
	num[num.size() - shift - 1] =
		num[num.size() - 1] >> in_word_shift;
	for (size_type i = num.size() - shift;
		 i < num.size();
		 ++i) {
		num[i] = 0;
	}
	return *this;
	//Old lazy version
	/*
		return *this / Big_int(static_cast<size_type>(pow(2, rhs)));
		*/
}

Big_int& Big_int::operator<<=(size_type rhs) {
	//Old lazy version
	/*
	return *this * Big_int(static_cast<unsigned long long>(pow(2, rhs)));
	*/
	//Possible optimization:
	//		check if in_word_shift is nul, 
	//		if so all the bit shift ops are 
	//		not needed
	size_type shift = rhs / word_type_size;
	size_type in_word_shift = rhs % word_type_size;
	if (in_word_shift > 0) 
		num.resize(num.size() + shift + 1, 0);
	else 
		num.resize(num.size() + shift, 0);
	for (size_type i = num.size() - 1; i >= shift + 1; --i) {
			word_type tmp = (num[i - shift] << in_word_shift) |
			(num[i - shift - 1] >> (word_type_size - in_word_shift));
			num[i] = tmp;
	}
	num[shift] = num[0] << in_word_shift;
	for (size_type i = 0; i < shift; ++i) {
		num[i] = 0;
	}
	return *this;
}

Big_int get_rand_big_int(const Big_int::size_type bit_size,
						 const bool strict) {
	Big_int ret {0};
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::bernoulli_distribution dist(0.5);
	for (Big_int::size_type i = bit_size - 2;
	   i-- > 0;) {
	  ret.set_bit(i, dist(gen));
		}
	  if (strict) {
		  ret.set_bit(bit_size - 1, 1);
	  }
	return ret;
}

std::pair<Big_int, Big_int> Big_int::division_with_rest(const Big_int& rhs,
														const Big_int& lhs) const {
	size_type base = std::numeric_limits<word_type>::max() + 1;
	Big_int u = rhs;
	Big_int v = lhs;

	size_type v_sz {v.num.size() - 1};
	while (v_sz && v.num[v_sz] == 0) { --v_sz; }
	size_type u_sz {u.num.size() - 1};
	while (v_sz && u.num[u_sz] == 0) { --u_sz; }
	//Handle the case where u and v are same sizes
	if (v_sz == 0 || v == 0) {
		if (v == 0)
			throw Bad_int {};
		//Handle case where v is single radix-b digit
		Big_int q {0};
		q.num.resize(u_sz + 1, 0);
		word_type tmp {0};
		for (size_type i = u_sz; i-- > 0;) {
			q.num[i] = (tmp*base + u.num[i]) / v.num[0];
			tmp = (tmp*base + u.num[i]) - (q.num[i] * v.num[0]);
		}
		return {q, tmp};
	}
	if (u_sz == v_sz) {
		long_word_type qhat = u.num[u_sz] / v.num[v_sz];
		Big_int tmp = v * qhat;
		while (tmp > u) {
			--qhat;
			tmp -= v;
		}
		return {qhat, u - tmp};
		/*
		if (u_sz == u.num.size() - 1) {
			u.num.push_back(0);
		}
		++u_sz;
		*/
	}
	if (u_sz < v_sz) {
		return {0, u};
	}

	//Normalize
	//count number of leading 0s in v[v_sz]
	size_type shift {0};
	for (word_type i = v.num[v_sz]; i; i <<= 1) {
		if (i & (0x1 << word_type_size - 1))
			break;
		++shift;
	}
	//nomalize
	v <<= shift;
	u <<= shift;
	//u_sz += 1;
	u.num.resize(u_sz+1,0);
	//v_sz += shift / word_type_size;

	//place to put quotient
	Big_int q {0};
	q.num.resize(u_sz - v_sz + 1, 0);//TODO::check the size

	//main loop
	for (size_type j = q.num.size(); j-- > 0;) {//TODO:check the # of iteratations
		//Calculate qhat
		//TODO: check indexes in
		//	u.num[j + v_sz] << IT used to be this
		long_word_type qhat = (u.num[j + v_sz] * base +
							   u.num[j + v_sz - 1]) / v.num[v_sz - 1];
		long_word_type rhat = (u.num[j + v_sz] * base +
							   u.num[j + v_sz - 1]) - qhat * v.num[v_sz - 1];
			   /*
			   long_word_type rhat = (u.num[j + v_sz + 1] * base +
							   u.num[j + v_sz]) % v.num[v_sz];
							   */

		for (;;) {
			if (qhat >= base ||
				(qhat*v.num[v_sz - 1]) >
				(base*rhat + u.num[j + v_sz - 1])) {
				--qhat;
				rhat += v.num[v_sz-1];
				if (rhat < base) {
					continue;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}

		//I'm pretty sure that for additions
		//	and substractions below implicit
		//	over, and under flow would have
		//	worked just fine

		//substract from u;
		bool u_k {false};//substraction may only underflowr by 1
		long_word_type v_k {0};
		for (size_type i = 0; i < v_sz; ++i) {
			long_word_type v_tmp {qhat * v.num[i] + v_k};
			v_k = v_tmp >> word_type_size;
			word_type v_i =
				(v_tmp << word_type_size) >> word_type_size;
			long_word_type u_tmp {u.num[i + j] - v_i - u_k};
			u_k = static_cast<bool>(u_tmp >= base);
			word_type u_i =
				(u_tmp << word_type_size) >> word_type_size;
			u.num[i + j] = u_i;
		}
		//TODO: check indices
		v_k = u.num[j + v_sz] - v_k;
		u.num[j + v_sz] =
			(v_k << word_type_size) >> word_type_size;

		//check if we substracted too much;
		if (v_k >= base) {
			--qhat;
			u_k = false;
			for (size_type i = 0; i < v_sz; ++i) {
				long_word_type u_tmp {u.num[i + j] + v.num[i]};
				u.num[i + j] =
					(u_tmp << word_type_size) >> word_type_size;
				u_k = static_cast<bool>(u_tmp >= base);
			}
			//TODO: check indices
			u.num[j + v_sz] += u_k;
		}
		q.num[j] = qhat;
	}
	return {q, (u >> shift)};
}

std::ostream & operator<<(std::ostream & os, const Big_int & n) {
	for (const Big_int::word_type x : n.num) {
		if (!(os << x)) {
			break;
		}
	}
	return os;
}

std::istream & operator >> (std::istream & is, Big_int & n) {
	n.num.clear();
	for (Big_int::word_type x {}; is >> x;) {
		n.num.push_back(x);
	}
	return is;
}
