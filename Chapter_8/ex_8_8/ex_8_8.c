/*
 * Exercise 8-8. Write a routine bfree(p,n)
 * that will free any arbitrary block p of n characters
 * into the free list maintained by malloc and free.
 * By using bfree, a user can add a static or external array to the free list at any time.
*/

#define _GNU_SOURCE 1 /*NOTE: to avoid  warning: implicit declaration of function ‘sbrk’
			   when using --std=* compile flag*/

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

typedef long Align;

union Header {
	struct {
		union Header *next; /* next block in a free list */
		short size; /* size of this block in sizeof(Header) */
	} s;
	Align x; /* force alignment of blocks */
};


static void* malloc(size_t);
static void* calloc(size_t, size_t);
static void free(void*);
/* Inserts memory of size n (in bytes) into a free list
   Returns the number of units actually used */
static size_t bfree(void *p, size_t n);

void printmem();

#define N 100

int main(){

	static int my_mem[N];

	void *curr = sbrk(0);
	size_t allocated = 0;
	
	if((allocated = bfree(my_mem, N * sizeof(int))) == 0){
		printf("Failed to bfree. Exit.\n");
		return 0;
	}

	printf("My static mem range %p - %p\nUsed:%lu\n", my_mem, (my_mem + N - 1)
			,allocated * sizeof(union Header));

	int *foo = malloc(10 * sizeof(int));


	for(int i = 0; i < 10; ++i){
		foo[i] = i;
		printf("%d - %p\n", foo[i], foo + i);
	}

	void *bar = malloc(1024);

	void *new = sbrk(0);

	printf("Total allocated: %lu\n", new - curr);
	printf("Foo: %p, potential header:%p\n", foo, ((union Header*)foo)-1);
	printmem();
	free(foo);
	free(bar);
	printmem();
}


union Header* morecore(size_t number_of_units);

static union Header base; /* empty list to get started */
static union Header *freep = NULL; /* start of free list */

#define MAX_BYTES 1048576 // 1MB

void* malloc(size_t nbytes)
{
	if(nbytes > MAX_BYTES)
		return NULL;

	union Header *p, *prevp;
	size_t nunits = (nbytes + sizeof(union Header) - 1)/sizeof(union Header) + 1;

	if((prevp = freep) == NULL){ /* no free list yet */
		base.s.next = freep = prevp = &base;
		base.s.size = 0;
	}

	for(p = prevp->s.next; ;prevp = p, p = p->s.next){
		if(p->s.size >= nunits){ /* big enough */
			if(p->s.size == nunits) /* exactly */
				prevp->s.next = p->s.next;
			else {
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}

			freep = prevp;
			return (void*)(p + 1);
		}

		if(p == freep){ /* wrapped around free list */
			if((p = morecore(nunits)) == NULL)
				return NULL; /* non left */
		}
	}
	return NULL;
}

#define NALLOC 1024 /* minimum #units tp request */
static size_t maxalloc; /* Idea from the C answer book */

/* morecore: ask system for more memory */
union Header* morecore(size_t nu)
{
	void *cp;
	union Header *up;
	
	if(nu < NALLOC)
		nu = NALLOC;

	cp = sbrk(nu * sizeof(union Header));

	if(cp == (void*)-1) /* No space at all*/
		return NULL;

	up = (union Header*)cp;
	up->s.size = nu;

	/* Remebmer the biggest allocated chunk size */
	maxalloc = up->s.size > maxalloc ? up->s.size : maxalloc ;

	free((void*)(up + 1));

	return freep;
}

/* free: tup block ap in free list */
void free(void *ap)
{
	if(ap == NULL)
		return;

	union Header *bp, *p;
	bp = (union Header*)ap - 1; /* point to block header */

	if(bp == NULL || bp->s.size == 0 || bp->s.size > maxalloc)
		return;

	for(p = freep; !(bp > p && bp < p->s.next); p = p->s.next)
		if(p >= p->s.next && (bp > p || bp < p->s.next))
			break; /* free block at start or end of arena*/

	if(bp + bp->s.size == p->s.next){ /* join to upper nbr */
		bp->s.size += p->s.next->s.size;
		bp->s.next = p->s.next->s.next;
	} else
		bp->s.next = p->s.next;

	if(p + p->s.size == bp){ /* join to lower nbr */
		p->s.size += bp->s.size;
		p->s.next = bp->s.next;
	} else
		p->s.next = bp;
	freep = p;
}

size_t bfree(void *p, size_t n)
{
	if(n < 2 * sizeof(union Header))
		return 0;

	union Header *hp = (union Header*)p;
	hp->s.size = n / sizeof(union Header);

	maxalloc = hp->s.size;

	if(freep == NULL){ /* no free list yet */
		base.s.next = freep = &base;
		base.s.size = 0;
	}

	free((void*)(hp + 1));

	return  hp->s.size;
}

void* calloc(size_t nmemb, size_t size){
	return malloc(nmemb * size);
}

void printmem(){
	union Header *p, *prev;

	printf("Max allocated size(in units): %lu\n", maxalloc);

	p = prev = freep;

	while(p){
		printf("Header: %8p\nSize: %4d\nNext: %8p\n\n", p, p->s.size, p->s.next);
		prev = p;
		p = p->s.next;

		if(p == freep)
			break;
	}
	printf("==== end printmem\n");
}

