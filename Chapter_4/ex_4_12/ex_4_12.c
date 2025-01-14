/*
 * Exercise 4-12. Adapt the ideas of printd to write a recursive version of itoa; that is, convert
 * an integer into a string by calling a recursive routine.
 *
 * This will be a program which test our implementation of itoa.
 * The number string will be read from input, then converted to integer via library function atoi()
 * Then this converted to int value will be converted back to string via out itoa
 * Both strings will be displayed to compare result
 * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXINPUTSTRLEN 50
#define MAXOUTPUTSTRLEN 4

void print_memory(char* str, unsigned int len)
{
    char* s = str;
    printf("Memory %p, len %d:\n", s, len);
    for(int i = 0; i < len; ++i)
    {
        char c = s[i];
        switch (c) {
            case '\0': printf("_"); break; 
            case '\n': printf("\\n"); break;
            case EOF:  printf("#"); break;
            default:   printf("%c", s[i]);
        }
    }
    printf("\n");
}

/*
 * Converts n to string and writes it to str
 * Returns number of chars written to str besides \0 at the last element
 * len is length of a buffer for str
 * If buffer is too small then returns -1, memory is unmodified 
 * */
int itoa(int n, char str[], int buf_len);

/* reads all digit chars int str until non-digit char
 * return 0 if no digit met
 * return 1 is succeeded
 */
int read_number_to_str(char str[], unsigned long buf_len);

int main()
{
    char str[MAXOUTPUTSTRLEN] = {};
    char reference[MAXINPUTSTRLEN] = {};
    printf("str       addr:%p\n", str);
    printf("reference addr:%p\n", reference);

    if(read_number_to_str(reference, MAXINPUTSTRLEN))
    {
        print_memory(str, MAXINPUTSTRLEN + MAXOUTPUTSTRLEN);
       
        int n = atoi(reference);

        if((n = itoa(n, str, MAXOUTPUTSTRLEN)) >= 0)
        {
            printf("Input str: %s\n", reference);
            printf("itoa results: %s\n", str);
            printf("chars written: %d\n", n);
        }
        else
        {
            printf("Failed to convert int to a string\n");
        }
        
        print_memory(str, MAXINPUTSTRLEN + MAXOUTPUTSTRLEN);
    }
    else
    {
        printf("Any number have been entered. Try again and enter some digits\n");
    }
}

int itoa_positive(unsigned int n, char str[], int buf_len);

int itoa(int n, char str[], int buf_len)
{
    int charsWritten = 0;

    if(n < 0 && buf_len >= 2)
    {
        charsWritten = itoa_positive(-n, str + 1, buf_len - 2);

        if(charsWritten > 0)
        {
            str[0] = '-';
            charsWritten++;
        }
    }
    else
        charsWritten = itoa_positive(n, str, buf_len - 1);

    if(charsWritten >=0 && charsWritten < buf_len)
        str[charsWritten] = '\0';

    return charsWritten;
}

int itoa_positive(unsigned int n, char str[], int buf_len)
{
    if(buf_len <= 0)
        return -1;

    int pos = 0;

    if(n >= 10)
        pos = itoa_positive(n / 10, str, buf_len-1);

    if(pos >= 0 && pos < buf_len)
    {
        str[pos++] = '0' + n % 10;
    }

    return pos;
}

int read_number_to_str(char str[], unsigned long buf_len)
{
    int c, i = 0;

    if(buf_len >= 2 && ((c = getchar()) == '-' || c == '+' || isdigit(c)))
        str[i++] = c;

    for(;i < buf_len-1 && isdigit(c = getchar()); ++i)
        str[i] = c;

    str[i]  = '\0';

    return i > 0;
}

