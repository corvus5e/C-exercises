/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and exercises with pointers
 * instead of array indexing. Good possibilities include getline (Chapters 1 and 4), atoi, itoa,
 * and their variants (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
 * */

#include <stdio.h>

char* reverse(char* s);
int get_line(char *buf, int lim);
// That's all. I'm too lazy to reimplement other function from the task

int main(){
    char s[] = "This is a string!";
    printf("%s\n", s);
    printf("%s\n", reverse(s));

    char buf[30];
    while (get_line(buf, 30)) {
        printf("%s", buf); 
    }
}

char* reverse(char *s){
    char* begin = s;
    char *end = s;

    for(; *end; end++)
        ;
    --end; // handmade strlen, just want to reimplement 

    char tmp;
    for(;s < end; s++, end--){
        tmp = *s;
        *s = *end;
        *end = tmp;
    }

    return begin;
}

int get_line(char *buf, int lim){
    char *begin = buf;
    for(; lim-- > 0  && (*buf = getchar()) != '\n' && *buf != EOF; ++buf)
        ;

    if(*buf == '\n')
        ++buf;
    *buf = '\0';

    return buf - begin;
}
