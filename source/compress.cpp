#include <math.h>
#include <algorithm>
#include <map>

#include "stack.h"
#include "heap.h"

#include "compress.h"

CTree::CTree(std::vector<char>& data) {
	this->_root = build_tree(sort_frequencies(get_frequencies(data)));
	this->_dict = build_dict(_root);
	this->_rdict = build_rdict(_dict);
}

CTree::CTree(const CTree & old_t) {
	if (old_t._root == 0) {
		this->_root = 0;
		return;
	}
	_root = copy_tree(old_t._root);
	_dict = build_dict(_root);
	_rdict = build_rdict(_dict);
}

CTree & CTree::operator=(const CTree & old_t) {
	delete_tree(this->_root);
	if (old_t._root == 0) {
		this->_root = 0;
		return *this;
	}
	
	_root = copy_tree(old_t._root);
	_dict = build_dict(_root);
	_rdict = build_rdict(_dict);
	return *this;
}

std::vector<bool> CTree::get_code(const char c) {
	return _dict[c];
}

char CTree::get_char(std::vector<bool> seq) {
	/*
	unsigned i = 0;
	node_s* cur = _root;
	while (i < seq.size()) {
		if (seq[i]) {//go left
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}
	return cur->c;
	*/
	

	//if element exits
	std::map<std::vector<bool>, char>::iterator it = _rdict.find(seq);
	if (it != _rdict.end()) {
		return it->second;
	}
	
}

CTree::node_s* CTree::get_root() {
	return _root;
}

CTree::~CTree() {
	delete_tree(_root);
}

void CTree::delete_tree(node_s* node) {
		if (node) {
			delete_tree(node->right);
			delete_tree(node->left);
			delete node;
			node = 0;
		}
	}

CTree::node_s* CTree::copy_tree(node_s * source) {
	if (source) {
		node_s* new_node = new node_s;
		new_node->c = source->c;
		new_node->f = source->f;
		new_node->isLeaf = source->isLeaf;
		new_node->left = copy_tree(source->left);
		new_node->right = copy_tree(source->right);
		return new_node;
	}
	else {
		return 0;
	}
}
bool CTree::get_code_internal(const char c, 
						   node_s* node,
						   std::vector<bool>& ret) {
		if (node->isLeaf) {
			if (node->c == c) {
				return true;//return empty bit_vector
			}
			else {
				return false;
			}
		}
		else if (get_code_internal(c, node->right, ret)) {
			ret.insert(ret.begin(), 1);
			return true;
		}
		else if (get_code_internal(c, node->left, ret)) {
			ret.insert(ret.begin(), 0);
			return true;
		}
		else {
			return false;
		}
	}
std::map<char, unsigned long long> CTree::get_frequencies(std::vector<char>& data) {
		unsigned long long total_size = 0;
		std::map<char, unsigned long long> effectif;
		for (std::vector<char>::size_type i = 0;
			 i < data.size();
			 ++i) {
			++effectif[data[i]];
		//	++total_size;
		}
		/*std::map<char, double> frequency;
		for (unsigned long long i = 0;
			 i < effectif.size();
			 ++i) {
			frequency[data[i]] = double(effectif[data[i]]) / double(total_size);
		}*/
		//return frequency;
		return effectif;
	}

std::vector<std::pair<unsigned long long, char> > 
		CTree::sort_frequencies(std::map<char, unsigned long long>& freq) {
		std::vector<std::pair<unsigned long long, char> > ret;
		for (std::map<char, unsigned long long>::iterator it =
			 freq.begin();
			 it != freq.end();
			 ++it) {
			ret.push_back({it->second, it->first});
		}
		std::sort(ret.begin(), ret.end());
		return ret;
	}
CTree::node_s* CTree::build_tree(std::vector<std::pair<unsigned long long, char>>& freq) {
		MinHeap<node_s*, unsigned long long> h;
		for (std::vector<std::pair<unsigned long long, char>>::reverse_iterator 
			 i = freq.rbegin(); 
			 i != freq.rend();
			 ++i) {
			node_s* new_node = new node_s;
			new_node->c = i->second;
			new_node->f = i->first;
			new_node->right = new_node->left = 0;
			new_node->isLeaf = true;
			h.insert(new_node, new_node->f);
		}
		for (unsigned i = 0; i < freq.size() - 1; ++i) {
			node_s* new_node = new node_s;
			new_node->isLeaf = false;
			new_node->right = h.extractMin();
			new_node->left = h.extractMin();
			new_node->f = new_node->left->f + new_node->right->f;
			h.insert(new_node, new_node->f);
		}
		return h.extractMin();
	}

std::map<char, std::vector<bool>> CTree::build_dict(node_s * node) {
	std::map<char, std::vector<bool>> dict;
	std::vector<bool> code;
	build_dict_recrrsive(node, dict, code);
	return dict;
}

void 
CTree::build_dict_recrrsive(node_s * node,
								 std::map<char, std::vector<bool>>& dict,
								 std::vector<bool>& code) {
	if (node) {
		if (node->isLeaf) {
			dict[node->c] = code;
		}
		else {
			code.push_back(1);
			build_dict_recrrsive(node->left,
								 dict,
								 code);
			code.pop_back();
			code.push_back(0);
			build_dict_recrrsive(node->right,
								 dict,
								 code);
			code.pop_back();
		}
	}
}

std::map<std::vector<bool>, char> CTree::build_rdict(std::map<char, std::vector<bool>>& dict) {
	std::map<std::vector<bool>, char> rdict;

	for (std::map<char, std::vector<bool>>::iterator
		 i = dict.begin();
		 i != dict.end();
		 ++i) {
		rdict[i->second] = i->first;
	}
	return rdict;
}

std::ifstream& 
get_data_from_file(std::ifstream& is,
				   std::vector<char>& data) {
	data = std::vector<char>();
	if (is.is_open()) {
		char c;
		while (is.get(c)) {
			data.push_back(c);
		}
	}
	return is;
}

std::ofstream& 
wrtie_data_to_file(std::ofstream& os,
				   std::vector<char>& data) {
	if (os.is_open()) {
		for (std::vector<char>::iterator it = data.begin();
			 it != data.end();
			 ++it) {
			os.put(*it);
		}
	}
	return os;
}

std::ifstream&
read_compressed_data_from_file(std::ifstream& is,
							 std::vector<bool>& compressed_data) {
	compressed_data = std::vector<bool>();
	if (is.is_open()) {
		char c;
		while (is.get(c)) {
			if (c == '1') {
				compressed_data.push_back(1);
			}
			else {
				compressed_data.push_back(0);
			}
		}
	}
	return is;
}

std::ofstream&
write_compressed_data_to_file(std::ofstream& os,
							  std::vector<bool>& compressed_data) {
	if (os.is_open()) {
		for (std::vector<bool>::iterator it = compressed_data.begin();
			 it != compressed_data.end();
			 ++it) {
			if (*it) {
				os.put('1');
			}
			else {
				os.put('0');
			}
		}
	}
	return os;
}

void 
compress (std::vector<char>& data,
		 std::vector<bool>& compressed_data,
		 CTree& tree) {
	CTree t(data);
	compressed_data = std::vector<bool>();
	for (unsigned long long i = 0; i < data.size(); ++i) {
		std::vector<bool> code = t.get_code(data[i]);
		compressed_data.insert(compressed_data.end(), 
							   code.begin(),
							   code.end());
	}
	tree = t;
}							  

void
decompress(std::vector<char>& dest,
		   std::vector<bool>& compressed_data,
		   CTree& t) {
	dest = std::vector<char>();
	for (unsigned long long i = 0;
		 i < compressed_data.size();) {
		CTree::node_s* cur = t.get_root();
		while (!cur->isLeaf && i < compressed_data.size()) {
			if (compressed_data[i]) {
				cur = cur->right;
			}
			else {
				cur = cur->left;
			}
			++i;
		}
		dest.push_back(cur->c);
	}
}