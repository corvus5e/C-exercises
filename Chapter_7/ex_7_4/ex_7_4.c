/*
 * Exercise 7-4. Write a private version of scanf analogous to
 * minprintf from the previous section.
*/

#include <_ctype.h>
#include <_stdio.h>
#include <stdio.h>
#include <sys/_types/_ssize_t.h>
#include <sys/_types/_va_list.h>
#include <stdarg.h>
#include <stdlib.h>

struct Date {
	int day;
	int month;
	int year;
};

struct Record {
	struct Date date;
	float speed;
};

int minscanf(char *fmt, ...);
char* read_int(char*, int*);
char* read_float(char*, float*);

int main()
{
	struct Record r;
	printf("Please, enter the new world 100m speed record:\n");
	printf("Input format:day/month/year - speed\n");
	minscanf("%d/%d/%d-%f", &r.date.day, &r.date.month, &r.date.year, &r.speed);
	printf("Done!\nNew record for 100m is %f, set on %d/%d/%d\n", r.speed, r.date.day, r.date.month, r.date.year);
}

int minscanf(char *fmt, ...)
{
	char *linep = NULL;
	size_t linecapp;

	if(getline(&linep, &linecapp, stdin) < 0){
		free(linep);
		return -1;
	}

	va_list ap;
	va_start(ap, fmt);

	char *p = linep;
	char *f = fmt;
	while(*p && *f)
	{
		for(;isspace(*p);++p);
		for(;isspace(*f);++f);

		if(*f != '%'){
			if(*f++ != *p++){
				printf("p=%c; f=%c\n", *p, *f);
				printf("Wrong format\n");
				break;
			}
		}
		else {
			switch (*(++f)) {
			case 'd':
				p = read_int(p, va_arg(ap, int*));
				break;
			case 'f':
				p = read_float(p, va_arg(ap, float*));
				break;
			default:
				printf("Unsupported format!\n");
			}
			++f;
		}
	}

	free(linep);

	va_end(ap);

	return 0;
}

char* read_int(char *str, int *val)
{
	char *endp = NULL;

	long value = strtol(str, &endp, 10);

	if(!endp || endp == str){
		printf("Failed to read int\n");
		return str;
	}

	*val = value;

	return endp;
}

char* read_float(char *str, float *val)
{
	int tmp = 0;
	char *end = read_int(str, &tmp);
	*val = tmp;
	return end;
}
