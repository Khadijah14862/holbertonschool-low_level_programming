#include "main.h"

/**
 * print_last_digit - prints the last digit of a number
 * @n: integer to check
 *
 * Return: the value of the last digit
 */
int print_last_digit(int n)
{
	int digit;

	digit = n % 10;

	if (digit < 0)
	{
		digit = -digit;
	}

	_putchar(digit + '0');

	return (digit);
}
