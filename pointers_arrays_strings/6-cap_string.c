#include "main.h"

/**
 * is_separator - checks whether a character separates words
 * @c: character to check
 *
 * Return: 1 if separator, otherwise 0
 */
int is_separator(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' ||
	    c == ',' || c == ';' || c == '.' ||
	    c == '!' || c == '?' || c == '"' ||
	    c == '(' || c == ')' || c == '{' ||
	    c == '}')
		return (1);

	return (0);
}

/**
 * cap_string - capitalizes all words of a string
 * @str: string to modify
 *
 * Return: pointer to str
 */
char *cap_string(char *str)
{
	int i;
	int capitalize;

	i = 0;
	capitalize = 1;

	while (str[i] != '\0')
	{
		if (capitalize && str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - ('a' - 'A');

		if (is_separator(str[i]))
			capitalize = 1;
		else
			capitalize = 0;

		i++;
	}

	return (str);
}
