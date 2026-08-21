#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

/**
 * print_magic - prints the ELF magic bytes
 * @e: ELF identification bytes
 */
void print_magic(unsigned char *e)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", e[i]);
		if (i != EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

/**
 * print_class - prints the ELF class
 * @e: ELF identification bytes
 */
void print_class(unsigned char *e)
{
	printf("  Class:                             ");
	if (e[EI_CLASS] == ELFCLASS32)
		printf("ELF32\n");
	else if (e[EI_CLASS] == ELFCLASS64)
		printf("ELF64\n");
	else
		printf("<unknown: %x>\n", e[EI_CLASS]);
}

/**
 * print_data - prints the ELF data encoding
 * @e: ELF identification bytes
 */
void print_data(unsigned char *e)
{
	printf("  Data:                              ");
	if (e[EI_DATA] == ELFDATA2LSB)
		printf("2's complement, little endian\n");
	else if (e[EI_DATA] == ELFDATA2MSB)
		printf("2's complement, big endian\n");
	else
		printf("<unknown: %x>\n", e[EI_DATA]);
}

/**
 * print_version - prints the ELF identification version
 * @e: ELF identification bytes
 */
void print_version(unsigned char *e)
{
	printf("  Version:                           %d", e[EI_VERSION]);
	if (e[EI_VERSION] == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

/**
 * print_osabi - prints the ELF OS/ABI
 * @e: ELF identification bytes
 */
void print_osabi(unsigned char *e)
{
	printf("  OS/ABI:                            ");
	switch (e[EI_OSABI])
	{
	case 0:
		printf("UNIX - System V\n");
		break;
	case 1:
		printf("UNIX - HP-UX\n");
		break;
	case 2:
		printf("UNIX - NetBSD\n");
		break;
	case 3:
		printf("UNIX - GNU\n");
		break;
	case 6:
		printf("UNIX - Solaris\n");
		break;
	case 9:
		printf("UNIX - FreeBSD\n");
		break;
	case 12:
		printf("UNIX - OpenBSD\n");
		break;
	default:
		printf("<unknown: %x>\n", e[EI_OSABI]);
	}
}

/**
 * get_value - reads an integer using ELF byte order
 * @e: ELF header bytes
 * @start: first byte offset
 * @size: number of bytes
 *
 * Return: decoded value
 */
unsigned long get_value(unsigned char *e, int start, int size)
{
	unsigned long value = 0;
	int i;

	if (e[EI_DATA] == ELFDATA2MSB)
	{
		for (i = 0; i < size; i++)
			value = (value << 8) | e[start + i];
	}
	else
	{
		for (i = size - 1; i >= 0; i--)
			value = (value << 8) | e[start + i];
	}
	return (value);
}

/**
 * print_type - prints the ELF object type
 * @e: ELF header bytes
 */
void print_type(unsigned char *e)
{
	unsigned long type = get_value(e, 16, 2);

	printf("  Type:                              ");
	switch (type)
	{
	case ET_NONE:
		printf("NONE (None)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("<unknown>: %lx\n", type);
	}
}

/**
 * print_entry - prints the ELF entry point
 * @e: ELF header bytes
 */
void print_entry(unsigned char *e)
{
	unsigned long high, low;

	printf("  Entry point address:               0x");
	if (e[EI_CLASS] == ELFCLASS32)
	{
		printf("%lx\n", get_value(e, 24, 4));
		return;
	}
	if (e[EI_DATA] == ELFDATA2MSB)
	{
		high = get_value(e, 24, 4);
		low = get_value(e, 28, 4);
	}
	else
	{
		low = get_value(e, 24, 4);
		high = get_value(e, 28, 4);
	}
	if (high)
		printf("%lx%08lx\n", high, low);
	else
		printf("%lx\n", low);
}

/**
 * check_elf - checks whether a buffer contains an ELF header
 * @e: ELF header bytes
 * @n: number of bytes read
 * @name: file name
 */
void check_elf(unsigned char *e, ssize_t n, char *name)
{
	ssize_t need;

	if (n < EI_NIDENT || e[EI_MAG0] != ELFMAG0 ||
	    e[EI_MAG1] != ELFMAG1 || e[EI_MAG2] != ELFMAG2 ||
	    e[EI_MAG3] != ELFMAG3)
	{
		dprintf(STDERR_FILENO, "Error: %s is not an ELF file\n", name);
		exit(98);
	}
	if (e[EI_CLASS] != ELFCLASS32 && e[EI_CLASS] != ELFCLASS64)
	{
		dprintf(STDERR_FILENO, "Error: invalid ELF class in %s\n", name);
		exit(98);
	}
	need = (e[EI_CLASS] == ELFCLASS32) ? 52 : 64;
	if (n < need)
	{
		dprintf(STDERR_FILENO, "Error: incomplete ELF header in %s\n", name);
		exit(98);
	}
}

/**
 * read_header - opens a file and reads its ELF header
 * @name: file name
 * @header: buffer for the ELF header
 *
 * Return: number of bytes read
 */
ssize_t read_header(char *name, unsigned char *header)
{
	ssize_t n;
	int fd;

	fd = open(name, O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't open file %s\n", name);
		exit(98);
	}
	n = read(fd, header, 64);
	if (n == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", name);
		close(fd);
		exit(98);
	}
	if (close(fd) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close file %s\n", name);
		exit(98);
	}
	return (n);
}

/**
 * main - displays selected information from an ELF header
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	unsigned char header[64];
	ssize_t n;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
		exit(98);
	}
	n = read_header(argv[1], header);
	check_elf(header, n, argv[1]);
	printf("ELF Header:\n");
	print_magic(header);
	print_class(header);
	print_data(header);
	print_version(header);
	print_osabi(header);
	printf("  ABI Version:                       %d\n",
	       header[EI_ABIVERSION]);
	print_type(header);
	print_entry(header);
	return (0);
}
