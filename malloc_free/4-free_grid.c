#include "main.h"
#include <stdlib.h>

/**
 * free_grid - frees a two-dimensional grid
 * @grid: pointer to the grid
 * @height: number of rows
 *
 * Return: Nothing
 */
void free_grid(int **grid, int height)
{
	int row;

	if (grid == NULL)
		return;

	for (row = 0; row < height; row++)
		free(grid[row]);

	free(grid);
}
