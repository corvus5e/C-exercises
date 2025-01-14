/*
* Exercise 5-13. Write the program tail, which prints the last n lines of its input. By default, n
* is set to 10, let us say, but it can be changed by an optional argument so that
*	tail -n
* prints the last n lines. The program should behave rationally no matter how unreasonable the
* input or the value of n. Write the program so it makes the best use of available storage; lines
* should be stored as in the sorting program of Section 5.6, not in a two-dimensional array of
* fixed size.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LINES_NUM 10

int main(int argc, char* argv[]){
	int n = DEFAULT_LINES_NUM;
	if(argc > 1){
		int tmp_n = atoi(argv[1]);
		if(tmp_n > 0)
			n = tmp_n;
	}

	char** lines = malloc(n * sizeof(char*));
	size_t* lengths = malloc(n * sizeof(size_t));

	for(int i = 0; i < n; ++i){
		lines[i] = NULL;
		lengths[i] = 0;
	}

	ssize_t nread = 0;
	int curr_line = 0;

	while((nread = getline(lines + curr_line, lengths + curr_line, stdin)) != -1){
		++curr_line;
		curr_line %= n;
	}

	for(int i = curr_line; i < n; ++i)
		if(lines[i] != NULL)
			printf("%s", lines[i]);

	for(int i = 0; i < curr_line; ++i)
		printf("%s", lines[i]);

	for(int i = 0; i < n; ++i)
		if(lines[i] != NULL)
			free(lines[i]);

	free(lines);
	free(lengths);
	
	return 0;
}
