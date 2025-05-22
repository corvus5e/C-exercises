/*
 * Exercise 8-6. The standard library function calloc(n,size) returns a pointer to n objects of
 * size size, with the storage initialized to zero. Write calloc, by calling malloc or by
 * modifying it.
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

void printmem();

int main(){
	void *curr = sbrk(0);

	int *foo = calloc(10, sizeof(int));
	for(int i = 0; i < 10; ++i){
		foo[i] = i;
		printf("%d - %p\n", foo[i], foo + i);
	}

	void *new = sbrk(0);

	printf("Total allocated: %lu\n", new - curr);
	printf("Foo: %p, potential header:%p\n", foo, ((union Header*)foo)-1);
	printmem();
	free(foo);
	printmem();
}


union Header* morecore(size_t number_of_units);

static union Header base; /* empty list to get started */
static union Header *freep = NULL; /* start of free list */

void* malloc(size_t nbytes)
{
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

	free((void*)(up + 1));

	return freep;
}

/* free: tup block ap in free list */
void free(void *ap)
{
	union Header *bp, *p;
	bp = (union Header*)ap - 1; /* point to block header */

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

static void* calloc(size_t nmemb, size_t size){
	return malloc(nmemb * size);
}

void printmem(){
	union Header *p, *prev;

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

