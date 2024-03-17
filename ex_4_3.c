#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100  // max size of operand or operator
#define NUMBER '0' // signal that a number was not found

int gettop(char[]);
void push(double f);
double pop(void);
void print_stack(void);

/*reverse Polish calculator*/
int main()
{
	int type;
	double op2;
	char s[MAXOP];

	while((type = gettop(s)) != EOF)
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
					push(pop() - op2);
				else
					printf("Error: Zero divisor\n");
				break;
			case '\n':
				printf("\t%.8g\n", pop());
				break;
		}
	}

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
int gettop(char s[])
{
	int i, c;

	while((s[0] = c = getch()) == ' ' || c == '\t');

	s[1] = '\0';

	if(!isdigit(c) && c != '.' && c != '-')
		return c; /*not a number*/

	i = 0;
	if(c == '-')
	{
		if(isdigit((c = getch())) || c == '.')
		{
			s[++i] = c; // negative number
		}
		else
		{
			if(c != EOF)
				ungetch(c);
			return '-'; /*minus operator*/
		}
	}

	if(isdigit(c)) /*collect integer part*/
		while(isdigit(s[++i] = c = getch()));

	if(c == '.') /*collect fraction part*/
		while(isdigit(s[++i] = c = getch()));

	s[i] = '\0';

	if(c != EOF)
		ungetch(c);

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









