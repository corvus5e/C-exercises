/*
 * Exercise 7-2. Write a program that will print arbitrary input in a sensible way. As a
 * minimum, it should print non-graphic characters in octal or hexadecimal according to local
 * custom, and break long text lines.
*/

// Prints chars is they are ascii, otherwise decimal/hex/octal code

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

const char *hex = "[0x%x]";
const char *oct = "[0%o]";
const char *dec = "[%d]";
const char *gra = "%c";/*graphical*/

void read_params(int argc, char *argv[], int *column_lim, const char **non_ascii_fmt);

int main(int argc, char *argv[])
{
	const char *fmt_switch[2] = {hex, gra};
	int column_lim = 40;

	read_params(argc, argv, &column_lim, &fmt_switch[0]);

	int n;
	for(int c; (c = getchar()) != EOF;){
		n = printf(fmt_switch[c < 128], c);
	}
}

void read_params(int argc, char *argv[], int *column_lim, const char **non_ascii_fmt)
{
	if(argc < 2)
		return;

	for(int i = 0; i < argc; ++i){
		char first_char = argv[i][0];
		if(first_char == '-'){ // this is a key
			switch(argv[i][1]){
				case 'h': *non_ascii_fmt = hex; break;
				case 'o': *non_ascii_fmt = oct; break;
				case 'd': *non_ascii_fmt = dec; break;
			}
		}
		else if(isnumber(first_char)){
			*column_lim = atoi(argv[i]);
		}
	}
	//printf("Params:\nfmt:%s\nlim:%d\n", *non_ascii_fmt, *column_lim);
}
