#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "linked_list.h"

#ifndef A_STAR_H
#define A_STAR_H

typedef enum{
	EMPTY,WALL,PATH,GOAL='X'
} Cells;

typedef enum{
	UP,DOWN,LEFT,RIGHT
} Direction;

typedef struct Node {
	uint32_t x;
	uint32_t y;
	struct Node* parent;
	float f_cost;
	float g_cost;
	float h_cost;
}Node;

typedef struct Point {
	uint32_t x;
	uint32_t y;
} Point;

static inline float f_cost(LLNode* node)
{
	assert(node != NULL);
	Node* n = (Node*)node->element;
	return n->g_cost + n->h_cost;
}

static void print_node(Node* node)
{
	int parent_x,parent_y;

	if(node->parent != NULL)
	{
		Node* parent = (Node*)node->parent;
		parent_x = parent->x;
		parent_y = parent->y;
	}else
	{
		parent_x = -1;
		parent_y = -1;
	}

	printf("Node(%d/%d) Parent:(%d/%d) Cost:%.2f,%.2f,%.2f\n", node->x, node->y, parent_x, parent_y, node->f_cost, node->g_cost, node->h_cost);
}

static void print_node_list(LLNode* list)
{
	list = LL_first(list);

	printf("Node list: {\n");

	while(list != NULL){
		if(list->element != NULL)
		{
			printf("\t");
			print_node((Node*)list->element);
		}
		list = LL_next(list);
	}

	printf("}\n");
}

// Some quick and dirty list sorting, doing bubble sort 
void sort_node_list(LLNode* ll)
{
	bool swap = false;

	LLNode* ll_first = LL_first(ll);

	while (true)
	{
		ll = ll_first;

		while(LL_next(ll) != NULL){
			if(f_cost(ll) > f_cost(LL_next(ll)))
			{
				swap = true;
				ll = LL_swap_next(ll);
			}

			// Additional check if element swapped to the end which would result in a segfault 
			if(LL_next(ll)!= NULL)
			{
				ll = LL_next(ll);
			}
		}

		if(!swap)
		{
			break;
		}
		swap = false;
	}
}

bool point_in_list(LLNode* list, Point point)
{
	list = LL_first(list);

    while(list != NULL)
    {
		Node* node = (Node*)list->element;
        if(node->x == point.x && node->y == point.y)
        {
            return true;
        }
        list = LL_next(list);
    }
    return false;
}

static Node* create_node(int x, int y, Node* parent, float g_cost, float h_cost)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->x = x;
	node->y = y;
	node->parent = parent;
	node->g_cost = g_cost;
	node->h_cost = h_cost;
	node->f_cost = g_cost + h_cost;

	return node;
}

__always_inline float distance(Point a, Point b)
{
	return hypot(abs((int)a.x-(int)b.x), abs((int)a.y-(int)b.y));
}

LLNode* shortest_path(int** map, int width, int height, Point start, Point end)
{
	assert(map[start.x][start.y] == EMPTY);
	assert(map[end.x][end.y] == GOAL);

	LLNode* open = NULL;
	LLNode* closed = NULL;

	LLNode* path = NULL;

	Node* current = create_node(start.x, start.y, NULL, INFINITY, INFINITY);
	open = LL_create(current);

	while(open != NULL)
	{
		
		sort_node_list(open);
		open = LL_first(open);
		current = open->element;

		/*printf("\n############################################################################\n");
		printf("\ncurrent: ");
		print_node(current);
		printf("\nopen: ");
		print_node_list(open);
		printf("\nclosed: ");
		print_node_list(closed);*/

		closed = LL_append(closed, (void*)current);
		open = LL_remove(open);

		// generate path when goal is reached using backtracing of parent nodes
		if(map[current->x][current->y] == GOAL)
		{
			Point* step = malloc(sizeof(Point));
			step->x = current->x;
			step->y = current->y;
			path = LL_create(step); 

			while(current != NULL)
			{
				step = malloc(sizeof(Point));
				step->x = current->x;
				step->y = current->y;
				path = LL_append(path, step);

				current = current->parent;
			}

			break;
		}

		if(current->x> 0)
		{
			Point position = {current->x-1, current->y};
			if(map[position.x][position.y] != WALL && !point_in_list(closed, position))
			{
				Node* new_node = create_node(
					position.x,
					position.y,
					current,
					distance(position, start),
					distance(position, end)
				);

				open = LL_append(open, new_node);
			}
		}

		if((current->x + 1) <= width)
		{
			Point position = {current->x+1, current->y};
			if(map[position.x][position.y] != WALL && !point_in_list(closed, position))
			{
				Node* new_node = create_node(
					position.x,
					position.y,
					current,
					distance(position, start),
					distance(position, end)
				);

				open = LL_append(open, new_node);
			}
		}

		if(current->y > 0)
		{
			Point position = {current->x, current->y-1};
			if(map[position.x][position.y] != WALL && !point_in_list(closed, position))
			{
				Node* new_node = create_node(
					position.x,
					position.y,
					current,
					distance(position, start),
					distance(position, end)
				);

				open = LL_append(open, new_node);
			}
		}

		if((current->y+1) <= height)
		{
			Point position = {current->x, current->y+1};
			if(map[position.x][position.y] != WALL && !point_in_list(closed, position))
			{
				Node* new_node = create_node(
					position.x,
					position.y,
					current,
					distance(position, start),
					distance(position, end)
				);

				open = LL_append(open, new_node);
			}
		}
	}

	if(open != NULL){
		LL_delete(open);
	}

	if(closed != NULL){
		LL_delete(closed);
	}

	return path;
}


#endif
