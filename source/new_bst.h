#ifndef GUARD_BST_H
#define GUARD_BST_H

struct node_s {
	node_s* right;
	node_s* left;
	node_s* parent;
	int key;
};

class BST{
public:
	BST(): _root(0) {}
	~BST() {
		deleteTree(this->_root);
	}

	void insert(const int& key) {
		node_s* new_node = new node_s;
		new_node->right = new_node->left = new_node->parent = 0;
		new_node->key = key;
		insertNode(new_node);
	}
	void remove(const int& key) {
		node_s* old_node = findNode(key);
		if (old_node && old_node->key == key) 
			removeNode(old_node);

	}
	bool search(const int& key) {
		node_s* node = findNode(key);
		if (node && node->key == key)
			return true;
		return false;
	}
	
	int max() const {
		node_s* node = findMax(this->_root);
		return node->key;
	}//these do not check if the tree is empty
	int min() const {
		node_s* node = findMin(this->_root);
		return node->key;
	}

	bool isEmpty() const {
		return this->_root == 0;
	}
private:
	void insertNode(node_s* new_node) {
		//if _root is null
		if (this->_root == 0) {
			this->_root = new_node;
		}

		//find where to insert
		node_s* target = this->_root;
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
	void removeNode(node_s* old_node) {
		//Do not call this on an empty sub-tree
		
		//if old node has no right child (or no children at all)
		if (old_node->right == 0) {
			node_s* to_delete = old_node;
			if (old_node->left) {
				old_node->left->parent = old_node->parent;
			}
			if (old_node == this->_root)
				this->_root = old_node->left;
			old_node = old_node->left;
			delete to_delete;
			return;
		}
		/* else */
		//if old_node has no left child (but does have a right child)
		if (old_node->left == 0) {
			node_s* to_delete = old_node;
			old_node->right->parent = old_node->parent;
			if (old_node == this->_root)
				this->_root = old_node->right;
			old_node = old_node->right;
			delete to_delete;
			return;
		}
		/* else */
		//if old_node has both children
		node_s* rep = findMin(old_node->right);
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
		node_s* to_delete = old_node;
		if (old_node == this->_root)
			this->_root = rep;
		old_node = rep;
		delete to_delete;
		return;
	}

	node_s* findNode(const int& key) {
		if (this->_root == 0) {
			return this->_root;
		}
		node_s* current = this->_root;
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
	}//returns closest node, if no such node present
	node_s* findMax(node_s* node) const {
		while (node->right) {
			node = node->right;
		}
		return node;
	}
	node_s* findMin(node_s* node) const {
		while (node->left) {
			node = node->left;
		}
		return node;
	}

	void deleteTree(node_s* node) {
		//might want to derecurrsify this
		if (node) {
			deleteTree(node->right);
			deleteTree(node->left);
			delete node;
		}
	}

	node_s* _root;
};

#endif