/*
  * Exercise 6-3. Write a cross-referencer that prints a list of all words in a document, and for
  * each word, a list of the line numbers on which it occurs. Remove noise words like "the",
  * "and", and so on
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct list_node {
	size_t line_number;
	struct list_node *next;
};

struct list {
	struct list_node *head;
	struct list_node *tail;
};

struct tree_node {
	char *word;
	struct list *lines_list;

	struct tree_node *left;
	struct tree_node *right;
};

#define MAXWORD 100

struct list_node *list_node_alloc(void);
struct list *list_alloc(void);
void list_node_free(struct list_node *p);
void list_free(struct list *p);
void list_add(struct list *p, int line);

struct tree_node *tree_node_alloc(char *word);
void tree_node_free(struct tree_node *p);
/* Returns a pointer to a node with a given word, or if missing -
 * a pointer to a pointer in which node coud be allocated*/
struct tree_node **search_tree(struct tree_node **p, char *word);
struct tree_node *addtree(struct tree_node **p, char *word, int line);
void treeprint(struct tree_node *root);
void free_tree(struct tree_node *p);

int getword(char *word, int limit, int* line_number);
char *binsearch(char *, char *[], int);

int getch(void);
void ungetch(int);

int main(int argc, char* argv[])
{
	char *noise_words[] = {"a", "and", "the"};
	size_t noise_count = (sizeof noise_words/ sizeof(noise_words[0]));

	char word[MAXWORD];
	struct tree_node *root = NULL;

	int line = 1;
	while (getword(word, MAXWORD, &line) != EOF)
		if(!binsearch(word, noise_words, noise_count))
			addtree(&root, word, line);

	treeprint(root);

	free_tree(root); // free memory
	tree_node_free(root);

	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
char *binsearch(char *word, char *tab[], int n)
{
	int cond;
	char **low = tab;
	char **high = tab + n;
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

struct tree_node *addtree(struct tree_node **root, char *word, int line)
{
	struct tree_node **node = search_tree(root, word);

	if(*node == NULL)
		*node = tree_node_alloc(word);

	list_add((*node)->lines_list, line);

	return *node;
}

struct tree_node *tree_node_alloc(char *word)
{
	struct tree_node *p = (struct tree_node*)malloc(sizeof(struct tree_node));

	if(!p)
		printf("Error in tree_node_alloc: malloc failed!\n");

	p->word = strdup(word);
	p->left = p->right = NULL;
	p->lines_list = list_alloc();

	return p;
}
struct list *list_alloc()
{
	struct list *p = (struct list*)malloc(sizeof(struct list));
	if(!p)
		printf("Error in list_alloc: malloc failed\n");

	p->head = NULL;
	p->tail = NULL;

	return p;
}

void list_node_free(struct list_node *p)
{
	free(p);
}

void list_free(struct list *p)
{
	if(!p){
		printf("Error in list_free: list is NULL\n");
	}

	struct list_node *next;
	for(struct list_node *node = p->head; node; node = next){
		next = node->next;
		list_node_free(node);
	}
}

struct list_node *list_node_alloc(void)
{
	struct list_node *p = (struct list_node*)malloc(sizeof(struct list_node));

	if(!p)
		printf("Error in list_node_alloc: failed to malloc\n");

	p->next = NULL;

	return p;
}

void list_add(struct list *p, int line)
{
	if(!p){
		printf("Error in list_add: input list is NULL\n");
		return;
	}

	if(!p->head)
		p->head = p->tail = list_node_alloc();
		
	else
		p->tail = p->tail->next = list_node_alloc();
	
	p->tail->line_number = line;
}

void treeprint(struct tree_node *p)
{
	if(p != NULL){
		treeprint(p->left);

		printf("%s:", p->word);
		for(struct list_node* l = p->lines_list->head;l;l = l->next){
			printf(" %lu", l->line_number);
		}
		printf("\n");

		treeprint(p->right);
	}
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

	list_free(p->lines_list);

	free(p);
}

int getword(char *word, int lim, int *line)
{
	char *w = word;
	char c;

	while(!isalnum(c = getch()) && c != EOF)
		*line += c == '\n';

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
