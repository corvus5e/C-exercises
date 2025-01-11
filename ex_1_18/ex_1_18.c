#include <stdio.h>

#define FALSE 0
#define TRUE 1

int main()
{
	int c = 0;
	char line_blank = TRUE;
	char prev_c_blank = TRUE;

	while((c = getchar()) != EOF)
	{
		if(c == ' ' || c == '\t')
		{
			if(!prev_c_blank && !line_blank)
			{
				putchar(c);
			}
			prev_c_blank = TRUE;
		}
		else
		{
			if(!line_blank || c != '\n')
			{
				putchar(c);
				line_blank = FALSE;
				prev_c_blank = FALSE;
			}

			if(c == '\n')
			{
				line_blank = TRUE;
			}
		}
	}

	return 0;
}
