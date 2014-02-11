#include "types.h"
#include "graph.h"
#include "node.h"

#include <cstdio>
#include <cstdlib>

int read_next_int(FILE *file, bool require_valid_node_state);
void skip_white_space(FILE *file);

void graph::visit(node *n) 
{ 
	open_count--;
}

void graph::unvisit(node *n)
{ 
	open_count++;
}

void graph::load(FILE* mapFile, bool loadExpectedResult)
{
	if(mapFile)
	{
		this->width = read_next_int(mapFile, false);
		if(this->width <= 0)
		{
			printf("invalid width\n");
			exit(1);
		}

		this->height = read_next_int(mapFile, false);
		if(this->height <= 0)
		{
			printf("invalid height\n");
			exit(1);
		}

		nodeMap = new node*[this->height];

		for(int height = 0; height < this->height; height++)
		{
			nodeMap[height] = new node[this->width];
		}

		for(int y = 0; y < this->height; y++)
		{
			for(int x = 0; x < this->width; x++)
			{
				int next = read_next_int(mapFile, true);
				if(next == -1)
				{
					printf("Input file format was incorrect");
					exit(1);
				}

				node_state state = (node_state)next;

				nodeMap[y][x].set(this, x, y,
					(y > 0) ? &nodeMap[y-1][x] : NULL,
					(y < this->height-1) ? &nodeMap[y+1][x] : NULL,
					(x < this->width-1) ? &nodeMap[y][x+1] : NULL,
					(x > 0) ? &nodeMap[y][x-1] : NULL,
					state);

				if(state == start)
				{
					if(this->start_node != NULL)
					{
						printf("graph has multiple start points\n");
						exit(1);
					}

					this->start_node = &nodeMap[y][x];
				}

				else if(state == end)
				{
					if(this->end_node != NULL)
					{
						printf("graph has multiple end points\n");
						exit(1);
					}

					this->end_node = &nodeMap[y][x];
				}
			}
		}

		if(loadExpectedResult)
		{
			this->expected = read_next_int(mapFile, false);
		}

		load_graph_defaults();

		// find the best way to run the graph.
		if(!this->start_node->is_edge())
		{
			if(this->end_node->is_edge())
			{
				// if the starting point is an edge node we can use edge detection
				// so if start is not a wall but end is ... swap the direction we run this in.
				node *temp = this->start_node;
				this->start_node = this->end_node;
				this->end_node = temp;

				this->start_node->state = start;
				this->end_node->state = end;
			}
		}


		this->detect_divider_lines = (this->start_node->is_edge() || this->end_node->is_edge());
	}
}


void graph::load(const char *map)
{
	this->winners = 0;

	FILE* mapFile = fopen(map, "r");
	if(mapFile)
	{
		load(mapFile, true);
		fclose(mapFile);
	}
	else
	{
		printf("file not found (errno %d): %s\n", errno, map);
		exit(1);
	}
}

void graph::load_graph_defaults()
{
	for(int h = 0; h < this->height; h++)
	{
		for(int w = 0; w < this->width; w++)
		{
			if(nodeMap[h][w].state != wall)
			{
				this->open_count++;
			}
		}
	}
}

bool graph::is_valid()
{
	for(int h = 0; h < this->height; h++)
	{
		for(int w = 0; w < this->width; w++)
		{
			if(nodeMap[h][w].is_open())
			{
				int open_count = nodeMap[h][w].get_open_neighbor_count();

				// there is a node that is blocked in like so:
				// 1 1 1
				// 1 0 1
				// 1 1 1
				if(open_count == 0)
				{
					return false;
				}

				// there is a non-terminal node that will be blocked when visited:
				// 1 1 1
				// 1 0 0
				// 1 1 1
				if(open_count == 1 && nodeMap[h][w].state == open)
				{
					return false;
				}
			}
		}
	}

	return true;
}

int read_next_int(FILE *file, bool require_valid_node_state)
{
	int buffer;

	skip_white_space(file);
	int read = fscanf(file, "%d", &buffer);

	if(read != 1)
	{
		return -1;
	}

	if(require_valid_node_state)
	{
		switch(buffer)
		{
		case start:
		case end:
		case wall:
		case open:
			break;
		default:
			printf("value must be valid node state - was %d", buffer);
			exit(1);
			break;
		};
	}

	return buffer;
}

void skip_white_space(FILE *file)
{
	int found = fscanf(file, "%*[ \n\t]");
}