/*
  * Exercise 6-2. Write a program that reads a C program and prints in alphabetical order each
  * group of variable names that are identical in the first 6 characters, but different somewhere
  * thereafter. Don't count words within strings and comments. Make 6 a parameter that can be
  * set from the command line.
*/

/*NOTE:
  * Variables names of struct types are not parsed by this program.
  * Struct members are recognized as variables names.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct tdata {
	char *word;
	struct tnode *group_root;
};

struct tnode {
	struct tnode *left;
	struct tnode *right;
	struct tdata *data;
};

#define MAXWORD 100

struct tnode *talloc();
void tfree(struct tnode *p);

struct tnode *addtree(struct tnode *p, char *word, struct tnode *group_root, int prefix_n);
void treeprint(struct tnode *root);
void free_tree(struct tnode *p);
void print_tnode(struct tnode *p);

int getword(char *, int);
int get_decl_name(char *, int);
char *binsearch(char *, char *[], int);
int strcmp_prefix(const  char*, const char*, int*);

int getch(void);
void ungetch(int);

/*Skips stdin untill pattern or EOF is met.
  Returns 1 is patters is met. 0 if EOF.
  Next stdin read would be just after the pattern.
  */
int skip_stdin_till(const char* pattern);

/* count C keywords */
int main(int argc, char* argv[])
{
	char *types[] = {"char", "double", "float", "int", "long", "short", "unsigned", "void"};
	size_t types_count = (sizeof types/ sizeof(types[0]));

	int group_prefix_count;
	if(argc <= 1 || (group_prefix_count = atoi(argv[1])) <= 0)
		group_prefix_count = 3;

	char word[MAXWORD];
	char declaration[MAXWORD];
	struct tnode *root = NULL;

	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]) && binsearch(word, types, types_count) != NULL)
			if(get_decl_name(declaration, MAXWORD) != EOF &&
				(getword(word, MAXWORD) != EOF && word[0] != '('))
				root = addtree(root, declaration, NULL, group_prefix_count);

	treeprint(root);

	free_tree(root); // free memory
	tfree(root);

	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
char *binsearch(char *word, char *tab[], int n)
{
	int cond;
	char **low = &tab[0];
	char **high = &tab[n];
	char **mid;

	while (low < high)
	{
		mid = low + (high - low) / 2;
		if ((cond = strcmp(word, *mid)) < 0)
			high = mid;
		else if (cond > 0)
			low = mid + 1;
		else
			return *mid;
	}

	return NULL;
}

int strcmp_prefix(const  char *l, const char *r, int *same_prefix_count)
{
	*same_prefix_count = 0;

	while(*l && (*l == *r)){
		++l;
		++r;
		++(*same_prefix_count);
	}

	return *l - *r;
}

struct tnode *addtree(struct tnode *p, char *word, struct tnode* group_root, int prefix_n)
{
	if(p == NULL){
		if(!(p = talloc())){
			printf("Error: talloc failed\n");
			return p;
		}

		p->data->word = strdup(word);
		p->data->group_root = group_root ? group_root : p;
		p->left = p->right = NULL;

		return p;
	}

	int cond;
	int common_prefix_count;

	if((cond = strcmp_prefix(word, p->data->word, &common_prefix_count)) == 0)
		return p;

	if(!group_root && common_prefix_count >= prefix_n)
		group_root = p;

	if(cond < 0)
		p->left = addtree(p->left, word, group_root, prefix_n);
	else
		p->right = addtree(p->right, word, group_root, prefix_n);

	return p;
}

struct tnode *talloc()
{
	struct tnode *p = (struct tnode*)malloc(sizeof(struct tnode));

	if(p != NULL)
		p->data = (struct tdata*)malloc(sizeof(struct tdata));

	return p;
}

void treeprint(struct tnode *p)
{
	if(p != NULL){
		treeprint(p->left);
		print_tnode(p);
		treeprint(p->right);
	}
}

void free_tree(struct tnode *p)
{
	if(p != NULL){
		free_tree(p->left);
		tfree(p->left);

		free_tree(p->right);
		tfree(p->right);
	}
}

void print_tnode(struct tnode *p)
{
	static struct tnode *last_printed_group = NULL;

	if(p && p->data && p->data->word){
		if(last_printed_group == NULL || last_printed_group != p->data->group_root){
			last_printed_group = p->data->group_root;
			printf("\n");
		}
		printf("%s\n", p->data->word);
	}
	else
		printf("Error in print_tnode: attempt to print NULL");
}

void tfree(struct tnode *p)
{
	if(p == NULL)
		return;

	if(p->data){
		if(p->data->word)
			free(p->data->word);
		free(p->data);
	}

	free(p);
}


/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	char *w = word;
	*w = '\0'; // consider the buffer as empty at start
	int c = 0;

	while(c != EOF && !word[0]){
		while (isspace(c = getch()))
			;

		if(c == EOF)
			return EOF;

		if(c == '/'){ // possible comment section
			if((c = getch()) == '/'){ // this is line comment
				skip_stdin_till("\n");
				continue;
			}
			else if(c == '*'){ // this is a block comment
				skip_stdin_till("*/");
				continue;
			}
		}
		else if(c == '#'){ // this is a preprocessor 
			skip_stdin_till("\n");
			continue;
		}
		else if (c == '"') { // this is a string literal begin
			skip_stdin_till("\"");
			continue;
        	}

		if (c != EOF)
			*w++ = c;

		if (!isalpha(c) && c != '_') {
			*w = '\0';
			return c;
		}

		for ( ; --lim > 0; w++)
			if (!isalnum(*w = getch()) && *w != '_') {
				ungetch(*w);
				break;
			}
	}

	*w = '\0';

	return word[0];
}

int get_decl_name(char *word, int lim)
{
	int c = 0;

	if((c = getword(word, lim)) != EOF)
		if(c == '*') // this is a pointer
			return getword(word, lim);
	return c;
}


int skip_stdin_till(const char* pattern)
{
	//TODO: Make buf a parameter ?
	size_t n = strlen(pattern);
	char *buf = malloc(n + 1);

	if(!buf)
		return 0;

	//read first n chars
	int i = 0;
	for(; i < n && (buf[i] = getch()) != EOF; ++i)
	    ;
	
	if(buf[i] == EOF){ // we exited from previous loop because of EOF
	    free(buf);
	    return 0;
	}

	buf[n] = '\0';

	char *b = NULL;
	while(strcmp(buf, pattern) != 0){
		for(b = buf; *b && *(b+1); ++b) //shift buf left by one
			*b = *(b + 1);
		
		if((*b = getch()) == EOF){
			free(buf);
			return 0;
		}

		//printf("buf:%s\n", buf);
	}

	return 1;
}

#define BUFERSIZE 10

char buf[BUFERSIZE];/*buffer for ungetch*/
int bufp = 0; /*next free position in buf*/

int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) /*push character back to input*/
{
	if(bufp >= BUFERSIZE)
		printf("Error: ungetch - too many charackters\n");
	else
	{
		buf[bufp++] = c;
	}
}
