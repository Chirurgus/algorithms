#ifndef GURAD_HASH_H
#define GUARD_HASH_H

#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <iterator>

template<class K>
size_t hash(const K& k) {
	//Magic
	return 0;
}
template<>
size_t hash(const std::string& k) {
	size_t h {1};
	for (auto c : k) {
		h *= unsigned char(c);
	}
	return h;
}
/*
template<class K, class V> 
class HashTable {
public:
	using size_type = size_t;

	virtual HashTable() = 0;
	virtual HashTable(const HashTable& ht) = 0;
	virtual HashTable<K, V>& operator=(const HashTable& ht) = 0;
	virtual ~HashTable() = 0;
	virtual	V& operator[](const K& k) = 0;	
};
*/
/*
template<class K, class V>
class ListHashTable: public HashTable {
*/
template<class K, class V>
class ListHashTable {
public:
	using size_type = size_t;
	ListHashTable():_table(16) {}
	ListHashTable(const ListHashTable& ht):_table(ht._table) {}
	ListHashTable<K, V>& operator=(const ListHashTable& ht) {
		_table = ht._table;
	}
	~ListHashTable() {}

	V& operator[](const K& k) {
		size_type h = hash(k) % _table.size();
		/*
		std::list<std::pair<K, V>>::iterator
			it = std::find(_table[h].begin(),
						   _table[h].end(),
						   [&](std::pair<K, V>& p)->bool { return p.first == k; });
						   */
		std::list<std::pair<K, V>>::iterator it = _table[h].begin();
		for (std::list<std::pair<K, V>>::iterator it_end = _table[h].end();
			 it != it_end;
			 ++it) {
			if (it->first == k) {
				break;
			}
		}
		if (it != _table[h].end()) {
			return it->second;
		}
		else {
			_table[h].push_back({k,V{}});
			return std::prev(_table[h].end())->second;
		}
	}
private:
	std::vector<std::list<std::pair<K,V>>> _table;
};
#endif