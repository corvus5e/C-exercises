/*
 * Excercise 5-1. As written, getint treats a + or - not followed by a digit as a valid
 * representation of zero. Fix it to push such a character back on the input.
 * */

#include <stdio.h>
#include <ctype.h>

#define SIZE 100

int getint(int *pn);

int main()
{
    int arr[SIZE]; 
    int status = 0;

    for(int n = 0; n < SIZE && ((status = getint(&arr[n])) != EOF); ++n)
    {
        if(status)
            printf("%d ,\n", arr[n]);
    }

}

int getch(void);
void ungetch(int);

int getint(int *pn)
{
    int c, sign; 
    
    while(isspace(c = getch())) /*skip white-space*/
        ;

    if(!isdigit(c) && c != EOF && c != '-' && c != '+')
    {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if(c == '-' || c == '+')
        c = getch();

    if(c != EOF && !isdigit(c))
    {
        ungetch(c);
        return 0;
    }

    for(*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');

    *pn *= sign;

    if(c != EOF)
        ungetch(c);

    return c;
}

int buf = 0;       /*next free position in buf*/

int getch(void)
{
    if(buf > 0)
    {
        int tmp = buf;
        buf = 0;
        return tmp;
    }

	return getchar();
}

void ungetch(int c)
{
    buf = c;
}
