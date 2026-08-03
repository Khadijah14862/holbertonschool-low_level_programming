#include "main.h"

/**
 * _strspn - Gets the length of a prefix substring
 * @s: String to examine
 * @accept: String containing accepted characters
 *
 * Return: Number of bytes in the initial segment of s
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int i;
	unsigned int j;

	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = 0; accept[j] != '\0'; j++)
		{
			if (s[i] == accept[j])
				break;
		}

		if (accept[j] == '\0')
			break;
	}

	return (i);
}
