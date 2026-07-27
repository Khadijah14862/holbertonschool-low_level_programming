#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 */
void times_table(void)
{
	int row;
	int column;
	int product;

	row = 0;

	while (row <= 9)
	{
		column = 0;

		while (column <= 9)
		{
			product = row * column;

			if (column != 0)
			{
				_putchar(',');
				_putchar(' ');

				if (product < 10)
				{
					_putchar(' ');
				}
			}

			if (product >= 10)
			{
				_putchar((product / 10) + '0');
			}

			_putchar((product % 10) + '0');
			column++;
		}

		_putchar('\n');
		row++;
	}
}
