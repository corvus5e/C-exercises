/*
 * Exercise 8-3. Design and write _flushbuf, fflush, and fclose.
*/

#include <fcntl.h>
#include <stdlib.h>

#define EOF 	 (-1)
#define BUFSIZE	 5
#define OPEN_MAX 20 /* max files open at once*/

typedef struct _iobuf {
	int  ctn;   /* characters left */
	char *ptr;  /* next character position */
	char *base; /* location of buffer */

	struct {
		unsigned char read : 1;  /* file open for reading */
		unsigned char write : 1;  /* file open for writing */
		unsigned char unbuf : 1;  /* file is unbuffered */
		unsigned char eof : 1; /* EOF has occured on this file */
		unsigned char err : 1; /* erro occured on this file */
	} flags;

	int  fd;    /* file descriptor */
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

int _fillbuf(FILE*);
int _flushbuf(int, FILE*);

#define getc(p) 	(--(p)->ctn >= 0 \
			? (unsigned char) *(p)->ptr++ : _fillbuf(p))

#define putc(x, p)	(--(p)->ctn >= 0 \
			? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar()	getc(stdin)

#define putchar(x)	putc((x), stdout)

FILE* fopen(const char *name, char *mode);

int main(){
	FILE *f = fopen("test.txt", "r");
	FILE *d = fopen("dest.txt", "w");
	int c = 0;
	while((c = getc(stdin)) > 0)
		putc(c, d);

	return 0;
}


#define PERMS 0666

#include <unistd.h>

FILE* fopen(const char *name, char *mode)
{
	int fd = -1;
	FILE *fp;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;

	for(fp = _iob; fp < _iob + OPEN_MAX; fp++)
		if(!fp->flags.read && !fp->flags.write)
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

	if(*mode == 'r')
		fp->flags.read = 1;
	else
		fp->flags.write = 1;

	return fp;
}

/* _fillbuf: allocate and fill input buffer*/
int _fillbuf(FILE *fp)
{
	if(fp->flags.eof || fp->flags.err)
		return EOF;

	size_t bufsize = fp->flags.unbuf ? 1 : BUFSIZE;

	if(!fp->base && !(fp->base = (char*)malloc(bufsize)))
		return EOF;

	fp->ptr = fp->base;

	fp->ctn = read(fp->fd, fp->ptr, bufsize + 1);

	if(--fp->ctn < 0)
	{
		if(fp->ctn == -1)
			fp->flags.eof = 1;
		else
			fp->flags.err = 1;

		fp->ctn = 0;

		return EOF;
	}


	return *(fp->ptr++);
}

int _flushbuf(int x, FILE *fp)
{
	if(fp->flags.eof || fp->flags.err || !fp->flags.write)
		return EOF;

	//NOTE: Write directly to file.
	// This should be a separate function
	if(fp->flags.unbuf){
		fp->ctn = 0;
		if(write(fp->fd, &x, 1) < 0){
			fp->flags.err = 1;
			return EOF;
		}
		return x;
	}

	size_t bufsize = BUFSIZE;

	if(!fp->base){
		fp->base = (char*)malloc(bufsize);
		if(!fp->base){
			fp->flags.err = 1;
			return EOF;
		}
	}
	else if(write(fp->fd, fp->base, fp->ptr - fp->base) < 0){
			fp->flags.err = 1;
			return EOF;
		}

	fp->ptr = fp->base;
	fp->ctn = bufsize - 1;

	return *(fp->ptr++) = x;
}

FILE _iob[OPEN_MAX] = {
	{0, NULL, NULL, {1/*read*/, 0 , 1/*unbuf*/, 0, 0}, 0},
	{0, NULL, NULL, {0, 1/*write*/, 0, 0, 0}, 1},
	{0, NULL, NULL, {0, 1, 1/*unbuf*/, 0, 0}, 2},
};



