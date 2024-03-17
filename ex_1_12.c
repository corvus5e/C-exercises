#include <stdio.h>

#define IN 1
#define OUT 0
#define NEW_LINE -1

int main()
{
	int c = 0;
	int state = NEW_LINE;

	while((c = getchar()) != EOF)
	{
		if(c != ' ' && c != '\t' && c!= '\n')
		{
			if(state == OUT)
				putchar('\n');

			putchar(c);

			state = IN;
		}
		else
		{
			if(state == IN)
				state = OUT;

			if(c == '\n' && state != NEW_LINE)
			{
				state = NEW_LINE;
				putchar('\n');
			}
		}
	}
}
