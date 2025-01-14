/*
 * Exercise 4-13. Write a recursive version of the function reverse(s), which reverses the
 * string s in place.
 * */

#include <stdio.h>
#include <string.h>

/*
 * Reverses string, len should be valid string length
 */
void reverse(char str[], int start_index, int len);

void test_reverse(char str[], int len);

int main()
{
    char str_1[] = "";
    char str_2[] = "a";
    char str_3[] = "ab";
    char str_4[] = "abcdefgh";
    
    test_reverse(str_1, strlen(str_1));
    test_reverse(str_2, strlen(str_2));
    test_reverse(str_3, strlen(str_3));
    test_reverse(str_4, strlen(str_4));
}

void reverse(char str[], int index, int len)
{
    int r = len - 1 - index;

    if(index < r) // this should be faster then initiall version:  if(index < len/2)
    {
        char tmp = str[index];
        str[index] = str[r];
        str[r] = tmp;

        reverse(str, ++index, len);
    }
}

void test_reverse(char str[], int len)
{
    printf("str:         %s\n", str);
    reverse(str, 0, len);
    printf("str reversed %s\n", str);
}

