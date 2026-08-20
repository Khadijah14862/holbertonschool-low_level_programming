#include "main.h"

/**
 * find_sqrt - finds the natural square root of a number
 * @n: number to find the square root of
 * @i: possible square root
 *
 * Return: natural square root, or -1 if none exists
 */
int find_sqrt(int n, int i)
{
	if (i > n / i)
		return (-1);

	if (i * i == n)
		return (i);

	return (find_sqrt(n, i + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number
 * @n: number to find the square root of
 *
 * Return: natural square root, or -1 if none exists
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
		return (-1);

	if (n == 0)
		return (0);

	return (find_sqrt(n, 1));
}
