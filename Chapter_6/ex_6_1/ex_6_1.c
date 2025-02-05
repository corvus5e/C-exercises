/*
 * Exercise 6-1. Our version of getword does not properly handle underscores, string constants,
 * comments, or preprocessor control lines. Write a better version.
*/

/* TODO:
   - string constants
   - preprocessor control lines
*/

#include <stdlib.h>
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
