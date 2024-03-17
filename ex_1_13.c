#include <stdio.h>

#define MAX_WORD_LEN 10

int main()
{
	int c = 0;
	int length = 0;
	int historgam[MAX_WORD_LEN + 1];
	int max_value = 0;
	int index = 0;

	// Clear histogram values
	for(int i = 0; i <= MAX_WORD_LEN; i++)
	{
		historgam[i] = 0;
	}

	// Process input
	while((c = getchar()) != EOF)
	{
		if(c == ' ' || c == '\t' || c == '\n')
		{
			if(length > 0)
			{
				index = length - 1;

				if(length > MAX_WORD_LEN)
				{
					index = MAX_WORD_LEN;
				}

				historgam[index] += 1;
				length = 0;

				if(historgam[index] > max_value)
					max_value = historgam[index];
			}
		}
		else
		{
			++length;
		}
	}

	// Draw historgam
	while(max_value > 0)
	{
		for(index = 0; index <= MAX_WORD_LEN; index++)
		{
			if(historgam[index] >= max_value)
				printf(" # ");
			else
				printf("   ");
		}
		putchar('\n');

		max_value--;
	}

	for(index = 0; index < MAX_WORD_LEN; index++)
	{
		printf("%2d ", index + 1);
	}
		printf(">%2d", MAX_WORD_LEN);
}
