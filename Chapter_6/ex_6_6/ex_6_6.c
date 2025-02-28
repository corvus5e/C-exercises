/*
 * Exercise 6-6. Implement a simple version of the #define processor (i.e., no arguments)
 * suitable for use with C programs, based on the routines of this section. You may also find
 * getch and ungetch helpful.
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
#define HASHSIZE 101

enum word_kind {
	SPACE,
	WORD,
	LITERAL,
	COMMENT,
	NEW_LINE = '\n',
};

struct nlist {
	struct nlist *next;
	char *name;
	char *defn;
};

static struct nlist *hashtab[HASHSIZE];

struct nlist *lookup(char*);
struct nlist *install(char *name, char *defn);
void undef(char *name);
void free_nlist(struct nlist*);
void print_hashtab();
unsigned int hash(char*);

int getword(char *, int);
int get_next_word(char*, int); // skips spaces and comments
int read_till_char(char*, char stop_char, int lim);
int read_defn(char *, int);
int skip_stdin_till(const char* pattern);
int getch(void);
void ungetch(int);

int main()
{
	char word[MAXWORD];
	char value[MAXWORD];
	int c = 0;
	struct nlist *np;

	while((c = getword(word, MAXWORD)) != EOF){
		if(!strcmp(word, "#define")){
			if(get_next_word(word, MAXWORD) != EOF &&
				read_defn(value, MAXWORD) != EOF){
				//printf("[%s=%s]", word, value);
				install(word, value);
			}
		}
		else if(isalpha(word[0]) && (np = lookup(word))){
			printf("%s", np->defn);
		}
		else
			printf("%s", word);
	}

	printf("/* Defines used:\n");
	print_hashtab();
	printf("*/\n");
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

int getword(char *word, int lim)
{
	char *w = word;
	*w = '\0'; // consider the buffer as empty at start

	int c = getch();
	
	if(c == EOF)
		return EOF;

	if(isspace(c)){
		for(*w++ = c; (*w = getch()) == c; ++w)
			;
		ungetch(*w);
		*w = '\0';
		return c == '\n' ? NEW_LINE : SPACE;
	}

	if(c == '/'){ // possible comment section
		if((c = getch()) == '/'){ // this is line comment
			skip_stdin_till("\n");
			ungetch('\n');
			return COMMENT;
		}
		else if(c == '*'){ // this is a block comment
			skip_stdin_till("*/");
			return COMMENT;
		}
		ungetch(c);
		c = '/';
	}
	else if(c == '"' || c == '\''){
		*w++ = c;
		read_till_char(w, c, MAXWORD - 1);
		return LITERAL;
	}

	*w++ = c;

	if(isalnum(c) || c == '#'){ //write # and directive in one word
		for(;--lim > 0 && (isalnum((*w = getch())) || *w == '_'); ++w)
			;
		ungetch(*w);
	}

	*w = '\0';

	return WORD;
}

int get_next_word(char *word, int lim)
{
	int c;
	while((c = getword(word, lim)) != EOF && (c == SPACE || c == COMMENT))
		;
	return c;
}

int read_till_char(char *buf, char stop_char, int lim)
{
	char *p = &buf[0];

	while(--lim && (*p = getch()) != EOF && *p != stop_char){
		if(*p == '\\') //escape
			*(++p) = getchar();
		++p;
	}
	if(*p == EOF){
		ungetch(EOF);
		return 0;
	}

	*(++p) = '\0';

	return buf[0];
}

int read_defn(char *defn, int lim)
{
	int c;
	char word[MAXWORD];

	if((c = get_next_word(defn, lim)) == EOF)
		return c;

	for(;*defn;++defn) // skip the first written word
		;
	
	while(lim > 0 && (c = getword(word, lim)) != EOF && c != NEW_LINE){
		//printf("===>%s\n", word);
		char *w = &word[0];
		for(; --lim && (*defn = *w); ++defn, ++w)
			;
		*defn = '\0';
	}
	if(c == EOF)
		ungetch(c);

	return c;
}

int skip_stdin_till(const char* pattern)
{
	size_t n = strlen(pattern);
	char *buf = malloc(n + 1);

	if(!buf)
		return 0;

	//read first n chars
	int i = 0;
	for(; i < n && (buf[i] = getch()) != EOF; ++i)
	    ;
	
	if(buf[i] == EOF){ // we exited from previous loop because of EOF
		ungetch(buf[i]);
		free(buf);
		return 0;
	}

	buf[n] = '\0';

	char *b = NULL;
	while(strcmp(buf, pattern) != 0){
		for(b = buf; *b && *(b+1); ++b) //shift buf left by one
			*b = *(b + 1);
		
		if((*b = getch()) == EOF){
			ungetch(*b);
			free(buf);
			return 0;
		}
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
	if(bufp >= BUFERSIZE){
		printf("Error: ungetch - too many charackters\n");
		exit(1);
	}
	else
		buf[bufp++] = c;
}
