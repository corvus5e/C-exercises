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

struct tree_node {
	char *word;
	struct tree_node *left;
	struct tree_node *right;
	struct list_node *head;
	struct list_node *tail;
};

#define MAXWORD 100

struct list_node *list_node_alloc(void);
void list_node_free(void);
struct tree_node *tree_node_alloc(void);
void tree_node_free(struct tree_node *p);

struct tree_node *addtree(struct tree_node *p, char *word, int line);
void treeprint(struct tree_node *root);
void free_tree(struct tree_node *p);

int getword(char *word, int limit, int* line_number);
char *binsearch(char *, char *[], int);

int getch(void);
void ungetch(int);

/* count C keywords */
int main(int argc, char* argv[])
{
	char *noise_words[] = {"a", "and", "the"};
	size_t noise_count = (sizeof noise_words/ sizeof(noise_words[0]));

	char word[MAXWORD];
	struct tree_node *root = NULL;

	int line = 1;
	while (getword(word, MAXWORD, &line) != EOF)
		if(!binsearch(word, noise_words, noise_count))
			printf("%s - %d\n", word, line);
		//root = addtree(root, declaration, NULL, group_prefix_count);

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

struct tree_node *addtree(struct tree_node *p, char *word, int line)
{
	if(p == NULL){
		if(!(p = tree_node_alloc())){
			printf("Error: talloc failed\n");
			return p;
		}

		p->word = strdup(word);
		p->left = p->right = NULL;

		return p;
	}

	int cond;

	if((cond = strcmp(word, p->word)) == 0)
		return p;

	if(cond < 0)
		p->left = addtree(p->left, word, line);
	else
		p->right = addtree(p->right, word, line);

	return p;
}

struct tree_node *tree_node_alloc()
{
	struct tree_node *p = (struct tree_node*)malloc(sizeof(struct tree_node));

	return p;
}

void treeprint(struct tree_node *p)
{
	if(p != NULL){
		treeprint(p->left);
		printf("%s\n", p->word);
		//TODO: print list here
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
	//TODO: free list here
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

	while(isalnum(*w = getch()) && --lim > 0)
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
