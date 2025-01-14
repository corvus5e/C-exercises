#include <stdio.h>

#define MAX_LEN 100

void squeeze(char s[], char f[]);
void copy_str(char s[], char d[], int limit);

int main(int argc, char* argv[])
{
	if(argc > 2)
	{
		char str[MAX_LEN + 1];
		copy_str(argv[1], str, MAX_LEN);

		squeeze(str, argv[2]);

		printf("%s\n", str);
	}
}

void squeeze(char s[], char f[])
{
	int i, j, k;

	for(i = j = 0; s[i] != '\0'; i++)
	{
		for(k = 0; f[k] != '\0' && s[i] != f[k]; k++);

		if(f[k] == '\0') // k have reached end of line, then any filter symbol met
		{
			s[j++] = s[i];
		}
	}

	s[j] = '\0'; // trim the string if was filtered
}

void copy_str(char s[], char d[], int limit)
{
	int i;

	for(i = 0; i < limit && (d[i] = s[i]) != '\0'; i++);

	d[i] = '\0';

}
