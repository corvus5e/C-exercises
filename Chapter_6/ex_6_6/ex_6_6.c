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

int getword(char *, int);
int read_literal(char*, char);
int read_defn(char *, int);
int skip_stdin_till(const char* pattern);
int getch(void);
void ungetch(int);

int main()
{
	char word[MAXWORD];
	char value[MAXWORD];
	int c = 0;

	while((c = getword(word, MAXWORD)) != EOF){
		if(c == '\n'){
			printf("\n");
			continue;
		}
		if(!strcmp(word, "#define")){
			if(getword(word, MAXWORD) != EOF && read_defn(value, MAXWORD) != EOF)
				printf("[%s:%s]", word, value);
		}
		else
			printf("%s", word);
	}
}

int getword(char *word, int lim)
{
	char *w = word;
	*w = '\0'; // consider the buffer as empty at start
	int c = 0;

	while(c != EOF && !word[0]){
		while (isspace(c = getch())){
			if(c == '\n')
				return c;
			printf("%c", c);
		}

		if(c == EOF)
			return EOF;

		if(c == '/'){ // possible comment section
			if((c = getch()) == '/'){ // this is line comment
				skip_stdin_till("\n");
				ungetch('\n');
				continue;
			}
			else if(c == '*'){ // this is a block comment
				skip_stdin_till("*/");
				continue;
			}
			ungetch(c);
			c = '/';
		}
		else if(c == '"'){
			*w++ = c;
			return read_literal(w, '\"');
		}

		*w++ = c;

		if(!isalnum(c) && c != '#') //write # and directive in one word
			break;

		for ( ; --lim > 0; w++)
			if (!isalnum(*w = getch()) && *w != '_') {
				ungetch(*w);
				break;
			}
	}

	*w = '\0';

	return word[0];
}

int read_literal(char *buf, char stop_char)
{
	while((*buf = getch()) != EOF && *buf != stop_char){
		if(*buf == '\\') //escape
			*(++buf) = getchar();
		++buf;
	}
	if(*buf == EOF)
		ungetch(EOF);

	*(++buf) = '\0';

	return *buf;
}

int read_defn(char *defn, int lim)
{
	int c;
	char word[MAXWORD];

	while((c = getword(word, lim)) != EOF && c != '\n'){
		//printf("===>%s\n", word);
		char *w = &word[0];
		while((*defn = *w)){
			++defn;
			++w;
		}
	}
	if(c == '\n')
		printf("\n");
	return c;
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
