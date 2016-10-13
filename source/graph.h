#ifndef GUARD_GRAPH_H
#define GUARD_GRAPH_H

#include <iostream>
#include <map>
#include <vector>
#include <list>

struct Edge {
	int weight;
};
struct vertex {
	int name;

	bool mark;
	std::list<int> neighbors;
	std::list<Edge> edges;
};

struct Graph {
	std::vector<vertex> adj;

	void add_vertex(const vertex& v) {
		adj.push_back(v);
	}
	void print(std::ostream& os) {
		std::vector<char>
			alph{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
		std::map<int, char> char_table;

		for (const vertex& x : adj) {
			char_table[x.name] = alph.back();
			alph.pop_back();
		}
		os << "digraph graph{\n";
		for (const vertex& x : adj) {
			os << '\"' << char_table[x.name] << '\"';
			for (const int y : x.neighbors) {
				os << " -> " << '\"' << char_table[y] << '\"' ;
			}
			os << ";\n";
		}
		os << "}\n";
	}
};

#endif