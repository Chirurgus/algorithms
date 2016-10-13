#ifndef GUARD_B_TREE_H
#define GUARD_B_TREE_H

//As is the b-tree resides entirely in computer memory(RAM),
//but I do realise that in practice most of the B-Tree should be
//writen and read from the Hard-Drive - it is what this tree was designed for.

#include <list>
//not using my list from list.h because I want to be able to take advanatate of iterators
#include <iterator>

template<class T>
class BTree {
private:
	struct node_s {
		std::list<node_s*> _child;
		std::list<T> _key;
		bool isLeaf;
	};
public:
	typedef typename std::list<T>::size_type size_type;

	BTree(const size_type t): _t(t) {
		_root = new node_s;
		_root->isLeaf = true;
	}
	~BTree() {
		deleteTree(_root);
	}
	void insert(const T& key) {
		if (_root->_key.size() == 2 * _t - 1) splitRoot();
		insertKey(_root, key);
	}
	bool search(const T& key) {
		return 0 != findNode(_root, key);
	}
	void remove(const T& key) {
		if(_root->_key.size())
			removeKey(_root, key);
	}
private:
	BTree() {}//Enforce user giving the t factor when declaring a B-tree

	node_s* findNode(node_s* cur, const T& key) {
		while (true) {
			std::list<T>::const_iterator key_it = cur->_key.begin();
			std::list<node_s*>::size_type child_pos = 0;
			while (key_it != cur->_key.end() && key >= *key_it) {
				if (key == *key_it) {
					return cur;
				}
				++key_it, ++child_pos;
			}
			//now *key_it >= key
			
			if (cur->isLeaf) {
				return 0;//NULL
			}
			else {
				cur = *std::next(cur->_child.begin(), child_pos);
			}
		}
	}
	bool findKey(node_s* node, const T& key, typename std::list<T>::size_type& offset) {
		offset = 0;
		std::list<T>::const_iterator i = node->_key.begin();
		while (i != node->_key.end() && *i < key) ++i, ++offset;
		return i != node->_key.end() && *i == key;
	}
	//insertsKey into subtree rooted at key
	void insertKey(node_s* node, const T& key) {
		while (true) {
			std::list<T>::iterator i = node->_key.begin();
			std::list<node_s*>::size_type child_it_pos = 0;
			while (i != node->_key.end() && *i <= key) {
				 ++i,
				 ++child_it_pos;
			}
			if (node->isLeaf) {
				node->_key.insert(i, key);
				return;
			}
			//now I know that node is not a leaf, thus it has children
			std::list<node_s*>::iterator j = std::next(node->_child.begin(), child_it_pos);
			if ((*j)->_key.size() == 2 * _t - 1) {//Implies j->_child.size() == 2*_t
				splitChild(node, j, i);
			}
			else {
				node = *j;
			}
		}
	}
	void removeKey(node_s* node, const T& key) {
		//if key is in the node and node is leaf =>
		std::list<T>::size_type offset = 0;
		if (findKey(node, key, offset)) {
			if (node->isLeaf) {
		//	just remove the key from node (the min size condition will be verifield later)
				node->_key.erase(std::next(node->_key.begin(), offset));
			}

		//if key is in the node and node is not leaf
			else {
		//	if child preceding key has at lest _t keys
				std::list<node_s*>::iterator child = std::next(node->_child.begin(), offset);
				if ((*child)->_key.size() >= _t) {
		//		find key's predecessor in child, recursively delete it and replace key by it
					T pred = *std::prev((*child)->_key.end());
					removeKey(*child, pred);
					*std::next(node->_key.begin(), offset) = pred;
				}
				else {
		//	else if child after key has at last _t keys
					std::advance(child, 1);
					if ((*child)->_key.size() >= _t) {
		//		do same thing but to this child
					T pred = *std::prev((*child)->_key.end());
					removeKey(*child, pred);
					*std::next(node->_key.begin(), offset) = pred;
					}
		//	else merge both children, removing key in the process
					else {
						mergeChildren(node, offset);
						removeKey(*child, key);
					}
				}
			}
		}
		else {
		//if key is not in the node
		//	find the appropriate child to recurrse on to 
			std::list<node_s*>::iterator child = std::next(node->_child.begin(), offset);
		//		if this child has only _t - 1 keys
			if ((*child)->_key.size() == _t - 1) {
	//			if one of child's sibilings has at least t keys
			if (child != node->_child.begin() &&
				(*std::prev(child))->_key.size() >= _t) {
	//				move a key from the sibiling to parent, and from parent to child
				std::list<T>::iterator node_key_it = std::next(node->_key.begin(), offset - 1);
				std::list<node_s*>::iterator sibling_it = std::prev(child);
				(*child)->_key.push_front(*node_key_it);
				*node_key_it = (*sibling_it)->_key.back();
				if (!(*child)->isLeaf) {
					(*child)->_child.splice((*child)->_child.begin(),
						(*sibling_it)->_child,
											std::prev((*sibling_it)->_child.end()));
				}
				(*sibling_it)->_key.pop_back();
			}
			else if (child != std::prev(node->_child.end()) &&
					(*std::next(child))->_key.size() >= _t) {
	//				move a key from the sibiling to parent, and from parent to child
					std::list<T>::iterator node_key_it = std::next(node->_key.begin(), offset);
					std::list<node_s*>::iterator sibling_it = std::next(child);
					(*child)->_key.push_back(*node_key_it);
					*node_key_it = (*sibling_it)->_key.front();
					if (!(*child)->isLeaf) {
						(*child)->_child.splice((*child)->_child.end(),
							(*sibling_it)->_child,
												(*sibling_it)->_child.begin());
					}
					(*sibling_it)->_key.pop_front();
				}
		//			else
				else {
		//				merge child with one of the cibilings
					if (child != std::prev(node->_child.end()) &&
						(*std::next(child))->_key.size() == _t - 1) {
						child = mergeChildren(node, offset);
					}
					else if (child != node->_child.begin() &&
						(*std::prev(child))->_key.size() == _t - 1) {
						child = mergeChildren(node, offset - 1);
					}

				}
		
			}
		//			recurse on the appropriate child.
			removeKey(*child, key);
		}

	}

	//splits parent->_child[pos] node into two nodes, and updates parent->_child as needed.
	//assumes the child is full (has 2t children <=> 2t - 1 keys)
	//new_node will have _t - 1 keys, while the old one will have only t
	void splitChild(node_s* parent, typename std::list<node_s*>::iterator child_it,
									typename std::list<T>::iterator key_it) {
		node_s* child = *child_it;
		node_s* new_node = new node_s;
		new_node->isLeaf = child->isLeaf;
		new_node->_key.splice(new_node->_key.begin(),
							  child->_key,
							  std::next(child->_key.begin(), _t + 1),
							  child->_key.end());
		if (!new_node->isLeaf) {
			new_node->_child.splice(new_node->_child.begin(),
								  child->_child,
								  std::next(child->_child.begin(), _t + 1),
								  child->_child.end());
		}
		//now we need to move the median key to the parent
		//the median key is now in the _t'th position of child->_key
		parent->_key.splice(key_it, child->_key, std::prev(child->_key.end()));
		parent->_child.insert(std::next(child_it), new_node);
	}
	void splitRoot() {
		node_s* new_root = new node_s;
		new_root->isLeaf = false;
		new_root->_child.push_back(_root);
		splitChild(new_root, new_root->_child.begin(), new_root->_key.begin());
		_root = new_root;
	}
	void deleteTree(node_s* node) {
		if (!node->isLeaf) {
			for (std::list<node_s*>::iterator it = node->_child.begin();
				 it != node->_child.end();
				 ++it) {
				deleteTree(*it);
			}
		}
		delete node;
	}
	
	//merges two children into one _child[c1] and _child[c1+1]
	//asumes that both children have t - 1 keys <=> t children
	typename std::list<node_s*>::iterator mergeChildren(node_s* parent, typename std::list<node_s*>::size_type pos_offset) {
		std::list<node_s*>::iterator child1 = std::next(parent->_child.begin(), pos_offset),
			   child2 = std::next(parent->_child.begin(), pos_offset + 1);
		(*child1)->_key.splice((*child1)->_key.end(), 
							parent->_key,
							std::next(parent->_key.begin(), pos_offset));
		(*child1)->_key.splice((*child1)->_key.end(),
							(*child2)->_key,
							(*child2)->_key.begin(),
							(*child2)->_key.end());
		if (!(*child1)->isLeaf && !(*child2)->isLeaf) {//redundant but clear
			(*child1)->_child.splice((*child1)->_child.end(),
								  (*child2)->_child,
								  (*child2)->_child.begin(),
								  (*child2)->_child.end());
		}
		//we copied all information from child2, we can delete it.
		delete *child2;
		parent->_child.erase(std::next(parent->_child.begin(), pos_offset + 1));
		return child1;
	}

	node_s *_root;//root is the exeption to the below rules.
	size_type _t;//min #of children of a node = t => min # of keys = t - 1
				 //max # of children of a node = 2t => max # of keys = 2t - 1
};

#endif
