/*
 * Exercise 4-4. Add the commands to print the top elements of the stack without popping, to
 * duplicate it, and to swap the top two elements. Add a command to clear the stack 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100          // max size of operand or operator
#define NUMBER '0'         // signal that a number was found
#define FUNCTION '1'       // signal that a function/command was found 
#define INVALID_INPUT '2'  // signal that invalid term is met

int gettop(char[]);
void push(double f);
double pop(void);
void execute_function(char function_name[]); // executes function with `function_name` is it is available


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
            case FUNCTION:
                execute_function(s);
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
			case '\n':
				printf("\t%.8g\n", pop());
				break;
		}
	}

	return 0;
}

#include <string.h>

#define MAXVAL 100   /*Maximim depth of val stack*/
#define FUNCTIONS_AMOUNT 3 // the amount of available functions

int sp = 0;          /*next free stack position*/
double val[MAXVAL];  /*value stack*/

char* available_functions[FUNCTIONS_AMOUNT] = {
                     "print", // print stack
                     "clear", // clear stack
                     "swap",  // swaps two top elements in the stack
                    };


// available functions defenitions
void print_stack(void);
void clear_stack(void);
void swap_top_two(void);

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

void execute_function(char function[])
{
    for(int i = 0; i < FUNCTIONS_AMOUNT; i++)
    {
        if(strncmp(function, available_functions[i], MAXOP) == 0)
        {
            switch (i)
            {
                case 0:
                    print_stack();
                return;

                case 1:
                    clear_stack();
                return;

                case 2:
                    swap_top_two();
                return;
            }
        }
    }
    printf("Error: unknown function `%s`\n", function);
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

void clear_stack(void)
{
   sp = 0;
}

void swap_top_two(void)
{
    double tmp = val[sp - 1];
    val[sp - 1] = val[sp - 2];
    val[sp - 2] = tmp;
}



#include <ctype.h>

int getch(void);
void ungetch(int);


/*gettop: get next charackter or numeric operand*/
int gettop(char s[])
{
	int i = 0, c = 0;

	while((s[0] = c = getch()) == ' ' || c == '\t');

	s[1] = '\0';

	if(!isdigit(c) && c != '.' && c != '-')
    {
        if(isalpha(c)) //probably it is a function
        {
            s[0] = c;
            while(isalpha(s[++i] = c = getch()) || isdigit(c));
            s[i] = '\0';

            if(c != EOF)
                ungetch(c);

            return FUNCTION;
        }
		return c;
    }

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

