#include "main.h"

/**
 * _strncat - concatenates two strings using at most n bytes from src
 * @dest: destination string
 * @src: source string
 * @n: maximum number of bytes to use from src
 *
 * Return: pointer to dest
 */
char *_strncat(char *dest, char *src, int n)
{
	int dest_length;
	int i;

	dest_length = 0;
	while (dest[dest_length] != '\0')
		dest_length++;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[dest_length + i] = src[i];
		i++;
	}

	dest[dest_length + i] = '\0';

	return (dest);
}
