#include "bst.h"
#ifndef GUARD_FAILED_BINARY_SEARCH_TREE_H
#define GUARD_FAILED_BINARY_SEARCH_TREE_H

//I am clueless as to why this absolutly refuses to work.
template<class T>
class BST {
public:
	BST();
	BST(const BST<T>& odd_tree);
	BST<T>& operator=(const BST<T>& rhs_tree);

	void insert(const T& key);
	void erase(const T& key);
	bool search(const T& key);
	
	bool isEmpty() const;
private:
	struct node_s;

	node_s* maxNode(node_s* node);
	node_s* minNode(node_s* node);
	void insertNode(node_s* node);
	void eraseNode(node_s* node);
	node_s* searchNode(const T& key);

	node_s* _root;
};

template<class T>
struct BST<T>::node_s {
	node_s* right;
	node_s* left;
	node_s* parent;
	T key;
};

template<class T>
BST<T>::BST(): _root(0) {}

template<class T>
BST<T>::BST(const BST<T>& odd_tree) {
}

template<class T>
BST<T>& BST<T>::operator=(const BST<T>& rhs_tree) {
}

template<class T>
void BST<T>::insert(const T & key) {
	BST<T>::node_s* node = new BST<T>::node_s;
	node->right = node->left = node->parent = 0;
	node->key = key;
	insertNode(node);
}

template<class T>
void BST<T>::erase(const T & key) {
	BST<T>::node_s* ptr = searchNode(key);
	if (ptr) 
		eraseNode(ptr);
}

template<class T>
bool BST<T>::search(const T & key) {
	BST<T>::node_s* ptr = searchNode(key);
	return ptr;//true if ptr != 0
}

template<class T>
bool BST<T>::isEmpty() const {
	return this->_root == 0;
}

template<class T>
BST<T>::node_s* BST<T>::maxNode(BST<T>::node_s* node) {
	while (node->right)
		node = node->right;
	return node;
}

template<class T>
BST<T>::node_s* BST<T>::minNode(BST<T>::node_s* node) {
	while (node->left)
		node = node->left;
	return node;
}

template<class T>
void BST<T>::insertNode(BST<T>::node_s* node) {
	BST<T>::node_s* ptr = this->_root;
	if (this->_root == 0) {
		this->_root = node;
		return;
	}
	while (true) {
		if (ptr->key >= node->key) {
			if (ptr->left) {
				ptr = ptr->left;
			}
			else {
				ptr->left = node;
				node->parent = ptr;
				break;
			} 
		}
		else {
			if (ptr->right) {
				ptr = ptr->right;
			}
			else {
				ptr->right = node;
				node->parent = ptr;
				break;
			}
		}
	}
}

template<class T>
void BST<T>::eraseNode(BST<T>::node_s* node) {
	//for the love of god don't call this on an null node.
	if (node->right == 0) {
		if (node->left == 0) {
			if (node->parent->right == node) {
				node->parent->right = 0;
			}
			else {
				node->parent->left = 0;
			}
		}
		else {
			if (node->parent->right == node) {
				node->parent->right = node->left;
			}
			else {
				node->parent->left = node->left;
			}
			node->left->parent = node->parent;
		}
		delete node;
		return;
	}

	/* else */
	if (node->left == 0) {
			if (node->parent->right == node) {
				node->parent->right = node->right;
			}
			else {
				node->parent->left = node->right;
			}
			node->right->parent = node->parent;
			delete node;
			return;
	}

	/* else */
	BST<T>::node_s* rep = minNode(node->right);
	if (rep->right) {
		rep->parent->left = rep->right;
		rep->right->parent = rep->parent;
	}
	if (node->parent->right == node) {
		node->parent->right = rep;
	}
	else {
		node->parent->left = rep;
	}
	rep->right = node->right;
	rep->left = node->left;
	rep->parent = node->parent;

	delete node;
}

template<class T>
BST<T>::node_s* BST<T>::searchNode(const T& key) {
	BST<T>::node_s* ptr = this->_root;
	while (ptr) {
		if (ptr->key >= key) {
			if (ptr->key == key)
				return ptr;
			ptr = ptr->left;
		}
		else {
			ptr = ptr->right;
		}
	}
	return 0;// at this point ptr would have to be == 0
}

#endif