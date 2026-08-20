#include "main.h"

/**
 * check_prime - checks if a number is prime
 * @n: number to check
 * @i: possible divisor
 *
 * Return: 1 if prime, otherwise 0
 */
int check_prime(int n, int i)
{
	if (i > n / i)
		return (1);

	if (n % i == 0)
		return (0);

	return (check_prime(n, i + 1));
}

/**
 * is_prime_number - checks if an integer is a prime number
 * @n: number to check
 *
 * Return: 1 if n is prime, otherwise 0
 */
int is_prime_number(int n)
{
	if (n <= 1)
		return (0);

	return (check_prime(n, 2));
}
