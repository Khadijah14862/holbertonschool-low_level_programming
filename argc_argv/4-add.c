#include <stdio.h>
#include <stdlib.h>

/**
 * main - Adds positive numbers
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 *
 * Return: 0 on success, or 1 if an argument is invalid
 */
int main(int argc, char *argv[])
{
	int sum;
	int i;
	int j;

	sum = 0;
	i = 1;

	while (i < argc)
	{
		j = 0;

		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error\n");
				return (1);
			}

			j++;
		}

		sum += atoi(argv[i]);
		i++;
	}

	printf("%d\n", sum);

	return (0);
}
