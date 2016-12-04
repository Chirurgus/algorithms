#ifndef GUARD_STRING_MATCH_H
#define GUARD_STRING_MATCH_H

#include <vector>
#include <string>

//knuth-morris-pratt algorithm
std::vector<std::string::size_type>
kmp_str_match(const std::string& str, const std::string& pat) {
	using sz_t = std::string::size_type;

	std::vector<sz_t> ret;
	//construct pi
	std::vector<sz_t> p(pat.size(), 0);
	for (sz_t i = 0, j = 1; j < pat.size(); ++j) {
		if (pat[i] == pat[j]) {
			++i;
		}
		else {
			while (i > 0 && pat[i] != pat[j]) {
				i = p[i - 1];
			}
		}
		p[j] = i;
	}
	//now do the matching
	for (sz_t i = 0, j = 0; j < str.size(); ++j) {
		if (str[i] == str[j]) {
			if (i == pat.size() - 1) {
				ret.push_back(j - pat.size() + 1);
				i = p[i];
			}
			else {
				++i;
			}
		}
		else {
			while (i > 0 && pat[i] != str[j]) {
				i = p[i - 1];
			}
		}
	}
	return ret;
}
#endif