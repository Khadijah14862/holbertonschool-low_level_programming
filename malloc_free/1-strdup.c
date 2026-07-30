#include "main.h"
#include <stdlib.h>

/**
 * _strdup - returns a copy of a string in newly allocated memory
 * @str: string to duplicate
 *
 * Return: pointer to the duplicated string, or NULL on failure
 */
char *_strdup(char *str)
{
	char *copy;
	unsigned int length;
	unsigned int i;

	if (str == NULL)
		return (NULL);

	length = 0;
	while (str[length] != '\0')
		length++;

	copy = malloc(sizeof(char) * (length + 1));
	if (copy == NULL)
		return (NULL);

	for (i = 0; i <= length; i++)
		copy[i] = str[i];

	return (copy);
}
