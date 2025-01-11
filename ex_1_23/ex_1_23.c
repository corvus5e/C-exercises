#include <stdio.h>

int main()
{
	int comment_type[2] = {0, 0};
	int opened_quote = 0;
	char curr = 0;
	char next = 0;

	while((curr = getchar()) != EOF)
	{

		if(comment_type[0]) // in the comment secsion
		{
			if(((comment_type[1] == '/') && (curr == '\n')) || // one line coment finish
			    (comment_type[1] == '*' && (curr == comment_type[1]) && (next = getchar() == '/') )) // multiline comment finish
			{
				comment_type[0] = comment_type[1] = 0;

				if(curr == '\n') // print new line after one line comments
					putchar(curr);
			}
		}
		else
		{
			if(opened_quote)
			{
				if(curr == '\\') // coud be escaped " or ' insode string literal
				{
					putchar(curr);
					curr = getchar();
				}
				else if(curr == opened_quote) // the closing quote is met
				{
					opened_quote = 0;
				}
			}
			else
			{
				if(curr == '"' || curr == '\'')
				{
					opened_quote = curr;
				}
				else if(curr == '/')
				{
					if((next = getchar()) == '*' || next == '/')
					{
						comment_type[0] = curr;
						comment_type[1] = next;
						continue;
					}
					else
					{
						putchar(curr);
						curr = next;
					}
				}
			}

			putchar(curr);
		}
	}
}
