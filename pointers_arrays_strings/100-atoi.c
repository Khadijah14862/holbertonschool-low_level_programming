#include "main.h"

/**
 * _atoi - converts the first number in a string to an integer
 * @s: string to examine
 *
 * Return: converted integer, or 0 if no number is found
 */
int _atoi(char *s)
{
	int i;
	int sign;
	int found;
	unsigned int number;

	i = 0;
	sign = 1;
	found = 0;
	number = 0;

	while (s[i] != '\0')
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			found = 1;

			while (s[i] >= '0' && s[i] <= '9')
			{
				number = (number * 10) + (s[i] - '0');
				i++;
			}

			break;
		}

		i++;
	}

	if (found == 0)
		return (0);

	return (number * sign);
}
