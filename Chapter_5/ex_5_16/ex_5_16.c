/*
 * Exercise 5-16. Add the -d (``directory order'') option, which makes comparisons only on
 * letters, numbers and blanks. Make sure it works in conjunction with -f.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 5000 /* max lines to be sorted */
#define ASCENDING -1
#define DESCENDING 1


#define DIR_ORDER_FLAG 1
#define FOLD_FLAG 2
#define NUMBER_FLAG 4

typedef int(*cmp_func_ptr)(void*, void*);
typedef int(*char_diff_func_ptr)(int, int);

char *lineptr[MAXLINES]; /* pointers to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int read_options(char *options, int *order, cmp_func_ptr* cmp);

void quick_sort(void *lineptr[], int left, int right, int order, cmp_func_ptr cmp);
/*Returns true if ch is either letter, number or blank, false otherwise*/
int isdir(int ch);

/* Comparators for sorting.
 * _dir suffix ("directory order") means that only letters, numbers and blanks are compared*/
int numcmp(char *, char *);
int str_cmp_dir(char*, char *);
int str_cmp_dir_case_insensitive(char*, char*);
int str_cmp_case_insensitive(char*, char*);

int str_cmp_dir_general(char*, char*, char_diff_func_ptr);
int simple_char_diff(int l, int r) {return l - r;}
int case_insensitive_char_diff(int l, int r) { return tolower(l) - tolower(r);};


/* sort input lines */
int main(int argc, char *argv[])
{
	int nlines; /* number of input lines read */

	cmp_func_ptr comparator = (cmp_func_ptr)strcmp;
	int order = ASCENDING;

	if (argc > 1 && argv[1][0] == '-')
		if(!read_options(argv[1]+1/*skipping '-'*/, &order, &comparator))
			return 1; // bad params, exit

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		quick_sort((void**) lineptr, 0, nlines-1, order, comparator);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("input too big to sort\n");
		return 1;
	}
}

/* quick_sort: sort v[left]...v[right] into increasing order */
void quick_sort(void *v[], int left, int right, int order, cmp_func_ptr comp){
	int i, last;
	void swap(void *v[], int, int);
	if (left >= right) /* do nothing if array contains */
		return; /* fewer than two elements */
	swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp)(v[i], v[left]) * order > 0/*Order doesn't match*/)
			swap(v, ++last, i);
	swap(v, left, last);
	quick_sort(v, left, last-1, order, comp);
	quick_sort(v, last+1, right, order, comp);
}

int isdir(int ch)
{
	return isdigit(ch) || isalpha(ch) || isspace(ch);
}

/* numcmp: compare s1 and s2 numerically */
int numcmp(char *s1, char *s2)
{
	return atof(s1) - atof(s2);
}

int str_cmp_dir_general(char* l, char *r, char_diff_func_ptr diff_func)
{
	int diff = 0;

	do
	{
		for(;*l && !isdir(*l);++l)
			;

		for(;*r && !isdir(*r);++r)
			;

		diff = diff_func(*l, *r);
	}
	while(!diff && *l++ && *r++);

	return diff;
}

int str_cmp_dir(char* l, char* r)
{
	return str_cmp_dir_general(l, r, simple_char_diff);
}

int str_cmp_dir_case_insensitive(char* l, char* r)
{
	return str_cmp_dir_general(l, r, case_insensitive_char_diff);
}

int str_cmp_case_insensitive(char* l, char* r)
{
	int diff = 0;

	for(;((diff = (tolower(*l) - tolower(*r))) == 0) && *l; ++l, ++r)
		;

	return diff;
}

void swap(void *v[], int i, int j)
{
	void *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

int readlines(char *lineptr[], int nlines)
{
	ssize_t nread = 0;
	size_t len = 0;
	int read_lines_num = 0;

	while((nread = getline(lineptr, &len, stdin)) != -1 && read_lines_num++ < nlines){
		(*lineptr)[nread - 1] = '\0';
		lineptr++;
	}

	return read_lines_num;
}


void writelines(char *lineptr[], int nlines)
{
	while(nlines-- > 0 )
		printf("%s\n", *lineptr++);
}

int read_options(char *options, int *order, cmp_func_ptr* cmp)
{
	size_t len = strlen(options);

	char flags = 0;

	while(len--){
		switch (*options++) {
			case 'n': flags |= NUMBER_FLAG; break;
			case 'f': flags |= FOLD_FLAG; break;
			case 'd': flags |= DIR_ORDER_FLAG; break;
			case 'r': *order = DESCENDING; break;
		}
	}

	if(flags > 4){ // NUMBER_FLAG is 4, so if >4 than some other flags were set
		printf("Wrong combination of parameters. Can't use 'n' with 'd' and 'f'.\n");
		return 0;
	}

	if(flags & NUMBER_FLAG)
		*cmp = (cmp_func_ptr)numcmp;

	if(flags & FOLD_FLAG)
		*cmp = (cmp_func_ptr)str_cmp_case_insensitive;

	if(flags & DIR_ORDER_FLAG)
		*cmp = (cmp_func_ptr)str_cmp_dir;

	if(flags & FOLD_FLAG && flags & DIR_ORDER_FLAG)
		*cmp = (cmp_func_ptr)str_cmp_dir_case_insensitive;

	return 1; // return true
}
