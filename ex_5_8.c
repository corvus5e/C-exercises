/*
 * Exercise 5-8. There is no error checking in day_of_year or month_day. Remedy this defect.
 * */

#include <stdio.h>

int day_of_year(int year, int month, int day);
void month_day(int year, int year_day, int *month, int *day);

int main(){
    int input_year = 2024;
    int input_month = 7;
    int input_day   = 8;

    int year_day = day_of_year(input_year, input_month, input_day);
    printf("%d\n", year_day);

    int month, day;
    month_day(input_year, year_day, &month, &day);
    printf("%d, %d\n", month, day);
}

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day){
    int leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;  

    if(month < 0 || month > 13 || day < 1 || daytab[leap][month] < day)
        return -1;

    for(;--month > 0; day += daytab[leap][month])
        ;

    return day;
}

void month_day(int year, int year_day, int *month, int *day){
    if(year < 0){
        *month = -1;
        *day = -1;
        return;
    }

    int leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;   

    if(year_day < 1 || year_day > 365 + leap){
        *month = -1;
        *day = -1;
        return;
    }

    *month = 0;
    *day = year_day;
    for(;(*day > daytab[leap][++(*month)]); *day -= daytab[leap][*month])
        ;         
}


