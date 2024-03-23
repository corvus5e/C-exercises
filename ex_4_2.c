/*
 * Exercise 4-2. Extend atof to handle scientific notation of the form 123.45e-6
 * where a floating-point number may be followed by e or E and an optionally signed exponent.
 */

#include <stdio.h>
#include <ctype.h>
#include <math.h>

/* atof: convert string s to double */
double atof(char s[]);

/*atof_ex: converts string to a double and
 * puts into `non_digit_pos` the index of the firts non digit char
 * after the float number
 * */
double atof_ex(char s[], int* non_digit_pos);

int main()
{
    char* str = "-123.456";
    printf("The converted float value is %f\n", atof(str));

    char* str2 = "-123.45e+1.5";
    printf("The converted value scientific float value is %f\n", atof(str2));

}

double atof(char s[])
{
    double num;
    double exp; // according to the task the exponent can be float
    int i = 0;

    num = atof_ex(s, &i);

    if(s[i] == 'e' || s[i] == 'E')
    {
        exp = atof_ex(s + i+1, &i);
        num *= pow(10, exp);
    }

    return num;
}

double atof_ex(char s[], int* non_digit_pos)
{
    double val;
    double power = 1.0;
    int i, sign;
    for (i = 0; isspace(s[i]); i++) /* skip white space */
        ;

    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
        i++;

    for (val = 0.0; isdigit(s[i]); i++)
        val = 10.0 * val + (s[i] - '0');

    if (s[i] == '.')
        i++;

    for (power = 1.0; isdigit(s[i]); i++)
    {
        val = 10.0 * val + (s[i] - '0');
        power *= 10;
    }

    *non_digit_pos = i;

    return sign * val / power;
}
