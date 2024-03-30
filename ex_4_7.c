/*
 * Exercise 4-7. Write a routine ungets(s) that will push back an entire string onto the input.
 * Should ungets know about buf and bufp, or should it just use ungetch?
 * */

#include <stdio.h>
#include <string.h>

// Let's make it separately from the calculator program, which is the main task for 4th chapter
int getch(void);
void ungetch(int);
/*ungets string*/
void ungets(char s[], int len);
void ungets_atomic(char s[]);

int main()
{
    char greeting[] = "Hello there! Print something\n";

    ungets_atomic(greeting);

    int c = 0;

    while((c = getch()) != EOF)
    {
        printf("%c", c);
    }
}

// --- Implementation ---- //
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

/*This version puts up to `len` number of characters of the string `s` until there is place in the buffer*/
void ungets(char s[], int len)
{
    while(len)
    {
        ungetch(s[--len]);
    }
}

/*This version is "atomic" in a sence that if there is enoght space in the buffer,
 * it will put the whole string `s`, otherwise will not place and an error is reported 
 * This implementation should know about buf and bufp.*/
void ungets_atomic(char s[])
{
    int len = strlen(s);

    if(len <= BUFERSIZE - bufp)
    {
       ungets(s, len); 
    }
    else {
        printf("Error: not enough space to unget string");
    }
}

