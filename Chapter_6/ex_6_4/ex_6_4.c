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

#define MAXWORD 100
typedef void (*tree_node_fn)(struct tree_node*);

struct tree_node *tree_node_alloc(char *word, size_t initial_count);
void tree_node_free(struct tree_node *p);

/* Returns a pointer to a node with a given word, or if missing -
 * a pointer to a pointer in which node coud be allocated*/
struct tree_node **search_tree(struct tree_node **p, char *word);
struct tree_node *addtree(struct tree_node **p, char *word);

/*Iteratres over the tree applying op for each node*/
void iterate_tree(struct tree_node *root, tree_node_fn op);
void print_tree_node(struct tree_node *node);
void free_tree(struct tree_node *p);

int getword(char *word, int limit);

int getch(void);
void ungetch(int);

int main(int argc, char* argv[])
{
	char word[MAXWORD];
	struct tree_node *root = NULL;

	size_t count = 1;
	while (getword(word, MAXWORD) != EOF){
		addtree(&root, word);
		++count;
	}

	//allocate arraur fot count tree_node's
	iterate_tree(root, print_tree_node);

	// free memory
	free_tree(root);
	tree_node_free(root);

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

struct tree_node *addtree(struct tree_node **root, char *word)
{
	struct tree_node **node = search_tree(root, word);

	if(*node == NULL)
		*node = tree_node_alloc(word, 1);
	else
		(*node)->count += 1;

	return *node;
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

void tree_node_free(struct tree_node *p)
{
	if(p == NULL)
		return;

	free(p->word);
	free(p);
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

