#ifndef GUARD_VEB_TREE_H
#define GUARD_VEB_TREE_H

#include <vector>

class vEBTree {
public:
	typedef unsigned long long size_type;

	vEBTree(unsigned u);//For now u is resicted to 2^(2^k)
	vEBTree(const vEBTree& t);
	~vEBTree();

	vEBTree& operator= (const vEBTree& t);
	
	void insert(int k);
	int remvoe(int k);

	int& sucessor(int k) const;
	int& predecessor(int k) const;
	int& max() const;
	int& min() const;
private:
	struct node_s {
		int min;
		int max;
		unsigned u;
		node_s* summary;
		std::vector<node_s*> cluster;

		static const int _nul = -1;
	};

	vEBTree() {};//enforce user defining _u;
	
	size_type insert_in_tree(int key, node_s* t);

	node_s* allocate_node(size_type u);

	node_s* _root;
}

#endif