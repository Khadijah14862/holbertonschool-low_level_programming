#include "main.h"
#include <stdlib.h>

/**
 * alloc_grid - allocates a two-dimensional array of integers
 * @width: number of columns
 * @height: number of rows
 *
 * Return: pointer to the grid, or NULL on failure
 */
int **alloc_grid(int width, int height)
{
	int **grid;
	int row;
	int column;
	int i;

	if (width <= 0 || height <= 0)
		return (NULL);

	grid = malloc(sizeof(int *) * height);
	if (grid == NULL)
		return (NULL);

	for (row = 0; row < height; row++)
	{
		grid[row] = malloc(sizeof(int) * width);

		if (grid[row] == NULL)
		{
			for (i = 0; i < row; i++)
				free(grid[i]);

			free(grid);
			return (NULL);
		}

		for (column = 0; column < width; column++)
			grid[row][column] = 0;
	}

	return (grid);
}
