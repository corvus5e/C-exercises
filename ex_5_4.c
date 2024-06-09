/*
 * Exercise 5-4. Write the function strend(s,t), which returns 1 if the string t occurs at the
 * end of the string s, and zero otherwise.
 * */
//TODO: also test impl from https://stackoverflow.com/questions/33532638/strend-function-in-c-using-pointers
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> // ssize_t
#include <string.h>

#define MAX_STR_SIZE 50
#define LINE_SIZE 200
#define QUOTE_STR "\""

int strend(char *s, char *t);

/*
 * Reads a test case from standard input.
 * The test case should be a one line string in format "s" "t" expected_output
 * where "s" (with double quote signs) is a string s for strend(s, t),
 * "t" is a string s for strend(s, t)
 * and expected_outpur is 1 is or 0.
 * Function returs 0 if EOF was met, 1 otherwise
 */
int readTestCaseFromStdIn(char *s, char *t, int *expected);

int main()
{
    char s[MAX_STR_SIZE] = "";
    char t[MAX_STR_SIZE] = "";
    int expected_output = 0;

    while(readTestCaseFromStdIn(s, t, &expected_output))
    {
        printf("\"%s\" \"%s\" %d", s, t, expected_output);
        int actual = strend(s, t);
        if(actual != expected_output)
        {
            printf("\nFAILED actual = %d\n", actual);
        }
        else {
            printf(" PASSED\n"); 
        }
    }
}

/*
 * Read a string wrapped by some other string, null termination char is added to the dest
 * */
char* read_wrapped_str(char *src, char *dest, char *wrapper);
/*
 * Reads firs met disig and writes it to d and returns 1
 * If there is no digits - returns 0
 * */
int read_digit(char *str, int *d);

int readTestCaseFromStdIn(char *s, char *t, int *expected)
{
    char *line = NULL;
    size_t n = 0;
    ssize_t read_bytes = getline(&line, &n, stdin);

    int status = 0;

    if(read_bytes != -1)
    {
        char *pos = read_wrapped_str(line, s, QUOTE_STR);

        if(pos)
        {
            pos = read_wrapped_str(pos + 1, t, QUOTE_STR);

            if(pos)
            {
                if(read_digit(pos + 1, expected))
                    status = 1;
            }
        }

    }

    free(line);
    return status;
}

int read_digit(char *str, int *value)
{
    while (str && !isdigit(*str)) {
        ++str; 
    }

    if(isdigit(*str))
    {
        *value = *str - '0';
        return 1;
    }

    return 0;
}

char* read_wrapped_str(char *src, char *dest, char *wrapper)
{
    char *b = strstr(src, wrapper);
    if(!b)
        return NULL;

    char *e = strstr(b+1, wrapper);

    if(!b || !e || e <= b)
        return NULL;

    size_t len = e-b-1;

    memcpy(dest, b+1, len); 

    dest[len] = '\0';   

    return e;
}

int strend(char *s, char *t)
{
    char *bt = t;

    while(*s & *t)
    {
        if(*s++ == *t)
            ++t;        
        else
         t = bt; // TODO: need to compare with s again!
    }

    return !(*s || *t); // *s == *t
}
