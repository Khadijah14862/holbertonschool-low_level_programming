#include "main.h"

/**
 * print_triangle - Prints a right-aligned triangle
 * @size: The size of the triangle
 *
 * Return: Nothing
 */
void print_triangle(int size)
{
	int row;
	int space;
	int hash;

	if (size <= 0)
	{
		_putchar('\n');
		return;
	}

	for (row = 1; row <= size; row++)
	{
		for (space = 0; space < size - row; space++)
			_putchar(' ');

		for (hash = 0; hash < row; hash++)
			_putchar('#');

		_putchar('\n');
	}
}
