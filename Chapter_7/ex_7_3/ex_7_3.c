/*
 * Exercise 7-3. Revise minprintf to handle more of the other facilities of printf.
*/

/*
   Adding precision for floats, that's all
*/

#include <_ctype.h>
#include <stdio.h>
#include <sys/_types/_va_list.h>
#include <stdarg.h>
#include <stdlib.h>

#define PRECISION_NUM_LEN 5

void minprintf(char *fmt, ...);
char* read_int(char*, int*);

int main()
{
	minprintf("%s\n", "Hello, world!");
	minprintf("%s runs %d meters for %.2fsec\n", "Usain Bolt", 100, 9.58);
}

void minprintf(char *fmt, ...)
{
	int prec = -1;
	int f = -1;

	va_list ap;
	va_start(ap, fmt);

	for(char *p = fmt; *p; ++p){
		if(*p != '%' && prec < 0){
			putchar(*p);
			continue;
		}

		f = prec > 0 ? 'f' : *(++p);

		switch(f) {
		case 'd':
			printf("%d", va_arg(ap, int));
			break;
		case 'f':
			if(prec > 0){
				printf("%.*f", prec, va_arg(ap, double));
				prec = -1;
			}
			else
				printf("%f", va_arg(ap, double));
			break;
		case 's':
			for(char *sval = va_arg(ap, char*); *sval; ++sval)
				putchar(*sval);
			break;
		case '.':
			p = read_int(p, &prec);
			break;
		default:
			putchar(*p);
			break;
		}
	}

	va_end(ap);
}

char* read_int(char *p, int *val)
{
	int lim = PRECISION_NUM_LEN;
	char buf[PRECISION_NUM_LEN];
	char *b = &buf[0];

	while(--lim > 0 && isnumber(*(++p)))
		*b++ = *p;

	*b = '\0';

	*val = atoi(buf);

	return p;
}
