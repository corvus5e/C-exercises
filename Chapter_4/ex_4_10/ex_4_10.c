/**
* Exercise 4-10. An alternate organization uses getline to read an entire input line; this makes
* getch and ungetch unnecessary. Revise the calculator to use this approach
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100          // max size of operand or operator
#define NUMBER '0'         // signal that a number was found
#define FUNCTION '1'       // signal that a function/command was found 
#define VARIABLE '2'

int gettop(char[]);
void push(double f);
double pop(void);
void execute_function(char function_name[]); // executes function with `function_name` is it is available
void handle_variable(char);
void assign_variable(double);

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
            case VARIABLE:
                handle_variable(s[0]);
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
            case '=':
                //TODO: save previous type to check is assigns to a variable?
                pop(); 
                assign_variable(pop());
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
#define FUNCTIONS_AMOUNT 7 // the amount of available functions

int sp = 0;          /*next free stack position*/
double val[MAXVAL];  /*value stack*/

double variables['Z' - 'A']; // not sure how many letters are there)
int var_index;               // index of the current variable we are working with

char* available_functions[FUNCTIONS_AMOUNT] = {
                     "print", // print stack
                     "clear", // clear stack
                     "swap",  // swaps two top elements in the stack
                     "duplicate", // duplicates current stack
                     "sin",       // applies sin to the top element in the stask
                     "exp",       // calls exp with top element as argument
                     "pow"        // applies exponent function with two top elements as arguments
                    };


// available functions defenitions
void print_stack(void);
void clear_stack(void);
void swap_top_two(void);
void duplicate_stack(void);
void sin_function(void);
void exp_function(void);
void pow_function(void);

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
                case 0: print_stack(); return;

                case 1: clear_stack(); return;

                case 2: swap_top_two(); return;

                case 3: duplicate_stack(); return;

                case 4: sin_function(); return;

                case 5: exp_function(); return;

                case 6: pow_function(); return;
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

void duplicate_stack(void)
{
    if(2*sp - 1 < MAXVAL)
    {
        int curr_sp = sp;

        for(int i = 0; i < curr_sp; i++)
        {
           push(val[i]);
        }
    }
    else {
        printf("Error: can't duplicate stack. Not enough space left\n"); 
    }
}

void sin_function(void)
{
    push(sin(pop()));
}

void exp_function(void)
{
    push(exp(pop()));
}

void pow_function(void)
{
    double exp = pop();
    push(pow(pop(), exp));
}

void handle_variable(char name) // assume the var name is correct, checked in gettop()
{
    var_index = name - 'A'; // saving current index
    push(variables[var_index]);
}

void assign_variable(double value)
{
    variables[var_index] = value;
}



#include <ctype.h>

int getch(void);
void ungetch(int);

int get_line(char s[], int lim);
#define MAXLINELEN 100
char line[MAXLINELEN] = {'\0'};
int line_index = 0;

/*gettop: get next charackter or numeric operand*/
int gettop(char s[])
{
    if(line[line_index] == '\0')
    {
        if(get_line(line, MAXLINELEN) == 0)
            return EOF;
        else
            line_index = 0;
    }

	int i = 0, c = 0;

	while((s[0] = c = line[line_index++]) == ' ' || c == '\t');

	s[1] = '\0';

	if(!isdigit(c) && c != '.' && c != '-')
    {
        if(isalpha(c)) //probably it is a function
        {
            s[0] = c;
            while(isalpha(s[++i] = c = line[line_index++]) || isdigit(c));
            s[i] = '\0';

            --line_index; 
            
            if (i == 1 && (s[0] >= 'A' || s[0] <= 'Z')) // variables are upper case letters from A to Z
                return VARIABLE;

            return FUNCTION; 
        }
		return c;
    }

	if(c == '-')
	{
		if(isdigit((c = line[line_index++])) || c == '.')
		{
			s[++i] = c; // negative number
		}
		else
		{
	        --line_index;	

			return '-'; /*minus operator*/
		}
	}

	if(isdigit(c)) /*collect integer part*/
		while(isdigit(s[++i] = c = line[line_index++]));

	if(c == '.') /*collect fraction part*/
		while(isdigit(s[++i] = c = line[line_index++]));

	s[i] = '\0';

	--line_index;	

	return NUMBER;
}

int get_line(char s[], int lim)
{
    int c = 0;
    int i = 0;
    for(; i < lim-1 && (c = getchar()) != EOF && c != '\n'; ++i)
    {
        s[i] = c;
    }

    if(c == '\n')
    {
        s[i] = '\n';
        ++i;
    }

    s[i] = '\0';

    return i;
}

