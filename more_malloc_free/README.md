# More malloc, free

This directory contains C functions that practice dynamic memory allocation.

## Task 0

`malloc_checked` allocates a specified number of bytes using `malloc`.

If memory allocation fails, the function terminates the process with exit status 98.

Prototype:

void *malloc_checked(unsigned int b);
