#include <stdio.h>
#include <string.h>

#define MAX_LEN 4*8

void itob(int number, char str[], char base);
void itoh(int number, char str[]);
void reverse(char str[]);

int main()
{
	char str[MAX_LEN + 1];
	int number = 123;
	int base = 10;
	itob(number, str, base);
	printf("%s\n", str);
}

void reverse(char str[])
{
	char tmp = 0;

	for(int i = 0, j = strlen(str) - 1;i < j; ++i, --j)
	{
		tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}
}

void itoh(int number, char str[])
{
	int i = 0;
	int r = 0;

	do
	{
		r = number % 16;
		str[i++] = r <= 9 ? '0' + r : 'a' + (r - 10);
	}
	while((number /= 16) > 0);

	str[i] = '\0';

	reverse(str);
}

void itob(int number, char str[], char base)
{
	if(base == 16)
	{
		itoh(number, str);
		return;
	}
	else if(base <= 10)
	{
		int i = 0;

		do
		{
			str[i++] = number % base + '0';
		}
		while((number /= base) > 0);

		str[i] = '\0';

		reverse(str);
	}
}

