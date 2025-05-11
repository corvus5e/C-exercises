/*
 * Exercise 8-4. The standard library function
 * int fseek(FILE *fp, long offset, int origin)
 * is identical to lseek except that fp is a file pointer instead of a file descriptor
 * and return value is an int status, not a position.
 * Write fseek. Make sure that your fseek coordinates properly
 * with the buffering done for the other functions of the library.
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

int fflush(FILE*);
int putc(int x, FILE *p);

FILE* fopen(const char *name, char *mode);
int fclose(FILE*);
int fseek(FILE *stream, long off, int whence);

#define getc(p) 	(--(p)->ctn >= 0 \
			? (unsigned char) *(p)->ptr++ : _fillbuf(p))

#define pautc(x, p)	(--(p)->ctn >= 0 \
			? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar()	getc(stdin)

#define putchar(x)	putc((x), stdout)

int main(){
	FILE *f = fopen("test.txt", "r");
	FILE *d = fopen("dest.txt", "a");
	int c = 0;
	while((c = getc(stdin)) > 0)
		putc(c, d);

	fclose(d);

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

	if(*mode == 'a')
		fseek(fp, 0L, SEEK_END);

	return fp;
}

int fclose(FILE *fp)
{
	if(fflush(fp) < 0)
		return EOF;

	free(fp->base);
	fp->ptr = fp->base = NULL;
	fp->flags.write = fp->flags.read = fp->flags.eof = fp->flags.err = fp->flags.unbuf = 0;

	return 0;
}

int fseek(FILE *fp, long off, int whence)
{
	if(fp->flags.err)
		return -1;

	//tnx to ohkimur
	if(fp->flags.unbuf){
		if(fp->flags.read){
			fp->ctn = 0;
			fp->ptr = fp->base;
		}
		else if(fp->flags.write)
			fflush(fp);
	}

	return lseek(fp->fd, off, whence) < 0 ? EOF : 0;
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

int fflush(FILE *fp)
{
	if(fp->flags.eof || fp->flags.err || !fp->flags.write)
		return EOF;

	if(fp->flags.unbuf)
		return 0; /* No need to flush unbuffered output */

	if(!fp->base)
		return EOF;

	if(!fp->ptr)
		return EOF;

	size_t distance = fp->ptr - fp->base;

	if(write(fp->fd, fp->base, distance == 0 ? 1 : distance) < 0){
			fp->flags.err = 1;
			return EOF;
	}

	fp->ptr = fp->base;
	fp->ctn = BUFSIZE;

	return 0;
}

int putc(int x, FILE *fp)
{
	if(fp->flags.eof || fp->flags.err || !fp->flags.write)
		return EOF;

	if(fp->flags.unbuf){
		fp->ctn = 0;
		if(write(fp->fd, &x, 1) < 0){
			fp->flags.err = 1;
			return EOF;
		}

		return x;
	}

	return (--(fp)->ctn >= 0) ? *(fp)->ptr++ = (x) : _flushbuf(x, fp);
}

int _flushbuf(int x, FILE *fp)
{
	if(fp->flags.eof || fp->flags.err || !fp->flags.write)
		return EOF;

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



