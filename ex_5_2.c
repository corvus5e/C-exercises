#include <stdio.h>
#include <ctype.h>

#define SIZE 100

int getint(int *pn);
int getfloat(float *pn);

int main()
{
    float arr[SIZE]; 
    int status = 0;

    int n = 0;
    while(n < SIZE && ((status = getfloat(&arr[n])) != EOF))
    {
        if(status)
        {
            printf("n= %d, %f,\n", n, arr[n]);
            ++n;
        }
        else {
            printf("Not a number\n");
        }
    }

}

int getch(void);
void ungetch(int);

int getfloat(float *pn)
{
    int res = 0;
    int status = 0;
    
    if((status = getint(&res)) == EOF)
       return status;

    *pn = (float)res;

    int c;
    if((c = getch()) == '.') // we have fraction part
    {
        c = getch();

        if(!isdigit(c))
        {
            ungetch(c);
            return status;
        }

        int sign = res < 0 ? -1 : 1;

        float factor = 0.1;

        for(; isdigit(c); c = getch(), factor *= 0.1)
            *pn += sign * factor * (c - '0');

        ungetch(c); 
        return c;
    }

    return status;
}

int getint(int *pn)
{
    int c, sign; 
    
    while(isspace(c = getch()))
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

void ungetch(int c) /*push charackter back to input*/
{
    buf = c;
}
