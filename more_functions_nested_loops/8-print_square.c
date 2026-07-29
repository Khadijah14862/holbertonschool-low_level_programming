#include "main.h"

/**
 * print_square - Prints a square using the # character
 * @size: The size of the square
 *
 * Return: Nothing
 */
void print_square(int size)
{
	int row;
	int column;

	if (size <= 0)
	{
		_putchar('\n');
		return;
	}

	for (row = 0; row < size; row++)
	{
		for (column = 0; column < size; column++)
			_putchar('#');

		_putchar('\n');
	}
}
