/*
 * Exercise 5-3. Write a pointer version of the function strcat that we showed in Chapter 2:
 * strcat(s,t) copies the string t to the end of s.
 * */

#include <stdio.h>

void my_strcat(char *s, char *t);

#define SIZE 100

int main()
{
    char s[100] = "Hello, my dear ";
    char *t = "world!";

    my_strcat(s, t); // assuming we have enough space

    printf("%s\n",s);
}

void my_strcat(char *s, char *t)
{
    while(*s) // iterate to the end of the sting (to zero)
        s++;

    while((*s++ = *t++))
        ;
}
