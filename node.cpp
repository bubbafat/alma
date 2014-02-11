#include "node.h"
#include <cstdio>

const bool use_optimizations = true;

void node::visit()
{
	if(is_open())
	{
		if(state == end)
		{
			if(parent->open_count == 1)
			{
				parent->winners++;
				return;
			}

			// not a valid path
			return;
		}

		// everything after this depends on the visited state to be correct
		this->visited = true;
		parent->visit(this);

		int adjacentNodesWithOnePath = 0;

		bool goNorth = false;
		bool goSouth = false;
		bool goEast = false;
		bool goWest = false;

		// do this up front so that we don't repeat ourselves later on
		int north_open_count = get_neighbor_count(north);
		int south_open_count = get_neighbor_count(south);
		int east_open_count = get_neighbor_count(east);
		int west_open_count = get_neighbor_count(west);

		if(use_optimizations)
		{
			// the *->state == open doesn't need to check for visited, wall or null because
			// get_neighbor_count already did those checks.

			if(north_open_count == 1 && north->state == open)
			{
				adjacentNodesWithOnePath++;
				goNorth = true;
			}

			if(south_open_count == 1 && south->state == open)
			{
				adjacentNodesWithOnePath++;
				goSouth = true;
			}

			if(east_open_count == 1 && east->state == open)
			{
				adjacentNodesWithOnePath++;
				goEast = true;
			}

			if(west_open_count == 1 && west->state == open)
			{
				adjacentNodesWithOnePath++;
				goWest = true;
			}
		}

		if(adjacentNodesWithOnePath == 0)
		{
			go(north, north_open_count);
			go(south, south_open_count);
			go(east, east_open_count);
			go(west, west_open_count);
		}
		else if(adjacentNodesWithOnePath == 1)
		{
			if(goNorth) { go(north, north_open_count); }
			if(goSouth) { go(south, south_open_count); }
			if(goEast) { go(east, east_open_count); }
			if(goWest) { go(west, west_open_count); }
		}
		else
		{
			// if there is more than one adjacent node with one path
			// there is no way to resolve the current graph state.
		}

		this->visited = false;
		parent->unvisit(this);
	}
}

bool node::is_edge()
{
	return this->x == 0 || 
		this->y == 0 ||
		this->x == parent->width-1 ||
		this->y == parent->height-1;
}

void node::set(graph *g, int xloc, int yloc, node *n, node *s, node *e, node *w, node_state ns)
{
	x = xloc;
	y = yloc;
	north = n;
	south = s;
	east = e;
	west = w;
	state = ns;
	visited = false;
	parent = g;
}

int node::get_neighbor_count(node *node)
{
	if(node && node->is_open())
	{
		return node->get_open_neighbor_count();
	}

	return 0;
}


int node::get_open_neighbor_count()
{
	int count = 0;

	count += north ? north->is_open() : 0;
	count += south ? south->is_open() : 0;
	count += east ? east->is_open() : 0;
	count += west ? west->is_open() : 0;

	return count;
}


void node::go(node *neighbor, int open_neighbor_count)
{
	if(neighbor)
	{
		if(use_optimizations)
		{
			if(open_neighbor_count > 1)
			{
    			// if we are moving to a wall and the wall has more than one open neighbor
    			// and our starting point was a wall then we've just split the board.
    			if(parent->detect_divider_lines && neighbor->is_edge() && !this->is_edge())
    			{
    				return;
    			}
			}
		}

		neighbor->visit();
	}
}
