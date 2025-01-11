/*
 * Exercise 5-5.
 * Write versions of the library functions strncpy, strncat, and strncmp, which
 * operate on at most the first n characters of their argument strings. For example,
 * strncpy(s,t,n) copies at most n characters of t to s.
 * */

#include <stdio.h>

void strncopy(char *source, char *destination, int n);
char* strnconcat(char *source, char *destination, int n);
int strncompare(char *l, char *r, int n);

int main(){
    char *s = "Very big string here!";
    char t[50];
    size_t n = 4;
    strncopy(s, t, n);
    t[4] = '\0';
    printf("%s\n", t);

    strnconcat(" And even more_junk", t, 14);
    printf("%s\n", t);
    printf("%d\n", strncompare("R2", "R2xx", 22));
}

void strncopy(char *s, char *t, int n){
    while((*t++ = *s++) && --n)
        ;
    while (n-- > 0) {
        *t++ = '\0'; 
    }
}

char* strnconcat(char *source, char *destination, int n){
    char *begin = destination;

    while (*destination)
        destination++;

    while((*destination++ = *source++) && --n)
        ;

    *destination = '\0'; // strncat puts null-character at eh end, so will we

    return  begin;
}

int strncompare(char *l, char *r, int n){
    for(;n-- && *l && (*l == *r); ++r, ++l)
        ; 
    if(!*l || n < 0)
        return 0;
    return *l - *r;
}
