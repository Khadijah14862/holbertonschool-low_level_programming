#include "main.h"
#include <stdio.h>

/**
 * print_diagsums - Prints the sums of the two diagonals
 * @a: Pointer to the square matrix
 * @size: Size of the matrix
 */
void print_diagsums(int *a, int size)
{
	int i;
	int first_sum;
	int second_sum;

	first_sum = 0;
	second_sum = 0;

	for (i = 0; i < size; i++)
	{
		first_sum += a[(i * size) + i];
		second_sum += a[(i * size) + (size - i - 1)];
	}

	printf("%d, %d\n", first_sum, second_sum);
}
