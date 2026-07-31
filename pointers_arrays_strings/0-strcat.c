#include "main.h"

/**
 * _strcat - concatenates two strings
 * @dest: destination string
 * @src: source string
 *
 * Return: pointer to dest
 */
char *_strcat(char *dest, char *src)
{
	int dest_length;
	int i;

	dest_length = 0;
	while (dest[dest_length] != '\0')
		dest_length++;

	i = 0;
	while (src[i] != '\0')
	{
		dest[dest_length + i] = src[i];
		i++;
	}

	dest[dest_length + i] = '\0';

	return (dest);
}
