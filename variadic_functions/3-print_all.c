#include "variadic_functions.h"
#include <stdarg.h>
#include <stdio.h>

/**
 * print_all - Prints arguments according to a format string
 * @format: List of argument types
 */
void print_all(const char * const format, ...)
{
	va_list arguments;
	unsigned int i;
	char *string;
	const char *separator;

	i = 0;
	separator = "";
	va_start(arguments, format);

	while (format != NULL && format[i] != '\0')
	{
		switch (format[i])
		{
		case 'c':
			printf("%s%c", separator, va_arg(arguments, int));
			separator = ", ";
			break;
		case 'i':
			printf("%s%d", separator, va_arg(arguments, int));
			separator = ", ";
			break;
		case 'f':
			printf("%s%f", separator, va_arg(arguments, double));
			separator = ", ";
			break;
		case 's':
			string = va_arg(arguments, char *);
			if (string == NULL)
				string = "(nil)";
			printf("%s%s", separator, string);
			separator = ", ";
			break;
		default:
			break;
		}
		i++;
	}

	va_end(arguments);
	printf("\n");
}
