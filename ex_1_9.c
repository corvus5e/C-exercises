#include <stdio.h>

int main()
{
	int c = 0;
	int space_met = 0;
	int is_space = 0;

	while((c = getchar()) != EOF)
	{
		is_space = (c == ' ');

		if(!space_met || !is_space)
		{
			putchar(c);
		}

		space_met = is_space;
	}
}
