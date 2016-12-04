#ifndef GUARD_COMPRESS_H
#define GUARD_COMPRESS_H

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

class CTree {
public:
	struct node_s {
		node_s* right;
		node_s* left;
		char c;
		double f;
		bool isLeaf;
	};
	CTree() { _root = 0; }
	CTree(std::vector<char>& data);
	CTree(const CTree& old_t);
	CTree& operator=(const CTree& old_t);
	std::vector<bool> get_code(const char c);

	char get_char(std::vector<bool> seq);
	
	node_s* get_root();

	~CTree();
private:
	void delete_tree(node_s* node);
	node_s* copy_tree(node_s* source);
	bool get_code_internal(const char c,
						   CTree::node_s* node,
						   std::vector<bool>& ret);
	std::map<char, unsigned long long> get_frequencies(std::vector<char>& data);
	std::vector<std::pair<unsigned long long, char> > sort_frequencies(std::map<char, unsigned long long>& freq);
	node_s* build_tree(std::vector<std::pair<unsigned long long, char>>& freq);
	std::map<char, std::vector<bool>> build_dict(node_s* node);
	void build_dict_recrrsive(node_s* node, std::map<char, std::vector<bool>>& dict, std::vector<bool>& code);
	std::map<std::vector<bool>, char> build_rdict(std::map<char, std::vector<bool>>& dict);

	node_s* _root;
	std::map<char, std::vector<bool>> _dict;
	std::map<std::vector<bool>, char> _rdict;
};

std::ifstream&
get_data_from_file(std::ifstream& is,
				   std::vector<char>& data);
std::ofstream&
wrtie_data_to_file(std::ofstream& os,
				   std::vector<char>& data);
std::ifstream&
read_compressed_data_from_file(std::ifstream& is,
							   std::vector<bool>& compressed_data);
std::ofstream&
write_compressed_data_to_file(std::ofstream& os,
							  std::vector<bool>& compressed_data);

void
compress(std::vector<char>& data,
		 std::vector<bool>& compressed_data,
		 CTree& tree);

void
decompress(std::vector<char>& dest,
		   std::vector<bool>& compressed_data,
		   CTree& t);
#endif