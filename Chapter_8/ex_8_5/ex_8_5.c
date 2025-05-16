/*
 * Exercise 8-5. Modify the fsize program to print the other information contained in the inode
entry.
*/

/*NOTE: no opedir, readir and closedir are reimplemented as in the book, 
	as system call read returns EISDIR if intup file descriptor in a directory,
	so need to use readdir which required standard library DIR*/

#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void fsize(char*);
void dirwalk(char *, void (*fcn)(char*));

int main(int argc, char *argv[]){
	if(argc == 1) /* default: currect directory */
		fsize(".");
	else
		while(--argc > 0)
			fsize(*++argv);
	return 0;
}

void fsize(char *name)
{
	struct stat stbuf;

	if(stat(name, &stbuf) == -1){
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}

	//NOTE: On why there might be errors when using:
	//(stbuf.st_mode & S_IFMT)  != S_IFDIR
	// read here: https://stackoverflow.com/questions/28547271/s-ifmt-and-s-ifreg-undefined-with-std-c11-or-std-gnu11
	// works whithout --std=* flag

	if(S_ISDIR(stbuf.st_mode))
		dirwalk(name, fsize);

	printf("%8ld bytes %8ld blocks %s\n", stbuf.st_size, stbuf.st_blocks, name);
}

#define MAX_PATH 1024

/* dirwalk: apply fcn to all files in dir*/
void dirwalk(char *dir, void (*fcn)(char*)){
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

	if((dfd = opendir(dir)) == NULL){
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}

	while((dp = readdir(dfd)) != NULL){
		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue; /* skin self and parent */

		if(strlen(dir) + strlen(dp->d_name)+2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s %s too long\n", dir, dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			fcn(name);
		}
	}

	closedir(dfd);
}


