
#include <_string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE];

struct nlist *lookup(char*);
struct nlist *install(char *name, char *defn);
void undef(char *name);
void free_nlist(struct nlist*);
void print_hashtab();

unsigned int hash(char*);

int main()
{
	install("FOO", "bar");
	install("BAR", "foo");
	install("FOX", "bear");

	print_hashtab();

	undef("BAR");
	print_hashtab();

	undef("FOO");
	print_hashtab();

	undef("FOX");
	print_hashtab();
}

struct nlist *lookup(char *s)
{
	struct nlist *np = hashtab[hash(s)];
	for(; np; np = np->next)
		if(strcmp(s, np->name) == 0)
			return np;
	return NULL;
}

struct nlist *install(char *name, char *defn)
{
	struct nlist *np;

	if((np = lookup(name)) == NULL){
		np = (struct nlist*)malloc(sizeof(struct nlist));
		if(np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		unsigned hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else
		free((void*)np->defn);

	if((np->defn = strdup(defn)) == NULL)
		return NULL;
	return np;
}

void undef(char *name)
{
	unsigned hashval = hash(name);
	struct nlist *np = hashtab[hashval];

	if(!np)
		return;

	if(strcmp(name, np->name) == 0){
		hashtab[hashval] = np->next;
		free_nlist(np);
		return;
	}
	
	struct nlist *parent = np;
	np = np->next;

	for(; np; parent = np, np = np->next){
		if(strcmp(name, np->name) == 0){
			parent->next = np->next;
			free_nlist(np);
			return;
		}
	}
}

void free_nlist(struct nlist *np)
{
	if(!np)
		return;

	free(np->name);
	free(np->defn);
	free(np);
}

void print_hashtab()
{
	printf("\n");
	for(int i = 0; i < HASHSIZE; ++i){
		if(hashtab[i] != NULL){
			printf("%d: ", i);
			struct nlist *head = hashtab[i];
			for(; head; head = head->next){
				printf("%s = %s,", head->name, head->defn);
			}
			printf("\n");
		}
	}
}

unsigned int hash(char *s)
{
	unsigned hashval = 0;

	for(;*s != '\0';++s)
		hashval = *s + 31 * hashval;

	return hashval % HASHSIZE;
}
