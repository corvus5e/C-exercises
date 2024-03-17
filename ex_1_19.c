#include <stdio.h>

#define MAX_STR_LEN 100

int get_line(char* buffer, int limit);
void reverse(char* str, int size);

int main()
{
	char line[MAX_STR_LEN + 1];
	int read_chars = 0;

	while((read_chars = get_line(line, MAX_STR_LEN)) > 0)
	{
		printf("Read chars: %d \n", read_chars);
		reverse(line, read_chars - 1); // lets skip last \n character
		printf("%s\n", line);
	}

	return 0;
}


int get_line(char* buffer, int limit)
{
	char c = 0;
	int i = 0;

	while((c = getchar()) != EOF && c != '\n' && i < limit - 1)
	{
		buffer[i++] = c;
	}

	if(c == '\n')
	{
		buffer[i++] = c;
	}

	buffer[i] = 0; // end line is not part of str size

	return i;
}

void reverse(char* str, int size)
{
	// it's a caller responsibility to make sure
	// that str fits size
	int mid = size / 2;
	char tmp = 0;
	--size; // last index

	for(int i = 0; i < mid; i++)
	{
		tmp = str[size - i];
		str[size - i] = str[i];
		str[i] = tmp;
	}
}

