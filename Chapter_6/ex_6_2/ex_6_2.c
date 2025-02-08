/*
  * Exercise 6-2. Write a program that reads a C program and prints in alphabetical order each
  * group of variable names that are identical in the first 6 characters, but different somewhere
  * thereafter. Don't count words within strings and comments. Make 6 a parameter that can be
  * set from the command line.
*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct tdata {
	char *word;
	struct tnode *group_root;
};

struct tnode {
	struct tnode *left;
	struct tnode *right;
	struct tdata *data;
};

#define MAXWORD 100

struct tnode *addtree(struct tnode *p, char *word);
void treeprint(struct tnode *root);
int getword(char *, int);
char *binsearch(char *, char *[], int);

int getch(void);
void ungetch(int);

/*Skips stdin untill pattern or EOF is met.
  Returns 1 is patters is met. 0 if EOF.
  Next stdin read would be just after the pattern.
  */
int skip_stdin_till(const char* pattern);

/* count C keywords */
int main(int argc, char* argv[])
{
	char *types[] = {"char", "double", "float", "int", "long", "short", "unsigned", "void"};
	size_t types_count = (sizeof types/ sizeof(types[0]));

	char *p;
	char word[MAXWORD];

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			if ((p = binsearch(word, types, types_count)) != NULL)
				printf("Found:%s\n", p);

	//treeprint();
	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
char *binsearch(char *word, char *tab[], int n)
{
	int cond;
	char **low = &tab[0];
	char **high = &tab[n];
	char **mid;

	while (low < high)
	{
		mid = low + (high - low) / 2;
		if ((cond = strcmp(word, *mid)) < 0)
			high = mid;
		else if (cond > 0)
			low = mid + 1;
		else
			return *mid;
	}

	return NULL;
}

struct tnode *addtree(struct tnode *p, char *word){
}

void treeprint(struct tnode *root){
}

/* getword: get next word or character from input */
int getword(char *word, int lim)
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
		}
		else if(c == '#'){ // this is a preprocessor 
			skip_stdin_till("\n");
			continue;
		}
		else if (c == '"') { // this is a string literal begin
			skip_stdin_till("\"");
			continue;
        	}

		if (c != EOF)
			*w++ = c;

		if (!isalpha(c)) {
			*w = '\0';
			return c;
		}

		for ( ; --lim > 0; w++)
			if (!isalnum(*w = getch()) && *w != '_') {
				ungetch(*w);
				break;
			}
	}

	*w = '\0';
	printf("word:%s\n", word);
	return word[0];
}

int skip_stdin_till(const char* pattern)
{
	//TODO: Make buf a parameter ?
	size_t n = strlen(pattern);
	char *buf = malloc(n + 1);

	if(!buf)
		return 0;

	//read first n chars
	int i = 0;
	for(; i < n && (buf[i] = getch()) != EOF; ++i)
	    ;
	
	if(buf[i] == EOF){ // we exited from previous loop because of EOF
	    free(buf);
	    return 0;
	}

	buf[n] = '\0';

	char *b = NULL;
	while(strcmp(buf, pattern) != 0){
		for(b = buf; *b && *(b+1); ++b) //shift buf left by one
			*b = *(b + 1);
		
		if((*b = getch()) == EOF){
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
