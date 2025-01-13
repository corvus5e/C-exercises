/*
 * Exercise 5-14. Modify the sort program to handle a -r flag, which indicates sorting in reverse
 * (decreasing) order. Be sure that -r works with -n.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000 /* max lines to be sorted */
#define ASCENDING -1
#define DESCENDING 1
typedef int(*cmp_func_ptr)(void*, void*);

char *lineptr[MAXLINES]; /* pointers to text lines */
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void quick_sort(void *lineptr[], int left, int right, int order, cmp_func_ptr cmp);
int numcmp(char *, char *);
void read_options(char *options, int *order, cmp_func_ptr* cmp);

/* sort input lines */
int main(int argc, char *argv[]){
	int nlines; /* number of input lines read */
	cmp_func_ptr comparator = (cmp_func_ptr)strcmp;
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
		if ((*comp)(v[i], v[left]) * order > 0 /*order doesn't match*/)
			swap(v, ++last, i);
	swap(v, left, last);
	quick_sort(v, left, last-1, order, comp);
	quick_sort(v, last+1, right, order, comp);
}

/* numcmp: compare s1 and s2 numerically */
int numcmp(char *s1, char *s2)
{
	return atof(s1) - atof(s2);
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

	while(len--){
		printf("LEN=%lu\n", len);
		switch (*options++) {
			case 'n': *cmp = (cmp_func_ptr)numcmp;
				break;
			case 'r': *order = DESCENDING;
				break;
		}
	}
}
