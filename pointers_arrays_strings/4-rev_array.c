#include "main.h"

/**
 * reverse_array - reverses the content of an integer array
 * @a: array of integers
 * @n: number of elements in the array
 *
 * Return: Nothing
 */
void reverse_array(int *a, int n)
{
	int i;
	int temp;

	for (i = 0; i < n / 2; i++)
	{
		temp = a[i];
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}
}
