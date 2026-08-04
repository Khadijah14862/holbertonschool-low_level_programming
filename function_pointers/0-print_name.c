#include "function_pointers.h"

/**
 * print_name - Prints a name using a callback function
 * @name: Name to print
 * @f: Pointer to the printing function
 */
void print_name(char *name, void (*f)(char *))
{
	f(name);
}
