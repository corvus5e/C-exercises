/*
 * Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write, open, and close
 * instead of their standard library equivalents. Perform experiments to determine the relative
 * speeds of the two versions.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 25

int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("Usage: my_cat <file>\n");

	const char *path = argv[1];

	int fd = -1;
	if((fd = open(path, O_RDONLY)) < 0){
		printf("Failed to open file %s\n", path);
		return -1;
	}

	char buf[BUF_SIZE];
	int n = -1;

	while((n = read(fd, buf, BUF_SIZE)) > 0)
		if(write(STDOUT_FILENO, buf, n) != n){
			printf("Failed to write file. Exiting ...\n");
			break;
		}

	close(fd);

	return 0;
}
