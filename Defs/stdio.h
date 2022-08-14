#ifndef _STDIO_H_
#define _STDIO_H_
#ifndef _ANSI_H_
# include <ansi.h>
#endif
#ifndef _SIZE_T_
# define _SIZE_T_
typedef unsigned	size_t;
typedef int			ssize_t;
#endif

#define BUFSIZ	256
/* maximum number of open files */
#define _NFILE	16

#define FILE	struct _iobuf
extern struct _iobuf {
	char	*_ptr;		/* buffer pointer */
	char	*_base;		/* buffer base address */
	char	*_end;		/* buffer end address */
	int		_flag;		/* file status */
	int		_fd;		/* file path number */
	char	_save;		/* for 'ungetc' when unbuffered */
	int		_bufsiz;	/* size of data buffer */
} _iob[_NFILE];

#define _READ		0x01
#define _WRITE		0x02
#define _UNBUF		0x04
#define _BIGBUF		0x08
#define _EOF		0x10
#define _ERR		0x20
#define _SCF		0x40
#define _RBF		0x80
#define _DEVMASK	0xc0	/* _SCF|_RBF */
#define _WRITTEN	0x0100	/* buffer written in update mode */
#define _INIT		0x8000	/* _iob initialized */

#define EOF		(-1)
#define EOL		'\n'	/* ASCII 13 CR */
#ifndef NULL
# define NULL	0
#endif

#define stdin	(&_iob[0])
#define stdout	(&_iob[1])
#define stderr	(&_iob[2])

/*
 * More standard stdio names for our definitions
 */
#define _IOREAD		_READ
#define _IOWRT		_WRITE
#define _IORW		_READ|_WRITE

#define _IOFBF		0x0		/* stream is fully buffered */
#define _IOLBF		_SCF	/* stream is line-buffered like a terminal */
#define _IONBF		_UNBUF	/* stream is unbuffered */
#define _IOMYBUF	_BIGBUF	/* stream has a user buffer */
#define _IOEOF		_EOF	/* stream has EOF condition */
#define _IOERR		_ERR	/* stream has an error */

#define STDIN_FILENO	(0)
#define STDOUT_FILENO	(1)
#define STDERR_FILENO	(2)

#define NAME_MAX	(29)
#define MAXNAMLEN	NAME_MAX

#define FOPEN_MAX	_NFILE

#ifndef SEEK_SET
# define SEEK_SET	(0)
# define SEEK_CUR	(1)
# define SEEK_END	(2)
#endif

int fclose _OP((FILE *stream));
int fflush _OP((FILE *stream));

FILE *fopen _OP((const char *pathname, const char *mode));
FILE *fdopen _OP((int fd, const char *mode));
FILE *freopen _OP((const char *pathname, const char *mode, FILE *stream));

_VOID setbuf _OP((FILE *stream, char *buf));

int printf _OP((const char *format, ...));
int fprintf _OP((FILE *stream, const char *format, ...));
int sprintf _OP((char *str, const char *format, ...));

/*
	These functions don't actually do anything, but referencing them anywhere
	in your program causes other code to be linked into the final executable.
*/
_VOID pflinit _OP((_VOID));	/* program uses printf() to format long ints */
_VOID pffinit _OP((_VOID));	/* program uses printf() to format float/double */

int scanf _OP((const char *format, ...));
int fscanf _OP((FILE *stream, const char *format, ...));
int sscanf _OP((char *str, const char *format, ...));

int getc _OP((FILE *stream));
int fgetc _OP((FILE *stream));
int getw _OP((FILE *stream));
#define getchar()	getc(stdin)

int putc _OP((int c, FILE *stream));
int fputc _OP((int c, FILE *stream));
int putw _OP((int c, FILE *stream));
#define putchar(c)	putc(c,stdout)

#ifdef DEPRECATED
char *gets _OP((char *s));	/* Never use this function. It's massively unsafe. */
#endif
char *fgets _OP((char *s, int size, FILE *stream));

int puts _OP((const char *s));
int fputs _OP((const char *s, FILE *stream));

int ungetc _OP((int c, FILE *stream));

size_t fread _OP((_VOIDPTR ptr, size_t size, size_t nmemb, FILE *stream));
size_t fwrite _OP((const _VOIDPTR ptr, size_t size, size_t nmemb, FILE *stream));

int fseek _OP((FILE *stream, long offset, int whence));
long ftell _OP((FILE *stream));
_VOID rewind _OP((FILE *stream));

#define clearerr(p)	((p)->_flag&=~(_ERR|_EOF))
#define feof(p)		((p)->_flag&_EOF)
#define ferror(p)	((p)->_flag&_ERR)

_VOID perror _OP((const char *s));

#define fileno(p)	((p)->_fd)

FILE *popen _OP((const char *command, const char *type));
int pclose _OP((FILE *stream));

#endif /* _STDIO_H_ */
