/* xlprint - xlisp print routine */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* external variables */
extern struct node *xlstack;

/* local variables */
static struct node *printsym;

/* print - builtin function print */
static struct node *print(args)
  struct node *args;
{
    xprint(args,TRUE);
}

/* princ - builtin function princ */
static struct node *princ(args)
  struct node *args;
{
    xprint(args,FALSE);
}

/* xprint - common print function */
xprint(args,flag)
  struct node *args; int flag;
{
    struct node *oldstk,arg,val;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&val,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* evaluate and print each argument */
    while (arg.n_ptr != NULL)
        xlprint(xlevarg(&arg.n_ptr),flag);

    /* restore previous stack frame */
    xlstack = oldstk;

    /* return null */
    return (NULL);
}

/* xlprint - print an xlisp value */
xlprint(vptr,flag)
  struct node *vptr; int flag;
{
    struct node *nptr,*next,*msg;
#ifdef FGETNAME 
    char buffer[128];
#endif

    /* print null as the empty list */
    if (vptr == NULL) {
        printf("()");
        return;
    }

    /* check value type */
    switch (vptr->n_type) {
    case SUBR:
            printf("<Subr: #%o>",vptr);
            break;
    case LIST:
            putchar('(');
            for (nptr = vptr; nptr != NULL; nptr = next) {
                xlprint(nptr->n_listvalue,flag);
                if ((next = nptr->n_listnext) != NULL)
                    if (next->n_type == LIST)
                        putchar(' ');
                    else {
                        putchar('.');
                        xlprint(next,flag);
                        break;
                    }
            }
            putchar(')');
            break;
    case SYM:
            printf("%s",vptr->n_symname);
            break;
    case INT:
            printf("%d",vptr->n_int);
            break;
    case STR:
            if (flag)
                putstring(vptr->n_str);
            else
                printf("%s",vptr->n_str);
            break;
    case FPTR:
#ifdef FGETNAME
            printf("<File: %s>",fgetname(vptr->n_fp, buffer));
#else
            printf("<File: #%o>",vptr);
#endif
            break;
    case OBJ:
            if ((msg = xlmfind(vptr,printsym)) == NULL)
                xlfail("no print message");
            xlxsend(vptr,msg,NULL);
            break;
    case KMAP:
            printf("<Kmap: #%o>",vptr);
            break;
    }
}

/* putstring - output a string */
static putstring(str)
  char *str;
{
    int ch;

    /* output the initial quote */
    putchar('"');

    /* output each character in the string */
    while (ch = *str++)

        /* check for a control character */
        if (ch < 040 || ch == '\\') {
            putchar('\\');
            switch (ch) {
            case '\033':
                    putchar('e');
                    break;
            case '\n':
                    putchar('n');
                    break;
            case '\r':
                    putchar('r');
                    break;
            case '\t':
                    putchar('t');
                    break;
            case '\\':
                    putchar('\\');
                    break;
            default:
                    printf("%03o",ch);
                    break;
            }
        }

        /* output a normal character */
        else
            putchar(ch);

    /* output the terminating quote */
    putchar('"');
}

/* xlpinit - initialize the print routines */
xlpinit()
{
    /* find the 'print' symbol */
    printsym = xlenter("print");

    /* enter builtin functions */
    xlsubr("print",print);
    xlsubr("princ",princ);
}