/*
 * Exercise 6-6. Implement a simple version of the #define processor (i.e., no arguments)
 * suitable for use with C programs, based on the routines of this section. You may also find
 * getch and ungetch helpful.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
typedef int (*stop_cmp)(int);

int getword(char *, int, stop_cmp);
int skip_stdin_till(const char* pattern);
/*Returns 1  is next chars in std in equal to str(null-terminating char not included).
  Otherwise returns 0*/
int next_stdin_equals_to(const char *str);
int getch(void);
void ungetch(int);
int is_new_line(int c) { return c == '\n';}


int main()
{
	char name[MAXWORD];
	char value[MAXWORD];

	while(getword(name, MAXWORD, isspace) != EOF){
		if(strcmp(name, "#define") == 0 &&
		getword(name, MAXWORD, isspace) != EOF &&
		getword(value, MAXWORD, is_new_line) != EOF)
		{
			printf("%s %s\n", name, value);
		}
	}
}

int getword(char *word, int lim, stop_cmp cmp)
{
	char *w = word;
	*w = '\0'; // consider the buffer as empty at start
	int c = 0;

	while(c != EOF && !word[0]){
		while (isspace(c = getch()))
			;

		if(c == EOF)
			return EOF;

		if(c == '/'){ // possible comment section
			if((c = getch()) == '/'){ // this is line comment
				skip_stdin_till("\n");
				continue;
			}
			else if(c == '*'){ // this is a block comment
				skip_stdin_till("*/");
				continue;
			}
			ungetch(c);
			c = '/';
		}

		*w++ = c;

		for ( ; --lim > 0; w++)
			if (cmp(*w = getch())) {
				ungetch(*w);
				break;
			}
	}

	*w = '\0';

	return word[0];
}

int skip_stdin_till(const char* pattern)
{
	size_t n = strlen(pattern);
	char *buf = malloc(n + 1);

	if(!buf)
		return 0;

	//read first n chars
	int i = 0;
	for(; i < n && (buf[i] = getch()) != EOF; ++i)
	    ;
	
	if(buf[i] == EOF){ // we exited from previous loop because of EOF
		ungetch(buf[i]);
		free(buf);
		return 0;
	}

	buf[n] = '\0';

	char *b = NULL;
	while(strcmp(buf, pattern) != 0){
		for(b = buf; *b && *(b+1); ++b) //shift buf left by one
			*b = *(b + 1);
		
		if((*b = getch()) == EOF){
			ungetch(*b);
			free(buf);
			return 0;
		}
		//printf("buf:%s\n", buf);
	}

	return 1;
}

int next_stdin_equals_to(const char *str)
{
	int res = 1; //true

	for(;*str && (res = (*str == getchar())); ++str)
		;

	if(*str == EOF)
		ungetch(EOF);

	return res;
}

#define BUFERSIZE 10

char buf[BUFERSIZE];/*buffer for ungetch*/
int bufp = 0; /*next free position in buf*/

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /*push character back to input*/
{
	if(bufp >= BUFERSIZE)
		printf("Error: ungetch - too many charackters\n");
	else
	{
		buf[bufp++] = c;
	}
}
