#ifndef HAMBONE_GRAPH_H
#define HAMBONE_GRAPH_H

#include <cstdio>

#include "types.h"
#include "node.h"

struct node;

struct graph {
	int width;
	int height;

	int open_count;

	node *start_node;
	node *end_node;

	node **nodeMap;

	bool detect_divider_lines;

	void load(const char *map);
	void load(FILE* mapFile, bool loadExpectedResult);
	void visit(node *n);
	void unvisit(node *n);

	int winners;
	int expected;

	bool is_valid();

private:
	void load_graph_defaults();
	bool any_blocked_children();
};


#endif