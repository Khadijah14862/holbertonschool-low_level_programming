#include "main.h"

/**
 * _strstr - Locates a substring
 * @haystack: String to search
 * @needle: Substring to locate
 *
 * Return: Pointer to the beginning of needle, or NULL
 */
char *_strstr(char *haystack, char *needle)
{
	int i;
	int j;

	if (needle[0] == '\0')
		return (haystack);

	for (i = 0; haystack[i] != '\0'; i++)
	{
		j = 0;

		while (needle[j] != '\0' &&
		       haystack[i + j] == needle[j])
			j++;

		if (needle[j] == '\0')
			return (haystack + i);
	}

	return (0);
}
