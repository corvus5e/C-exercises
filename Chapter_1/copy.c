#include <stdio.h>

int main()
{

	char c = 0;

	while((c = getchar()) != EOF )
	{
		putchar(c);
	}
	printf("EOF value:\n");
	printf("%c\n", c);
}


