/*
 * Exercise 5-10. Write the program expr, which evaluates a reverse Polish expression from the
 * command line, where each operator or operand is a separate argument. For example,
 * expr 2 3 4 + *
 * evaluates 2 * (3+4). 
**/

/*
 * The basic Polish calculator copied from exercise 4.3*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100  // max size of operand or operator
#define NUMBER '0' // signal that a number was found

int gettop(char*);
void push(double f);
double pop(void);
void print_stack(void);

/*reverse Polish calculator with cmd input*/
int main(int argc, char *argv[])
{
	int type;
	double op2;
    char *s = *++argv;

    for(;s && (type = gettop(s)) != EOF; s = *++argv)
	{
		switch(type)
		{
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '%':
				op2 = pop();
				if(op2 != 0.0)
					push(fmod(pop(), op2));
				else
					printf("Error: Zero devisor\n");
				break;
			case '/':
				op2 = pop();
				if(op2 != 0.0)
					push(pop() / op2);
				else
					printf("Error: Zero divisor\n");
				break;
		}
	}

    printf("\t%.8g\n", pop());

	return 0;
}

#define MAXVAL 100   /*Maximim depth of val stack*/

int sp = 0;          /*next free stack position*/
double val[MAXVAL];  /*value stack*/

/*push: push f onto value stack*/
void push(double f)
{
	if(sp < MAXVAL)
		val[sp++] = f;
	else
		printf("Error: stack full, can't push %g\n", f);
}

/*pop: pop and return top value from stack*/
double pop(void)
{
	if(sp > 0)
		return val[--sp];
	else
	{
		printf("Error: pop() - Stack is empty\n");
		return 0.0;
	}
}

void print_stack(void)
{
	if(sp > 0)
	{
		printf("Stack:\n");
		for(int i = 0; i < sp; ++i)
		{
			printf("%f ", val[i]);
		}
		printf("\n");
	}
	else
		printf("Stack is empty\n");
}

#include <ctype.h>

int getch(void);
void ungetch(int);


/*gettop: get next charackter or numeric operand*/
int gettop(char *s)
{ 
    if(!s){
        return EOF;
    }

	for(;*s == ' ' || *s == '\t'; ++s)
        ;

	if(!isdigit(*s) && *s != '.' && *s != '-')
		return *s; /*not a number*/

    if(*s == '-' && !*++s){
        return '-'; // not a number
    }


	return NUMBER;
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

