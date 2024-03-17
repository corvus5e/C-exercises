#include <stdio.h>

int main()
{
	long blanks = 0;
	int c = 0;

	while((c = getchar()) != EOF)
	{
		if(c == '\n' || c == '\t' || c == ' ')
		{
			++blanks;
		}
	}

	printf("Blanks entered:%ld\n", blanks);
}
