/*
 * Exercise 8-2. Rewrite fopen and _fillbuf with fields instead of explicit bit operations.
 * Compare code size and execution speed.
*/

#include <fcntl.h>

#define NULL 	 0
#define EOF 	 (-1)
#define BUFSIZE	 1024
#define OPEN_MAX 20 /* max files open at once*/

typedef struct _iobuf {
	int  ctn;   /* characters left */
	char *ptr;  /* next character position */
	char *base; /* location of buffer */
	int  flag;  /* mode of file access */
	int  fd;    /* file descriptor */
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

enum _flags {
	_READ  = 01,  /* file open for reading */
	_WRITE = 02,  /* file open for writing */
	_UNBUF = 04,  /* file is unbuffered */
	_EOF   = 010, /* EOF has occured on this file */
	_ERR   = 020  /* erro occured on this file */
};

int _fillbuf(FILE*);
int _flushbuf(FILE*);

#define feof(p) 	(((p)->flag & _EOF) != 0)

#define ferror(p)	(((p)->flag & _ERR) != 0)

#define getc(p) 	(--(p)->ctn >= 0 \
			? (unsigned char) *(p)->ptr++ : _fillbuf(p))

#define putc(x, p)	(--(p)->ctn >= 0 \
			? (unsigned char) *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar()	getc(stdin)

#define putchar(x)	putc((x), stdout)

FILE* fopen(const char *name, char *mode);

int main(){
}

#include <unistd.h>

#define PERMS 0666

FILE* fopen(const char *name, char *mode)
{
	int fd = -1;
	FILE *fp;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;

	for(fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if((fp->flag & (_READ | _WRITE)) == 0)
			break;
	if(fp >= _iob + OPEN_MAX) /* no free slots */
		return NULL;

	if(*mode == 'w')
		fd = creat(name, PERMS);
	else if(*mode == 'a'){
		if((fd = open(name, O_WRONLY)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, SEEK_END);
	}
	else
		fd = open(name, O_RDONLY);

	if(fd == -1)
		return NULL;
	fp->fd = fd;
	fp->ctn = 0;
	fp->base = NULL;
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
	return fp;
}

/* _fillbuf: allocate and fill input buffer*/
int _fillbuf(FILE *fp)
{
}

