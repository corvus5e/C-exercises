/*
 * Exercise 1-20. Write a program detab that replaces tabs in the input with the proper number
 * of blanks to space to the next tab stop. Assume a fixed set of tab stops, say every n columns.
 * Should n be a variable or a symbolic parameter?
*/

/*
 * Make ex_1_20.c and then compare:
 * cat data/ex_1_20_input.txt
 * with
 * ex_1_20.out < data/ex_1_20_input.txt
*/

#include <stdio.h>

#define TAB_LEN 15

int main()
{
    int column = 0;
    for(int c; (c = getchar()) != EOF;){ 
        if(c == '\t'){
            int len = TAB_LEN - (column % TAB_LEN);
            for(int i = 0; i < len; ++i){
                putchar(' ');
                ++column;
            }
        }
        else{
            putchar(c);
            ++column;
        }

        if(c == '\n')
            column = 0;
    }
}
