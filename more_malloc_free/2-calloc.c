#include "main.h"
#include <limits.h>
#include <stdlib.h>

/**
 * _calloc - Allocates memory for an array and initializes it to zero
 * @nmemb: Number of elements
 * @size: Size of each element
 *
 * Return: Pointer to allocated memory, or NULL on failure
 */
void *_calloc(unsigned int nmemb, unsigned int size)
{
	char *memory;
	unsigned int total;
	unsigned int i;

	if (nmemb == 0 || size == 0)
		return (NULL);

	if (nmemb > UINT_MAX / size)
		return (NULL);

	total = nmemb * size;
	memory = malloc(total);

	if (memory == NULL)
		return (NULL);

	for (i = 0; i < total; i++)
		memory[i] = 0;

	return (memory);
}
