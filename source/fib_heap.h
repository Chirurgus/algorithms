#ifndef GAURD_FIB_HEAP_H
#define GUARD_FIB_HEAP_H

#include <list>
// AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
// i did it

//Note to self, max_deg of a node is bounded not by log[phi] (root_list.size) but by log[phi](#ofnodesintheheap)
class FibMinHeap {
public:
	struct node_s {
		friend FibMinHeap;
	private:
		int key;
		unsigned deg;
		node_s*  p;
		node_s* c;
		node_s* right;
		node_s* left;
		bool marked;
	};

	typedef node_s* FibNode;
	FibMinHeap():_min(0), _size(0), _h_size(0) {}
	FibMinHeap(const FibMinHeap& h);
	~FibMinHeap();

	FibMinHeap& operator=(const FibMinHeap& h);

	bool empty() const;
	node_s* insert(const int& key);
	void remove(node_s* node);
	int& min() const;
	int extract_min();//don't call this on an empty heap
	void decrease_key(node_s* node, const int& key);
private:
	void consolidate();
	void cascading_cut(node_s* node);

	void cutout(node_s* node);//cut out node from list;
	void insert_right(node_s* target, node_s* node);//insert node to the rightof target

	node_s* _min;
	unsigned _size;//root list size
	unsigned _h_size;//heap size
};

#endif