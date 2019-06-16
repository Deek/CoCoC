/* xlstr - xlisp string builtin functions */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* external variables */
extern struct node *xlstack;

/* external procedures */
extern char *strcat();

/* xstrlen - length of a string */
static struct node *xstrlen(args)
  struct node *args;
{
    struct node *oldstk,arg,*val;
    int total;

    /* create a new stack frame */
    oldstk = xlsave(&arg,NULL);

    /* initialize */
    arg.n_ptr = args;
    total = 0;

    /* loop over args and total */
    while (arg.n_ptr != NULL)
        total += strlen(xlevmatch(STR,&arg.n_ptr)->n_str);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* create the value node */
    val = newnode(INT);
    val->n_int = total;

    /* return the total */
    return (val);
}

/* xstrcat - concatenate a bunch of strings */
/*              this routine does it the dumb way -- one at a time */
static struct node *xstrcat(args)
  struct node *args;
{
    struct node *oldstk,arg,val,rval;
    int newlen;
    char *result,*argstr,*newstr;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&val,&rval,NULL);

    /* initialize */
    arg.n_ptr = args;
    rval.n_ptr = newnode(STR);
    rval.n_ptr->n_str = result = stralloc(0);
    *result = 0;

    /* loop over args */
    while (arg.n_ptr != NULL) {

        /* get next argument */
        val.n_ptr = xlevmatch(STR,&arg.n_ptr);
        argstr = val.n_ptr->n_str;

        /* compute length of result */
        newlen = strlen(result) + strlen(argstr);

        /* allocate string and copy */
        newstr = stralloc(newlen);
        strcpy(newstr,result);
        strfree(result);
        rval.n_ptr->n_str = result = strcat(newstr,argstr);
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the new string */
    return (rval.n_ptr);
}

/* substr - return a substring */
static struct node *substr(args)
  struct node *args;
{
    struct node *oldstk,arg,src,val;
    int start,forlen,srclen;
    char *srcptr,*dstptr;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&src,&val,NULL);

    /* initialize */
    arg.n_ptr = args;
    
    /* get string and its length */
    src.n_ptr = xlevmatch(STR,&arg.n_ptr);
    srcptr = src.n_ptr->n_str;
    srclen = strlen(srcptr);

    /* get starting pos -- must be present */
    start = xlevmatch(INT,&arg.n_ptr)->n_int;

    /* get length -- if not present use remainder of string */
    if (arg.n_ptr != NULL)
        forlen = xlevmatch(INT,&arg.n_ptr)->n_int;
    else
        forlen = srclen;                /* use len and fix below */

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* don't take more than exists */
    if (start + forlen > srclen)
        forlen = srclen - start + 1;

    /* if start beyond string -- return null string */
    if (start > srclen) {
        start = 1;
        forlen = 0; }
        
    /* create return node */
    val.n_ptr = newnode(STR);
    val.n_ptr->n_str = dstptr = stralloc(forlen);

    /* move string */
    for (srcptr += start-1; forlen--; *dstptr++ = *srcptr++)
        ;
    *dstptr = 0;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the substring */
    return (val.n_ptr);
}

/* ascii - return ascii value */
static struct node *ascii(args)
  struct node *args;
{
    struct node *oldstk,val;

    /* create a new stack frame */
    oldstk = xlsave(&val,NULL);

    /* build return node */
    val.n_ptr = newnode(INT);
    val.n_ptr->n_int = *(xlevmatch(STR,&args)->n_str);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the character */
    return (val.n_ptr);
}

/* chr - convert an INT into a one character ascii string */
static struct node *chr(args)
  struct node *args;
{
    struct node *oldstk,val;
    char *sptr;

    /* create a new stack frame */
    oldstk = xlsave(&val,NULL);

    /* build return node */
    val.n_ptr = newnode(STR);
    val.n_ptr->n_str = sptr = stralloc(1);
    *sptr++ = xlevmatch(INT,&args)->n_int;
    *sptr = 0;

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the new string */
    return (val.n_ptr);
}

/* xatoi - convert an ascii string to an integer */
static struct node *xatoi(args)
  struct node *args;
{
    struct node *val;
    int n;

    /* get the string and convert it */
    n = atoi(xlevmatch(STR,&args)->n_str);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* create the value node */
    val = newnode(INT);
    val->n_int = n;

    /* return the number */
    return (val);
}

/* xitoa - convert an integer to an ascii string */
static struct node *xitoa(args)
  struct node *args;
{
    struct node *val;
    char buf[20];

    /* get the integer and convert it */
    sprintf(buf,"%d",xlevmatch(INT,&args)->n_int);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* create the value node */
    val = newnode(STR);
    val->n_str = strsave(buf);

    /* return the string */
    return (val);
}

/* xlsinit - xlisp string initialization routine */
xlsinit()
{
    xlsubr("strlen",xstrlen);
    xlsubr("strcat",xstrcat);
    xlsubr("substr",substr);
    xlsubr("ascii",ascii);
    xlsubr("chr", chr);
    xlsubr("atoi",xatoi);
    xlsubr("itoa",xitoa);
}