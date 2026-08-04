#include "function_pointers.h"

/**
 * array_iterator - Executes a function on each array element
 * @array: Array of integers
 * @size: Number of elements in the array
 * @action: Function to execute on each element
 */
void array_iterator(int *array, size_t size, void (*action)(int))
{
	size_t i;

	if (array == 0 || action == 0)
		return;

	for (i = 0; i < size; i++)
		action(array[i]);
}
