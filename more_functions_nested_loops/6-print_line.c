#include "main.h"

/**
 * print_line - Draws a straight line using underscore characters
 * @n: Number of underscore characters to print
 *
 * Return: Nothing
 */
void print_line(int n)
{
	int count;

	for (count = 0; count < n; count++)
		_putchar('_');

	_putchar('\n');
}
