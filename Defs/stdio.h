#define BUFSIZ  256
#define _NFILE 16    /* maximum number of open files */

typedef struct _iobuf {
   char  *_ptr,      /* buffer pointer */
         *_base,     /* buffer base address */
         *_end;      /* buffer end address */
   int   _flag;      /* file status */
   int   _fd;        /* file path number */
   char  _save;      /* for 'ungetc' when unbuffered */
   int   _bufsiz;    /* size of data buffer */
} FILE;

extern FILE _iob[_NFILE];

#define _READ     0x01
#define _WRITE    0x02
#define _UNBUF    0x04
#define _BIGBUF   0x08
#define _EOF      0x10
#define _ERR      0x20
#define _SCF      0x40
#define _RBF      0x80
#define _DEVMASK  0xc0     /* _SCF|_RBF */
#define _WRITTEN  0x0100   /* buffer written in update mode */
#define _INIT     0x8000   /* _iob initialized */

#define EOF    (-1)
#define ERROR  (-1)
#define EOL    0x0d     /* ASCII 13 CR */
#define NULL   0

#define stdin _iob
#define stdout (&_iob[1])
#define stderr (&_iob[2])

#define PMODE  0x0b     /* r/w for owner, r for others */

#define fgetc        getc
#define putchar(c)   putc(c,stdout)
#define getchar()    getc(stdin)
#define ferror(p)    ((p)->_flag&_ERR)
#define feof(p)      ((p)->_flag&_EOF)
#define clearerr(p)  ((p)->_flag&=~_ERR)
#define fileno(p)    ((p)->_fd)

long ftell();

