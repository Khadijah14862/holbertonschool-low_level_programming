#include "function_pointers.h"

/**
 * int_index - Searches for an integer
 * @array: Array to search
 * @size: Number of elements in the array
 * @cmp: Function used to compare values
 *
 * Return: Index of the first match, or -1
 */
int int_index(int *array, int size, int (*cmp)(int))
{
	int i;

	if (array == 0 || cmp == 0 || size <= 0)
		return (-1);

	for (i = 0; i < size; i++)
	{
		if (cmp(array[i]) != 0)
			return (i);
	}

	return (-1);
}
