#include <stdio.h>
#include <limits.h>

#define NON_HEX_VALUE 16
#define TRUE 1
#define FALSE 0

int htoi(char s[], unsigned int* result);

char hex_char_to_int(char c);

int main()
{
	unsigned int input_value = 0;
	unsigned int result_value = 0;
	int succeed = 0;
	char str[10];

	printf("UINT_MAX: %u, 0x%x\n", UINT_MAX, UINT_MAX);

	do
	{
		printf("Enter int value: ");
		scanf("%d", &input_value);
		printf("in hex format: 0x%x\n", input_value);
		sprintf(str, "0x%x", input_value);
		succeed = htoi(str, &result_value);

		if(succeed)
		{
			printf("htoi result: %d\n", result_value);
		}
		else
			printf("Non hex\n");

	}
	while(getchar() != EOF);
}

int htoi(char s[], unsigned int* result)
{
	*result = 0;

	if(s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		s = s + 2;

	int i = 0;
	char char_value = 0;

	for(; s[i] != '\0' && i < 9; ++i) // hex int is 8 chars max + 1 chars for "\0"
	{
		char_value = hex_char_to_int(s[i]);

		if(char_value == NON_HEX_VALUE)
			return FALSE;

		*result = *result | (char_value << (4 * (7 - i)));
	}

	*result = *result >> ((8 - i) * 4);

	return TRUE;
}

char hex_char_to_int(char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';

	if(c >= 'a' && c <= 'f')
		return 10 + c - 'a';

	if(c >= 'A' && c <= 'F')
		return 10 + c - 'A';

	return NON_HEX_VALUE;
}

