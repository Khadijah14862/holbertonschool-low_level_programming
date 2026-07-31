#include "main.h"

/**
 * _strlen - Returns the length of a string
 * @s: String to measure
 *
 * Return: Length of the string
 */
int _strlen(char *s)
{
	int length;

	length = 0;

	while (s[length] != '\0')
		length++;

	return (length);
}
