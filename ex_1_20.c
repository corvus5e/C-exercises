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
