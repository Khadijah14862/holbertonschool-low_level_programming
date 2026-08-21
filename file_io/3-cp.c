#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * close_file - closes a file descriptor
 * @fd: file descriptor to close
 */
void close_file(int fd)
{
	if (close(fd) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
		exit(100);
	}
}

/**
 * read_error - handles a file read error
 * @name: name of the file
 * @fd_from: source file descriptor
 * @fd_to: destination file descriptor
 */
void read_error(char *name, int fd_from, int fd_to)
{
	dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", name);

	if (fd_from != -1)
		close_file(fd_from);
	if (fd_to != -1)
		close_file(fd_to);

	exit(98);
}

/**
 * write_error - handles a file write error
 * @name: name of the destination file
 * @fd_from: source file descriptor
 * @fd_to: destination file descriptor
 */
void write_error(char *name, int fd_from, int fd_to)
{
	dprintf(STDERR_FILENO, "Error: Can't write to %s\n", name);

	if (fd_from != -1)
		close_file(fd_from);
	if (fd_to != -1)
		close_file(fd_to);

	exit(99);
}

/**
 * copy_content - copies data between file descriptors
 * @fd_from: source file descriptor
 * @fd_to: destination file descriptor
 * @from: source file name
 * @to: destination file name
 */
void copy_content(int fd_from, int fd_to, char *from, char *to)
{
	char buffer[1024];
	ssize_t bytes_read, bytes_written;

	bytes_read = read(fd_from, buffer, 1024);

	while (bytes_read > 0)
	{
		bytes_written = write(fd_to, buffer, bytes_read);

		if (bytes_written != bytes_read)
			write_error(to, fd_from, fd_to);

		bytes_read = read(fd_from, buffer, 1024);
	}

	if (bytes_read == -1)
		read_error(from, fd_from, fd_to);
}

/**
 * main - copies the content of one file to another
 * @argc: number of arguments
 * @argv: array of arguments
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	int fd_from, fd_to;

	if (argc != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}

	fd_from = open(argv[1], O_RDONLY);
	if (fd_from == -1)
		read_error(argv[1], -1, -1);

	fd_to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
		write_error(argv[2], fd_from, -1);

	copy_content(fd_from, fd_to, argv[1], argv[2]);

	close_file(fd_from);
	close_file(fd_to);

	return (0);
}
