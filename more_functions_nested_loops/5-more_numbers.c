#include "main.h"

/**
 * more_numbers - Prints numbers from 0 to 14 ten times
 *
 * Return: Nothing
 */
void more_numbers(void)
{
	int line;
	int number;

	for (line = 0; line < 10; line++)
	{
		for (number = 0; number <= 14; number++)
		{
			if (number >= 10)
				_putchar((number / 10) + '0');

			_putchar((number % 10) + '0');
		}
		_putchar('\n');
	}
}
