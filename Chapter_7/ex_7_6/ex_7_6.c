/*
 * Exercise 7-6. Write a program to compare two files, printing the first line where they differ.
 */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LEN 100

/*Returns zero if files are equal, otherwise - the line number where files differ
  diff_line will be assigned with the malloced pointed to the corresponding line
  -1 if error occured
*/
int files_diff(FILE* l, FILE* r, char *r_diff_line, char *l_diff_line, int *column);
int lines_diff(const char *l, size_t l_len, const char *r, size_t r_len);
int get_line(char *line, int max, FILE *file);
void printf_diff(char *l, char *r, int line, int column);

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Provide two files to compare\n");
		return 0;
	}

	FILE *f1 = fopen(argv[1], "r");
	FILE *f2 = fopen(argv[2], "r");

	if(!f1 || !f2){
		printf("Failed to open at least one of specified files\n");
	}

	char r_diff_line[MAX_LINE_LEN];
	char l_diff_line[MAX_LINE_LEN];

	int column = -1;
	int line = files_diff(f1, f2, l_diff_line, r_diff_line, &column);

	if(line == 0)
		printf("Files are equal\n");
	else
		printf_diff(l_diff_line, r_diff_line, line, column);

	return 0;
}

int files_diff(FILE* l, FILE* r, char *r_diff_line, char *l_diff_line, int *column)
{
	char l_line[MAX_LINE_LEN];
	char r_line[MAX_LINE_LEN];

	int line_cnt = 1;
	int r_len = 0;
	int l_len = 0;
	
	for(;(r_len = get_line(l_line, MAX_LINE_LEN, l)) >= 0 &&
		(l_len = get_line(r_line, MAX_LINE_LEN, r)) >= 0 &&
		(*column = lines_diff(l_line, l_len, r_line, r_len)) == -1; ++line_cnt)
		;

	if(r_len < 0 || l_len < 0)
		return -1;

	if(r_len == 0 && l_len == 0)
		return 0;

	strcpy(l_diff_line, l_line);
	strcpy(r_diff_line, r_line);

	return line_cnt;
}

int lines_diff(const char *l, size_t l_len, const char *r, size_t r_len)
{
	int diff_pos = 0;
	size_t min = l_len < r_len ? l_len : r_len;

	while(min-- > 0 && *l == *r && *l){
		++diff_pos;
		++l;
		++r;
	}

	if(!*l && !*r) // both strings reached the end of string, so they are equal
		return -1;

	return diff_pos;
}

int get_line(char *line, int max, FILE *file)
{
	if(fgets(line, max, file) == NULL)
		return ferror(file) ? EOF : 0;
	
	return strlen(line);
}

void printf_diff(char *l, char *r, int line, int column)
{
	printf("On line %d, column %d:\n< %s", line, column, l);

	for(int i = 0; i < column + 2; ++i)
		printf("-");

	printf("*\n");

	printf("> %s", r);
}
