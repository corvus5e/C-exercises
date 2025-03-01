/*
 * Exercise 7-1. Write a program that converts upper case to lower or lower case to upper,
 * depending on the name it is invoked with, as found in argv[0].
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 50

#define UPPER "upper"
#define LOWER "lower"

typedef int (*to_case_fn)(int);

void get_file_name(const char *path, char *file_name);

int main(int argc, char *argv[])
{
	char program_name[MAX_NAME_LEN];
	to_case_fn convert_fn;

	get_file_name(argv[0], program_name);

	if(strcmp(program_name, UPPER) == 0)
		convert_fn = toupper;
	else if(strcmp(program_name, LOWER) == 0)
		convert_fn = tolower;
	else {
		printf("Rename program to \"upper\" or \"lower\"\n");
		return 0;
	}

	for(int c; (c = getchar()) != EOF;){
		printf("%c", convert_fn(c));
	}

	return 0;
}

void get_file_name(const char *path, char *file_name)
{
	const char *last_slash_pos = path;

	for(;*path;++path)
		if(*path == '/')
			last_slash_pos = path;

	strcpy(file_name, last_slash_pos + 1);
}

