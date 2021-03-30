#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>

#include "linked_list.h"
#include "AStar.h"

bool file_next_line(FILE* fp, uint32_t max)
{
	char character;
	uint32_t traversed_positions = 0;

	while(true)
	{
		character = fgetc(fp);
		if(character == '\n')
		{
			//printf("Newline found!\n");
			return true;
		}
		else if(traversed_positions >= max)
		{
			//printf("Max skips reached!\n");
			return false;
		}
		++traversed_positions;
	}
	
}

int** init_map(uint32_t width, uint32_t height)
{
	int** map = (int**)malloc(sizeof(int*) * width);

	for(uint32_t x_idx = 0; x_idx < width; x_idx++)
	{
		map[x_idx] = (int*)malloc(sizeof(int) * height);
	}

	return map;
}

void delete_map(int** map, uint32_t width)
{
	for(uint32_t x_idx = 0; x_idx < width; x_idx++)
	{
		free(map[x_idx]);
	}
	free(map);
}

void print_map(int** map, uint32_t width, uint32_t height)
{
	for(uint32_t x_idx = 0; x_idx < width; x_idx++)
	{
		for(uint32_t y_idx = 0; y_idx < height; y_idx++)
		{
			if(map[x_idx][y_idx] == 'X')
			{
				printf("X");
			}else if(map[x_idx][y_idx] == WALL)
			{
				printf("\e[1;34m█\e[0m");
			}else if(map[x_idx][y_idx] == PATH)
			{
				printf("\e[1;32m█\e[0m");
			}else if(map[x_idx][y_idx] == EMPTY)
			{
				printf(" ");
			}else
			{
				printf("%d",map[x_idx][y_idx]);
			}
		}
		printf("\n");
	}
}

int main()
{
	FILE* fp = fopen("Maze.txt", "r");

	int width, height;
	fscanf(fp,"%d %d", &width,&height);

	//printf("%d %d\n", width, height);
	if(!file_next_line(fp, 512))
	{
		printf("No map\n");
		return 1;
	}

	int** map = init_map(width, height);
	Point end = {};

	for(uint32_t x_idx = 0; x_idx < width; x_idx++)
	{
		for(uint32_t y_idx = 0; y_idx < height; y_idx++)
		{
			char cell = fgetc(fp);

			if(cell == 'X')
			{
				end.x = x_idx;
				end.y = y_idx;
				map[x_idx][y_idx] = (int)'X';
			}else
			{
				map[x_idx][y_idx] = (int)cell - 48;
			}

			//printf("%d", map[x_idx][y_idx]);
		}
		//printf("\n");
		file_next_line(fp, 512);
	}

	fclose(fp);

	print_map(map, width, height);

	Point start = {1,1};

	LLNode* path = shortest_path(map, width, height, start, end);

	if(path == NULL)
	{
		printf("\e[1;31mNo path available!\e[0m");
	}else
	{

		while(true)
		{
			Point* step = (Point*)path->element;
			map[step->x][step->y] = PATH;

			if(path->previous != NULL)
			{
				path = path->previous;
			}else
			{
				break;
			}
		}

		printf("\n");
		print_map(map, width, height);
		LL_delete(path);
	}

	
	delete_map(map, width);
}
