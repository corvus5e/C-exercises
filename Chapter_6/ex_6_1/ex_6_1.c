/*
 * Exercise 6-1. Our version of getword does not properly handle underscores, string constants,
 * comments, or preprocessor control lines. Write a better version.
*/

/* TODO:
   - string constants
   - preprocessor control lines
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct key {
	char *word;
	int count;
} keytab[] = { {"auto", 0},     {"break", 0},   {"case", 0},   {"char", 0},     {"const", 0},
			   {"continue", 0}, {"default", 0}, {"do", 0},     {"double", 0},   {"else", 0},
			   {"enum", 0},     {"extern", 0},  {"float", 0},  {"for", 0},      {"goto", 0},
			   {"if", 0},       {"int", 0},     {"long", 0},   {"register", 0}, {"return", 0},
			   {"short", 0},    {"signed", 0},  {"sizeof", 0}, {"static", 0},   {"struct", 0},
			   {"switch", 0},   {"typedef", 0}, {"union", 0},  {"unsigned", 0}, {"void", 0},
			   {"volatile", 0}, {"while", 0}};

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(keytab[0]))

#define EMPTY_FLAGS   0
#define BLOCK_COMMENT 1
#define LINE_COMMENT  2
#define STRING_CONST  4
#define PREPROCESSOR  8

char flags_s = EMPTY_FLAGS;

int getword(char *, int);
int binsearch(char *, struct key *, int);

/* count C keywords */
int main()
{
	int n;
	char word[MAXWORD];

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			if ((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;

	for (n = 0; n < NKEYS; n++)
		if (keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);

	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;

	while (low <= high)
	{
		mid = (low+high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}

	return -1;
}

/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	int getch(void);
	void ungetch(int);

	char *w = word;
	*w = '\0'; // consider the buffer as empty at start
	int c = 0;

	while(c != EOF && !word[0]){

		while (isspace(c = getch()))
			;

		if(c == EOF)
			return EOF;

		if(c == '/'){ // possible comment section
			// TODO: make separate function, for example skip_till(char* end_pattern);
			if((c = getch()) == '/'){ // this is line comment
				while((c = getch()) != EOF && c != '\n')
					;
				continue; // comment line is finished, no worlds, start from the beginnig
			}
			else if(c == '*'){ // this is a block comment
				char b = 0;
				c = getch();
				while(b != EOF && c != EOF && !(b == '*' && c == '/')){
					b = c;
					c = getch();
				}
				continue; // comment block is finished, no worlds, start from the beginnig
			}
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

	return word[0];
}

#define BUFERSIZE 100

char buf[BUFERSIZE];/*buffer for ungetch*/
int bufp = 0; /*next free position in buf*/

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /*push charackter back to input*/
{
	if(bufp >= BUFERSIZE)
		printf("Error: ungetch - too many charackters\n");
	else
	{
		buf[bufp++] = c;
	}
}
