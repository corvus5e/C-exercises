/*
 * Exercise 5-17. Add a field-searching capability, so sorting may bee done on fields within
 * lines, each field sorted according to an independent set of options. (The index for this book
 * was sorted with -df for the index category and -n for the page numbers.)
*/
/*TODO:
 - parse input parameters in format [start:end] [-nfrd]
 - Free getline memory
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 5000 /* max lines to be sorted */
#define MAXFIELDS 10  /* max number of fields to search on input string*/
#define ASCENDING -1
#define DESCENDING 1

#define REVERSED_FLAG  1
#define DIR_ORDER_FLAG 2
#define FOLD_FLAG      4
#define NUMBER_FLAG    8

typedef int(*cmp_func_ptr)(void*, void*);
typedef int(*char_diff_func_ptr)(int, int);

char *lineptr[MAXLINES]; /* pointers to text lines */
/* As structures are not discussed upon this chapter, let use arrays of start,count and comparator for each field */
size_t fields_start[MAXFIELDS];
size_t fields_count[MAXFIELDS];
cmp_func_ptr fields_comparators[MAXFIELDS];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void read_cmd(int argc, char *argv[], size_t fields_starts[], size_t fields_counts[], cmp_func_ptr fields_cmps[]);
//TODO: read_range()
// returns options as a bit set in a char. Reads until the end of string or unknown option
char read_options(char *options);
int get_comparator_and_order(char options, int *order, cmp_func_ptr* cmp);

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

	if (argc > 1 && argv[1][0] == '-'){
		char options = read_options(argv[1]+1/*skipping '-'*/);
		if(options == 0)
			return 1; // bad params, exit
		
		if(!get_comparator_and_order(options, &order, &comparator))
			return 1;
	}

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

void read_cmd(int argc, char *argv[], size_t fields_starts[], size_t fields_counts[], cmp_func_ptr fields_cmps[])
{
	//TODO: implement this!
	// if(read_range()){
	//		if(read_options(){
	//			i++
	//		}
	//		else{ set default options for this range }
	// }
	// else {
	//		set field range as full, 
	// 		if ( i == 0) // first read
	//			read_opts()
	//		else exit, that's all
	//}
}

char read_options(char *options_str)
{
	char flags = 0;
	char c = 0;

	while((c = *options_str++)){
		switch (c) {
			case 'n': flags |= NUMBER_FLAG; break;
			case 'f': flags |= FOLD_FLAG; break;
			case 'd': flags |= DIR_ORDER_FLAG; break;
			case 'r': flags |= REVERSED_FLAG; break;
			default: printf("Unknown option: '%c'\n", c);
					 return flags;
		}
	}

	return flags;

}

int get_comparator_and_order(char options, int *order, cmp_func_ptr* cmp)
{
	if(options > 9){ // 0 | NUMBER_FLAG | REVERSED_FLAG
		printf("Wrong combination of parameters. Can't set 'n' with 'f' or 'd'\n");
		return 0;
	}

	if(options & REVERSED_FLAG)
		*order = DESCENDING;

	if(options & NUMBER_FLAG)
		*cmp = (cmp_func_ptr)numcmp;

	if(options & FOLD_FLAG)
		*cmp = (cmp_func_ptr)str_cmp_case_insensitive;

	if(options & DIR_ORDER_FLAG)
		*cmp = (cmp_func_ptr)str_cmp_dir;

	if(options & FOLD_FLAG && options & DIR_ORDER_FLAG)
		*cmp = (cmp_func_ptr)str_cmp_dir_case_insensitive;

	return 1; // return true
}
