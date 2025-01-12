/*
 * Exercise 5-15. Add the option -f to fold upper and lower case together, so that case
 * distinctions are not made during sorting; for example, a and A compare equal.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 5000 /* max lines to be sorted */
#define ASCENDING -1
#define DESCENDING 1
typedef int(*cmp_func_ptr)(void*, void*);

char *lineptr[MAXLINES]; /* pointers to text lines */
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void quick_sort(void *lineptr[], int left, int right, int order, cmp_func_ptr cmp);

/* Comparators for sorting*/
int numcmp(char *, char *);
int str_cmp(char*, char *);
int str_cmp_case_insensitive(char*, char*);

void read_options(char *options, int *order, cmp_func_ptr* cmp);

/* sort input lines */
int main(int argc, char *argv[]){
	int nlines; /* number of input lines read */
	cmp_func_ptr comparator = (cmp_func_ptr)str_cmp;
	int order = ASCENDING;

	if (argc > 1 && argv[1][0] == '-')
		read_options(argv[1]+1/*skipping '-'*/, &order, &comparator);
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
		if ((*comp)(v[i], v[left]) == order)
			swap(v, ++last, i);
	swap(v, left, last);
	quick_sort(v, left, last-1, order, comp);
	quick_sort(v, last+1, right, order, comp);
}

/* numcmp: compare s1 and s2 numerically */
int numcmp(char *s1, char *s2)
{
	double v1, v2;
	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}
/*wrapper for standard strcmp. To return only -1 or 0 or +1*/
int str_cmp(char* l, char *r)
{
	int result = strcmp(l, r);
	if(result < 0)
		return -1;
	else if (result > 0)
		return 1;
	return 0;
}

int str_cmp_case_insensitive(char* l, char* r)
{
	int diff = 0;
	for(;((diff = (tolower(*l) - tolower(*r))) == 0) && l; ++l, ++r)
		;

	return diff > 0 ? 1 : -1;
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

void read_options(char *options, int *order, cmp_func_ptr* cmp)
{
	size_t len = strlen(options);
	*cmp = (cmp_func_ptr)str_cmp;
	*order = ASCENDING;

	while(len--){
		//printf("LEN=%lu\n", len);
		switch (*options++) {
			case 'n': *cmp = (cmp_func_ptr)numcmp;
				break;
			case 'f': *cmp = (cmp_func_ptr)str_cmp_case_insensitive;
				break;
			case 'r': *order = DESCENDING;
				break;
		}
	}
}
