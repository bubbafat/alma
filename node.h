#ifndef HAMBONE_NODE_H
#define HAMBONE_NODE_H

#include "types.h"
#include "graph.h"

struct graph;

struct node {
	int x;
	int y;
	bool visited;
	node *north, *south, *east, *west;
	node_state state;
	graph *parent;

	void set(graph *g, int xloc, int yloc, node *n, node *s, node *e, node *w, node_state ns);

	int get_neighbor_count(node *node);

	int get_open_neighbor_count();

	int is_open()
	{
		return (visited || state == wall) ? 0 : 1;
	}

	void visit();

	bool is_edge();

private:
	void go(node *neighbor, int open_neighbor_count);
};

#endif