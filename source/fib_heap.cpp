#include <limits>
#include <math.h>
#include <vector>

#include "fib_heap.h"
#include "stack.h"

FibMinHeap::FibMinHeap(const FibMinHeap & h) {
}

FibMinHeap::~FibMinHeap() {
}

FibMinHeap & FibMinHeap::operator=(const FibMinHeap & h) {
	return FibMinHeap();
}

bool FibMinHeap::empty() const {
	return this->_min == 0;
}

FibMinHeap::node_s* FibMinHeap::insert(const int & key) {
	++_h_size;
	node_s* new_node = new node_s;
	new_node->marked = false;
	new_node->key = key;
	new_node->deg = 0;
	new_node->p = 0;
	new_node->c = 0;
	insert_right(_min, new_node);
	return new_node;
}

void FibMinHeap::remove(node_s * node) {
	decrease_key(node, std::numeric_limits<int>::min());
	extract_min();
}

int & FibMinHeap::min() const {
	return this->_min->key;
}

int FibMinHeap::extract_min() {
	--_h_size;
	node_s* node = _min->c;
	for (unsigned i = 0, deg = _min->deg; i < deg; ++i) {
		node_s* next = node->right;
		cutout(node);
		insert_right(_min, node);
		node->p = 0;
		node = next;
	}
	//remove _min from root list;
	int ret = _min->key;
	node_s* to_delete = _min;
	_min = _min->right;
	cutout(to_delete);
	delete to_delete;
	if (_size > 0) {
		//now we "consolidate"
		consolidate();
	}
	return ret;
}

void FibMinHeap::decrease_key(node_s * node, const int& key) {
	if (key > node->key) return;
	/* else */
	node->key = key;
	if (node->p && node->p->key <= node->key) return;
	/* else */	
	//move node to root list
	cutout(node);
	insert_right(_min, node);
	//Do a cascading cut
}

void FibMinHeap::consolidate() {
	/*
	const double phi = (1 + sqrt(5)) / 2;
	unsigned max_deg = log(_size) / log(phi);
	std::vector<node_s*> A(max_deg, 0);
	std::vector<node_s*> root_list;
	root_list.reserve(_size);
	node_s* w = this->_min;
	do {
		root_list.push_back(w);
		w = w->right;
	} while (w != this->_min);
	for (w = root_list.back(), root_list.pop_back();
		 !root_list.empty();
		 w = root_list.back(), root_list.pop_back()) {
		node_s* x = w;
		unsigned d = x->deg;
		while (A[d] != 0) {
			node_s* y = A[d];
			if (x->key > y->key) {
				node_s* tmp = x;
				x = y;
				y = tmp;
			}
			cutout(y);
			insert_right(x->c, y);
			x->c = y->left;
			++x->deg;
			y->marked = false;
			A[d] = 0;
			++d;
		}
		A[d] = x;
	}
	_min = 0;
	for (unsigned i = 0; i < max_deg; ++i) {
		if (A[i] != 0) {
			if (_min == 0) {
				_min = A[i];
				_min->left = _min->right = _min;
			}
			else {
				insert_right(_min, A[i]);
				if (A[i]->key < _min->key) {
					_min = A[i];
				}
			}
		}
	}
	*/	

	
	const double phi = (1 + sqrt(5)) / 2;
	unsigned max_deg = double(log(_h_size)) / log(phi);
	std::vector<node_s*> tree_deg(max_deg + 1, 0);
	Stack<node_s*> root_list;
	node_s* cur_node = this->_min;
	do {
		root_list.push(cur_node);
		cur_node = cur_node->right;
	} while (cur_node != this->_min);
	while (!root_list.isEmpty()) {
		cur_node = root_list.pop();
		if (tree_deg[cur_node->deg] == 0) {
			tree_deg[cur_node->deg] = cur_node;
		}
		else {
			//merge the two trees
			//updating tree_deg
			while (tree_deg[cur_node->deg] != 0) {
				node_s* aux_node = tree_deg[cur_node->deg];
				if (aux_node->key < cur_node->key) {
					node_s* tmp = cur_node;
					cur_node = aux_node;
					aux_node = tmp;
				}
				cutout(aux_node);
				insert_right(cur_node->c, aux_node);
				if (cur_node->c != 0) --cur_node->deg;//to counteract insert_right increment, Yeah, I know.
				cur_node->c = aux_node;
				aux_node->marked = false;
				aux_node->p = cur_node;
				//update tree_deg
				tree_deg[cur_node->deg] = 0;
				++cur_node->deg;
			}
			tree_deg[cur_node->deg] = cur_node;
		}
	}
	_min = 0;
	for (unsigned i = 0; i < max_deg + 1; ++i) {
		node_s* node = tree_deg[i];
		if (node) {
			if (_min) {
				if (node->key < _min->key) {
					_min = node;
				}
			}
			else {
				_min = node;
			}
		}
	}
}

void FibMinHeap::cascading_cut(node_s* node) {
	if (node->p) {
		if (node->marked) {
			//put node into root list
			cutout(node);
			insert_right(_min, node);
			//tail recurrsion
			node = node->p;
		}
		else {
			node->p->marked = true;
		}
	}
}

void FibMinHeap::cutout(node_s * node) {
	node->right->left = node->left;
	node->left->right = node->right;
	if (node->p) {
		node->p->marked = true;
		--node->p->deg;
		if (node->p->deg == 0) {
			node->p->c = 0;
		}
	}
	else {
		if (node == _min) {
			_min = 0;
		}
		--_size;
	}
}

void FibMinHeap::insert_right(node_s * target, node_s * node) {
	if (target) {
		//update left-right pointers
		node->right = target->right;
		node->left = target;
		target->right->left = node;
		target->right = node;
		if (target->left == target) {
			target->left = node;
		}
		//update parent
		if (target->p) {
			++target->p->deg;
		}
		else {
			++_size;
		}
		node->p = target->p;
	}
	else {
		node->right = node->left = node;
		if (target == _min) {
			_min = node;
			node->p = 0;
			++_size;
		}
		else {//this might happen if let's say i pass a nul node->c ptr
			target = node;
		}
	}
}