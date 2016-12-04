#ifndef GUARD_BINARY_SEARCH_TREE_H
#define GUARD_BINARY_SEARCH_TREE_H

#include "list.h"
// this DOES NOT WORK for reasons unknown to me
// for the woring version see new_bst.h
template<class T>
class BST {
public:
	BST();
	~BST();
	//TODO: add other constructors
	void insert(const T& key);
	void remove(const T& key);
	bool search(const T& key);
	
	T max() const;//these do not check if the tree is empty
	T min() const;

	bool isEmpty() const;
private:
	struct node_s;
	
	void insertNode(node_s* new_node);
	void removeNode(node_s* old_node);
	node_s* findNode(const T& key);//returns closest node, if no such node present
								   //if there is such node returns first one	
	node_s* findMax(node_s* node) const;
	node_s* findMin(node_s* node) const;
	
	void deleteTree(node_s* node);

	node_s* _root;
};

template<class T>
struct BST<T>::node_s {
	BST<T>::node_s* right;
	BST<T>::node_s* left;
	BST<T>::node_s* parent;
	T key;
};

template<class T>
BST<T>::BST(): _root(0) {}

template<class T>
BST<T>::~BST() {
	deleteTree(this->_root);
}
template<class T>
void BST<T>::insert(const T& key) {
	BST<T>::node_s* new_node = new BST<T>::node_s;
	new_node->right = new_node->left = new_node->parent = 0;
	new_node->key = key;
	insertNode(new_node);
}

template<class T>
void BST<T>::remove(const T& key) {
	BST<T>::node_s* old_node = findNode(key);
	removeNode(old_node);
}

template<class T>
bool BST<T>::search(const T& key) {
	BST<T>::node_s* node = findNode(key);
	return node->key == key;
}

template<class T>
T BST<T>::max() const {
	BST<T>::node_s* node = findMax(this->_root);
	return node->key;
}

template<class T>
T BST<T>::min() const {
	BST<T>::node_s* node = findMin(this->_root);
	return node->key;
}

template<class T>
bool BST<T>::isEmpty() const {
	return this->_root;
}

template<class T>
void BST<T>::insertNode(BST<T>::node_s* new_node) {
	//if _root is null
	if (this->_root == 0) {
		this->_root = new_node;
	}

	//find where to insert
	BST<T>::node_s* target = this->_root;
	while (true) {
		if (new_node->key <= target->key) {
			if (target->left) {
				target = target->left;
			}
			else {
				target->left = new_node;
				break;
			}
		}
		else {
			if (target->right) {
				target = target->right;
			}
			else {
				target->right = new_node;
				break;
			}
		}
	}

	new_node->parent = target;
	new_node->right = new_node->left = 0;
}

template<class T>
void BST<T>::removeNode(BST<T>::node_s* old_node) {
	//Do not call this on an empty sub-tree
	
	//if old node has no right child (or no children at all)
	if (old_node->right == 0) {
		BST<T>::node_s* to_delete = old_node;
		if (old_node->left) {
			old_node->left->parent = old_node->parent;
		}
		old_node = old_node->left;
		delete to_delete;
		return;
	}
	/* else */
	//if old_node has no left child (but does have a right child)
	if (old->left == 0) {
		BST<T>::node_s* to_delete = old_node;
		old_node->right->parent = old_node->parent;
		old_node = old_node->left;
		delete to_delete;
		return;
	}
	/* else */
	//if old_node has both children
	BST<T>::node_s* rep = findMin(old_node->right);
	//if rep has a child it will be a right child
	if (rep == rep->parent->right) {
		rep->parent->right = rep->right;
	}
	else {
		rep->parent->left = rep->right;
	}
	rep->right = old_node->right;
	rep->left = old_node->left;
	rep->parent = old_node->parent;
	//need to change old_node's chidlren's parents
	old_node->left->parent = rep;
	if (old_node->right) {//if rep is the old_node's right child
		old_node->right->parent = rep;
	}
	BST<T>::node_s* to_delete = old_node;
	old_node = rep;
	delete to_delete;
	return;
}

template<class T>
BST<T>::node_s* BST<T>::findNode(const T& key) {
	if (this->_root == 0) {
		return this->_root;
	}
	BST<T>::node_s* current = this->_root;
	while (true) {
		if (key <= current->key) {
			if (key == current->key) {
				return current;
			}
			if (current->left) {
				current = current->left;
			}
			else {
				break;
			}
		}
		else {
			if (current->right) {
				current = current->right;
			}
			else {
				break;
			}
		}
	}
	return current;
}

template<class T> 
BST<T>::node_s* BST<T>::findMax(BST<T>::node_s* node) const {
	while (node->right) {
		node = node->right;
	}
	return node;
}

template<class T>
BST<T>::node_s* BST<T>::findMin(BST<T>::node_s* node) const {
	while (node->left) {
		node = node->left;
	}
	return node;
}

template<class T>
void BST<T>::deleteTree(node_s* node) {
	//might want to derecurrsify this
	deleteTree(node->right);
	deleteTree(node->left);
	if (node) {
		delete node;
	}
}

#endif