/*
 * Exercise 5-18. Make dcl recover from input errors.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);
int gettoken(void);

int tokentype;

/* type of last token */
char token[MAXTOKEN];

/* last token string */
char name[MAXTOKEN];

/* identifier name */
char datatype[MAXTOKEN]; /* data type = char, int, etc. */

char out[1000];

int main() /* convert declaration to words */
{
	while (gettoken() != EOF)
	{
		/* 1st token on line */
		strcpy(datatype, token); /* is the datatype */

		out[0] = '\0';

		dcl();

		/* parse rest of line */
		if (tokentype != '\n')
			printf("syntax error\n");

		printf("%s: %s %s\n", name, out, datatype);
	}

	return 0;
}

/* dcl: parse a declarator */
void dcl(void)
{
	int ns;

	for (ns = 0; gettoken() == '*'; ) /* count *'s */
		ns++;

	dirdcl();

	while (ns-- > 0)
		strcat(out, " pointer to");
}

/* dirdcl: parse a direct declarator */
void dirdcl(void)
{
	int type;
	if (tokentype == '(') { /* ( dcl ) */
		dcl();
		if (tokentype != ')')
			printf("error: missing )\n");
	} else if (tokentype == NAME) /* variable name */
		strcpy(name, token);
	else
		printf("error: expected name or (dcl)\n");

	while ((type=gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			strcat(out, " function returning");
		else {
			strcat(out, " array");
			strcat(out, token);
			strcat(out, " of");
		}
}

int gettoken(void) /* return next token */
{
	int c, getch(void);
	void ungetch(int);

	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;

	if (c == '(')
	{
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		}
		else
		{
			ungetch(c);
			return tokentype = '(';
		}
	}
	else if (c == '[')
	{
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;

		*p = '\0';

		return tokentype = BRACKETS;
	}
	else if (isalpha(c))
	{
		for (*p++ = c; isalnum(c = getch());)
			*p++ = c;

		*p = '\0';

		ungetch(c);

		return tokentype = NAME;
	}
	else return tokentype = c;
}

char buf = 0; /*buffer for ungetch*/

int getch(void)
{
    if(buf != 0)
    {
        char tmp = buf;
        buf = 0;
        return tmp;
    }
 
	return getchar();
}

void ungetch(int c) /*push charackter back to input*/
{
    if(buf == 0)
    {
        buf = c;
    }
    else {
        printf("Error: the buffer with length 1 is full, can't unget");
    }
}
