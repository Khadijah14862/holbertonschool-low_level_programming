#include "main.h"
#include <stdlib.h>

/**
 * create_array - creates an array and initializes it with a character
 * @size: size of the array
 * @c: character used to initialize the array
 *
 * Return: pointer to the array, or NULL on failure
 */
char *create_array(unsigned int size, char c)
{
	char *array;
	unsigned int i;

	if (size == 0)
		return (NULL);

	array = malloc(sizeof(char) * size);

	if (array == NULL)
		return (NULL);

	for (i = 0; i < size; i++)
		array[i] = c;

	return (array);
}
