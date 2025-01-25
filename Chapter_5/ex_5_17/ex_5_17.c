/*
 * Exercise 5-17. Add a field-searching capability, so sorting may bee done on fields within
 * lines, each field sorted according to an independent set of options. (The index for this book
 * was sorted with -df for the index category and -n for the page numbers.)
*/

/*TODO: Make -d option also work with -n, so 5_11 could be parsed as a number 511*/

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

typedef struct Field {
	size_t start;
	size_t count;
	char order;
	cmp_func_ptr comparator;
} Field;

/* As structures are not discussed upon this chapter, let use arrays of start,count and comparator for each field */
Field fields[MAXFIELDS];
size_t fields_number = 0;

int readlines(char *lineptr[], int nlines);
void freelines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int read_cmd(int argc, char *argv[], Field fields[]);
void read_field_range(char *str, size_t *start, size_t *count);
// returns options as a bit set in a char. Reads until the end of string or unknown option
char read_options(char *options);
int get_comparator_and_order(char options, char *order, cmp_func_ptr* cmp);

void quick_sort(void *lineptr[], int left, int right, cmp_func_ptr cmp);
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
int case_insensitive_char_diff(int l, int r) { return tolower(l) - tolower(r);}

// Comparator that applies comparators for each field
int comparator(char*, char*);


/* sort input lines */
int main(int argc, char *argv[])
{
	if(argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-h") == 0)){
		printf("Usage: sort [start:count] [-nfdr]\n");
		printf("[start:count] is a range for field comparison. If no range, the whole string compared.\n\
If no options, simple strcmp is used.\n\
More than one range could be provided. If no count provided, the comparison starts from start till the \
end of string.\n\n\
Example: sort 5:10 -df 20:10 -n < input.txt\n\
This compares strings by two fields: first from position 5 and length 10 using flags 'df',\n\
and the second field - from position 20 and length 10 using 'n' flag to compare number-wise");
		return 0;
	}

	fields_number = read_cmd(argc, argv, fields);

	if(fields_number <= 0){
		printf("Bad parameters\n");
		return 1;
	}

	int nlines; /* number of input lines read */

	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		quick_sort((void**) lineptr, 0, nlines-1, (cmp_func_ptr)comparator);
		writelines(lineptr, nlines);
		freelines(lineptr, nlines);
		return 0;
	} else {
		printf("input too big to sort\n");
		return 1;
	}
}

/* quick_sort: sort v[left]...v[right] into increasing order */
void quick_sort(void *v[], int left, int right, cmp_func_ptr comp){
	int i, last;
	void swap(void *v[], int, int);
	if (left >= right) /* do nothing if array contains */
		return; /* fewer than two elements */
	swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp)(v[i], v[left]) > 0) /*Order doesn't match*/
			swap(v, ++last, i);
	swap(v, left, last);
	quick_sort(v, left, last-1, comp);
	quick_sort(v, last+1, right, comp);
}

int isdir(int ch)
{
	return isdigit(ch) || isalpha(ch) || isspace(ch);
}

int comparator(char* left, char* right)
{
	// Make it simple for now, no dynamic allocation for an actual size, hope 100 is enough
	static char left_field[100];
	static char right_field[100];

	int res = 0;

	for(int i = 0; res == 0 && i < fields_number; ++i)
	{
		if(fields[i].count > 0) //Copy fields to buffers
		{
			strncpy(left_field, left + fields[i].start, fields[i].count);
			strncpy(right_field, right + fields[i].start, fields[i].count);

			left_field[fields[i].count] = '\0';
			right_field[fields[i].count] = '\0';

			res = fields[i].comparator(left_field, right_field) * fields[i].order;
		}
		else // Compare from the start to the end of str if count is 0
			res = fields[i].comparator(left + fields[i].start, right + fields[i].start) * fields[i].order;
	}

	return res;
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

void freelines(char *lineptr[], int n)
{
	for(int i = 0; i < n; ++i){
		free(lineptr[i]);
	}
}

void writelines(char *lineptr[], int nlines)
{
	while(nlines-- > 0 )
		printf("%s\n", *lineptr++);
}

int read_cmd(int argc, char *argv[], Field fields[])
{
	//WARNING: With parameters like: "./sort 0: < input.txt" this functions will fall into an endless loop.
	// Fix later!

	size_t start = 0, count = 0;
	size_t n = 0;

	// Add default params for the first range
	fields[0].start = 0;
	fields[0].count = 0; // 0 is a code for the whole string
	get_comparator_and_order(0/* default options*/, &fields[0].order, &fields[0].comparator);

	for(int i = 1; i < argc; i+=2, ++n)
	{
		read_field_range(argv[i], &start, &count);

		char options = 0;

		fields[n].start = start;
		fields[n].count = count;
		get_comparator_and_order(options, &fields[n].order, &fields[n].comparator); // put default options 
		
		//TODO: check range. If intersects - exit with error;

		if(i == 1 && start == 0 && count == 0) // there was no range, so try to read options
			--i; 

		if(i+1 < argc)
			options  = read_options(argv[i+1]);

		if(options >= 0){
			if(get_comparator_and_order(options, &fields[n].order, &fields[n].comparator))
				get_comparator_and_order(options, &fields[n].order, &fields[n].comparator); // update options if correct one were passed
			else
				return 0; // wrong options passed
		}
		else // Maybe it was range, try again
			i--;
	}

	return n == 0 ? 1 : n;
}

void read_field_range(char *str, size_t *start, size_t *count)
{
	char *pos = strchr(str, ':');
	*start = atoi(str);
	if(pos)
		*count = atoi(++pos);
	else
		*count = 0;
}

char read_options(char *options_str)
{
	if(*options_str != '-')
		return -1;

	char flags = 0;
	char c = 0;

	while((c = *(++options_str))){
		switch (c) {
			case 'n': flags |= NUMBER_FLAG; break;
			case 'f': flags |= FOLD_FLAG; break;
			case 'd': flags |= DIR_ORDER_FLAG; break;
			case 'r': flags |= REVERSED_FLAG; break;
			default: printf("Unknown option: '%c'\n", c);
					 return -1;
		}
	}

	return flags;
}

int get_comparator_and_order(char options, char *order, cmp_func_ptr* cmp)
{
	if(options > 9){ //9 == (0 | NUMBER_FLAG | REVERSED_FLAG)
		printf("Wrong combination of parameters. Can't set 'n' with 'f' or 'd'\n");
		return 0;
	}

	*order = ASCENDING;
	*cmp = (cmp_func_ptr)strcmp;

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
