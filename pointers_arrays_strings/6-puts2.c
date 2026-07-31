#include "main.h"

/**
 * puts2 - Prints every other character of a string
 * @str: String to print
 */
void puts2(char *str)
{
	int i;

	i = 0;

	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i += 2;
	}

	_putchar('\n');
}
