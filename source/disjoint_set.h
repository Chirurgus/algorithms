#ifndef GUARD_DISJOINT_SET_H
#define GUARD_DISJOINT_SET_H

#include <list>
#include <vector>

template<class T>
class disjoint_set{
private:
	struct node_s {
		node_s* p;
		T val;
	};
	std::vector<node_s*> _set;
public:
	using elem_t = node_s*;
	disjoint_set() {}
	disjoint_set(const disjoint_set& s) {}
	disjoint_set& operator=(const disjoint_set& s) {}
	void makeSet(const T& x) {
		node_s* node = new node_s {nulllptr, x};
		_set.push_back(node);
	}
	void unite(elem_t& x, elem_t& y) {
		node_s* x_set = this->findSet(x);
		node_s* y_set = this->findSet(y);
		x_set->p = y_set;
	}
	elem_t findSet(elem_t& x) {
		elem_t cur = x;
		if (cur->p) {
			cur->p = this->findSet(cur->p);
			return cur->p;
		}
		else {
			return cur;
		}
	}
};

#endif