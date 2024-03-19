/*
 * Exercise 4-1. Write the function strindex(s,t) which returns the position of the rightmost
 * occurrence of t in s, or -1 if there is none.
 * */

#include <stdio.h>
#include <string.h>

/*strindex: return index of t in s, -1 if none*/
int strindex(char s[], char t[]);

int main()
{
    char* s = "sold bold ld";
    char* t = "old";

    int i = strindex(s, t);

    if(i >= 0)
        printf("Rightmost index of \"%s\" in \"%s\" is %d\n", t, s, i);
    else
        printf("The \"%s\" is not found in \"%s\" string\n", t, s);
}

int strindex(char s[], char t[])
{
    int i, j, k;
    unsigned long sLen = strlen(s); // assuming there is a chance that compiler will cache len for a literal string
    unsigned long tLen = strlen(t);

    for(i = sLen - 1; i >= tLen - 1; --i)
    {
        for(j = i, k = tLen - 1; k >= 0 && s[j] == t[k]; --j, --k)
            ;

        if(k == -1)
            return j + 1;
    }

    return -1;

    /*
    int r = -1;
    for(i = 0; s[i] != '\0'; ++i)
    {
        for(j = i, k = 0; t[k] != '\0' && s[j] == t[k]; ++j, ++k)
            ;

        if(k > 0 && t[k] == '\0')
            r = i;
    }

    return r;*/
}
