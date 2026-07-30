# Simple Calculator

A terminal-based interactive calculator written in C.

## Compilation

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 calculator.c -o calculator

## Running

./calculator

## Supported Operations

- Addition
- Subtraction
- Multiplication
- Division
- Quit using option 0

## Numeric Behavior

The calculator uses double-precision decimal numbers.

All arithmetic operations accept integers or decimal values.

Examples:

- 10 + 25 produces 35
- 10 / 4 produces 2.5

## Known Limitations

The mandatory version expects numeric input.

Non-numeric input such as letters or words is not handled.

Division by zero is rejected with an error message.
