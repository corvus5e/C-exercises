/*
 * Exercise 6-4. Write a program that prints the distinct words in its input
 * sorted into decreasing order of frequency of occurrence. Precede each word by its count.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct tree_node {
	char *word;
	size_t count;

	struct tree_node *left;
	struct tree_node *right;
};

struct vector {
	struct tree_node **begin; // non owning pointer
	size_t size;
	size_t capacity;
};

#define MAXWORD 100
typedef void (*tree_node_fn)(struct tree_node*);

struct tree_node *tree_node_alloc(char *word, size_t initial_count);
void tree_node_free(struct tree_node *p);

struct vector *vector_alloc(int capacity);
void vector_free(struct vector *v);
int vector_add(struct vector *v, struct tree_node * const n);

/* Returns a pointer to a node with a given word, or if missing -
 * a pointer to a pointer in which node could be allocated*/
struct tree_node **search_tree(struct tree_node **p, char *word);

/*Iterates over the tree applying op for each node*/
void iterate_tree(struct tree_node *root, tree_node_fn op);
void print_tree_node(struct tree_node *node);
void free_tree(struct tree_node *p);
int tree_node_count_cmp(const void *l, const void *r);

int getword(char *word, int limit);

int getch(void);
void ungetch(int);

int main(int argc, char* argv[])
{
	char word[MAXWORD];
	struct tree_node *root = NULL;
	struct tree_node **node = NULL;
	struct vector *nodes = vector_alloc(1);

	if(!nodes)
		return 1;

	while (getword(word, MAXWORD) != EOF){
		if(*(node = search_tree(&root, word)) == NULL){
			*node = tree_node_alloc(word, 1);
			if(!vector_add(nodes, *node)){
				printf("Can't go without vector. Abort ...");
				return 1;
			}
		}
		else
			(*node)->count += 1;
	}

	qsort(nodes->begin, nodes->size, sizeof(struct tree_node*), tree_node_count_cmp);

	for(int i = 0; i < nodes->size; ++i)
		print_tree_node(nodes->begin[i]);

	// free memory
	free_tree(root);
	tree_node_free(root);
	vector_free(nodes);

	return 0;
}

struct tree_node **search_tree(struct tree_node **node, char *word)
{
	if(*node == NULL)
		return node;

	int cond;
	struct tree_node  *p = *node;

	if((cond = strcmp(word, p->word)) == 0)
		return node;

	if(cond < 0)
		return search_tree(&(p->left), word);

	return search_tree(&(p->right), word);
}

struct tree_node *tree_node_alloc(char *word, size_t initial_count)
{
	struct tree_node *p = (struct tree_node*)malloc(sizeof(struct tree_node));

	if(!p){
		printf("Error in tree_node_alloc: malloc failed!\n");
		return p;
	}

	p->word = strdup(word);
	p->count = initial_count;
	p->left = p->right = NULL;

	return p;
}

void iterate_tree(struct tree_node *p, tree_node_fn op)
{
	if(p != NULL){
		iterate_tree(p->left, op);
		op(p);
		iterate_tree(p->right, op);
	}
}

void print_tree_node(struct tree_node *p)
{
	printf("%s: %lu\n", p->word, p->count);
}

void free_tree(struct tree_node *p)
{
	if(p != NULL){
		free_tree(p->left);
		tree_node_free(p->left);

		free_tree(p->right);
		tree_node_free(p->right);
	}
}

int tree_node_count_cmp(const void *left_node, const void *right_node)
{
	struct tree_node *l = (*(struct tree_node **)left_node);
	struct tree_node *r = (*(struct tree_node **)right_node);

	int cond = r->count - l->count;

	if(cond == 0)
		return strcmp(l->word, r->word);

	return cond;
}

void tree_node_free(struct tree_node *p)
{
	if(p == NULL)
		return;

	free(p->word);
	free(p);
}

struct vector *vector_alloc(int capacity)
{
	struct vector *p = (struct vector*)malloc(sizeof(struct vector));

	if(!p){
		printf("Error in vector_alloc: malloc failed\n");
		return p;
	}

	p->begin = (struct tree_node**)malloc(capacity * sizeof(struct tree_node));

	if(!p->begin){
		printf("Error in vector_alloc: malloc for inner array failed\n");
	}

	p->capacity = capacity;
	p->size = 0;

	return p;
}

void vector_free(struct vector *v)
{
	if(!v)
		return;

	if(v->begin)
		free(v->begin);

	free(v);
}

int vector_add(struct vector *v, struct tree_node * const n)
{
	if(!v){
		printf("Error in vector_add: vector is NULL\n");
		return 0;
	}

	if(v->size >= v->capacity){
		int new_capacity = v->capacity * 2;
		void *new_begin = realloc(v->begin, sizeof(struct tree_node*) * new_capacity);
		if(!new_begin){
			printf("Error in vector_add: realloc failed\n");
			return 0;
		}
		v->begin = (struct tree_node**)new_begin;
		v->capacity = new_capacity;
	}

	v->begin[v->size++] = n;

	return 1;
}

int getword(char *word, int lim)
{
	char *w = word;
	char c;

	while(!isalnum(c = getch()) && c != EOF)
		;

	if(c == EOF)
		return EOF;

	*w++ = c;

	while((isalnum(*w = getch()) || *w == '\'') && --lim > 0)
		++w;

	ungetch(*w);

	*w = '\0';

	return word[0];
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

