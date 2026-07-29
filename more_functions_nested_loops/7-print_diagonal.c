#include "main.h"

/**
 * print_diagonal - Draws a diagonal line using backslashes
 * @n: Number of backslashes to print
 *
 * Return: Nothing
 */
void print_diagonal(int n)
{
	int row;
	int space;

	if (n <= 0)
	{
		_putchar('\n');
		return;
	}

	for (row = 0; row < n; row++)
	{
		for (space = 0; space < row; space++)
			_putchar(' ');

		_putchar('\\');
		_putchar('\n');
	}
}
