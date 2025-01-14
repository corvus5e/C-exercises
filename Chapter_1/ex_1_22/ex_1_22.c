#include <stdio.h>

#define MAX_LEN 10

void clear_buf(int buf[], int begin, int count);
void write_to_buf(int buf[], int begin, int count);
void print_buf(int buf[], int begin, int count);
int fold_and_print_buf(int buf[], int begin, int count);

int main()
{
	int buf[MAX_LEN];
	int c = 0;
	int saved_chars = 0;
	int last_blank_pos = 0;
	int left_chars_on_line = 0;

	clear_buf(buf, 0, MAX_LEN);

	while((c = getchar()) != EOF)
	{
		buf[saved_chars] = c;
		saved_chars++;

		if(saved_chars < (MAX_LEN - left_chars_on_line))
		{

			if(c == ' ' || c == '\t')
			{
				last_blank_pos = saved_chars - 1;
			}


			if(c == '\n')
			{
				print_buf(buf, 0, saved_chars);
				saved_chars = 0;
				last_blank_pos = 0;
			}
		}
		else
		{
			if(last_blank_pos < 1.0/2 * MAX_LEN)
			{
				last_blank_pos = MAX_LEN - 1; // not fold to close to the beginning of the line
			}

			print_buf(buf, 0, last_blank_pos + 1);
			putchar('\n');

			left_chars_on_line = MAX_LEN - last_blank_pos - 1;

			if(left_chars_on_line > 0)
				print_buf(buf, last_blank_pos + 1, left_chars_on_line);

			last_blank_pos = 0;
			saved_chars = 0;
		}
	}
}

void clear_buf(int buf[], int begin, int count)
{
	while((count--) > 0)
	{
		buf[begin++] = 0;
	}
}

void print_buf(int buf[], int begin, int count)
{
	while((count--) > 0)
	{
		putchar(buf[begin]);
		buf[begin] = 0;
		begin++;
	}
}
