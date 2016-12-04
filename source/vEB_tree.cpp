/*
#include <math.h>

#include "vEB_tree.h"

vEBTree::vEBTree(unsigned u) {
	_root = allocate_node(u);
}

vEBTree::size_type vEBTree::insert_in_tree(int key, node_s* t) {
	if (t->u == 2) {
		if (key == 0) {
			t->min = 1;
		}
		else {//if key == 1
			t->max = 1;
		}
	}
	return size_type();
}

vEBTree::node_s * vEBTree::allocate_node(size_type u) {
	node_s* ret_ptr = new node_s;
	ret_ptr->u = u;
	ret_ptr->max = ret_ptr->min = node_s::_nul;
	if (u > 2) {
		const size_type sqrt_u = sqrt(u);
		ret_ptr->summary = allocate_node(sqrt_u);
		ret_ptr->cluster.reserve(sqrt_u);
		for (size_type i = 0; i < sqrt_u; ++i) {
			ret_ptr->cluster.push_back(allocate_node(sqrt_u));
		}
	}
	else {
		ret_ptr->summary = 0;
	}
	return ret_ptr;
}

void vEBTree::insert(int k) {
	
}

int & vEBTree::sucessor(int k) const {
}

int & vEBTree::predecessor(int k) const {
}

int & vEBTree::max() const {
	return _root->max;
}

int & vEBTree::min() const {
	return _root->min;
}
*/
