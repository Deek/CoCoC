/* xlfio - xlisp file i/o */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* external variables */
extern struct node *xlstack;

/* local variables */
static char buf[STRMAX+1];

/* xlfopen - open a file */
static struct node *xlfopen(args)
  struct node *args;
{
    struct node *oldstk,arg,fname,mode,*val;
    FILE *fp;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&fname,&mode,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get the file name */
    fname.n_ptr = xlevmatch(STR,&arg.n_ptr);

    /* get the mode */
    mode.n_ptr = xlevmatch(STR,&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* try to open the file */
    if ((fp = (FILE *) fopen(fname.n_ptr->n_str,
                        mode.n_ptr->n_str)) != (FILE *) NULL) {
        val = newnode(FPTR);
        val->n_fp = fp;
    }
    else
        val = NULL;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the file pointer */
    return (val);
}

/* xlfclose - close a file */
static struct node *xlfclose(args)
  struct node *args;
{
    struct node *fptr;

    /* get file pointer */
    fptr = xlevmatch(FPTR,&args);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* make sure the file exists */
    if (fptr->n_fp == NULL)
        xlfail("file not open");

    /* close the file */
    fclose(fptr->n_fp);
    fptr->n_fp = NULL;

    /* return nil */
    return (NULL);
}

/* xlgetc - get a character from a file */
static struct node *xlgetc(args)
  struct node *args;
{
    struct node *val;
    FILE *fp;
    int ch;

    /* get file pointer */
    if (args != NULL)
        fp = xlevmatch(FPTR,&args)->n_fp;
    else
        fp = stdin;

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* make sure the file exists */
    if (fp == NULL)
        xlfail("file not open");

    /* get character and check for eof */
    if ((ch = getc(fp)) != EOF) {

        /* create return node */
        val = newnode(INT);
        val->n_int = ch;
    }
    else
        val = NULL;

    /* return the character */
    return (val);
}

/* xlputc - put a character to a file */
static struct node *xlputc(args)
  struct node *args;
{
    struct node *oldstk,arg,chr;
    FILE *fp;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&chr,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get the character */
    chr.n_ptr = xlevmatch(INT,&arg.n_ptr);

    /* get file pointer */
    if (arg.n_ptr != NULL)
        fp = xlevmatch(FPTR,&arg.n_ptr)->n_fp;
    else
        fp = stdout;

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* make sure the file exists */
    if (fp == NULL)
        xlfail("file not open");

    /* put character to the file */
    putc(chr.n_ptr->n_int,fp);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the character */
    return (chr.n_ptr);
}

/* xlfgets - get a string from a file */
static struct node *xlfgets(args)
  struct node *args;
{
    struct node *str;
    char *sptr;
    FILE *fp;

    /* get file pointer */
    if (args != NULL)
        fp = xlevmatch(FPTR,&args)->n_fp;
    else
        fp = stdin;

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* make sure the file exists */
    if (fp == NULL)
        xlfail("file not open");

    /* get character and check for eof */
    if (fgets(buf,STRMAX,fp) != NULL) {

        /* create return node */
        str = newnode(STR);
        str->n_str = strsave(buf);

        /* make sure we got the whole string */
        while (buf[strlen(buf)-1] != '\n') {
            if (fgets(buf,STRMAX,fp) == NULL)
                break;
            sptr = str->n_str;
            str->n_str = stralloc(strlen(sptr) + strlen(buf));
            strcpy(str->n_str,sptr);
            strcat(buf);
            strfree(sptr);
        }
    }
    else
        str = NULL;

    /* return the string */
    return (str);
}

/* xlfputs - put a string to a file */
static struct node *xlfputs(args)
  struct node *args;
{
    struct node *oldstk,arg,str;
    FILE *fp;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&str,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get the string */
    str.n_ptr = xlevmatch(STR,&arg.n_ptr);

    /* get file pointer */
    if (arg.n_ptr != NULL)
        fp = xlevmatch(FPTR,&arg.n_ptr)->n_fp;
    else
        fp = stdout;

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* make sure the file exists */
    if (fp == NULL)
        xlfail("file not open");

    /* put string to the file */
    fputs(str.n_ptr->n_str,fp);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the string */
    return (str.n_ptr);
}

/* xlfinit - initialize file stuff */
xlfinit()
{
    xlsubr("fopen",xlfopen);
    xlsubr("fclose",xlfclose);
    xlsubr("getc",xlgetc);
    xlsubr("putc",xlputc);
    xlsubr("fgets",xlfgets);
    xlsubr("fputs",xlfputs);
}