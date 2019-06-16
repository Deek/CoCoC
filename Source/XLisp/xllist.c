/* xllist - xlisp list builtin functions */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* external variables */
extern struct node *xlstack;

/* local variables */
static struct node *t;
static struct node *a_subr;
static struct node *a_list;
static struct node *a_sym;
static struct node *a_int;
static struct node *a_str;
static struct node *a_obj;
static struct node *a_fptr;
static struct node *a_kmap;

/* xlist - builtin function list */
static struct node *xlist(args)
  struct node *args;
{
    struct node *oldstk,arg,list,val,*last,*lptr;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&list,&val,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* evaluate and append each argument */
    for (last = NULL; arg.n_ptr != NULL; last = lptr) {

        /* evaluate the next argument */
        val.n_ptr = xlevarg(&arg.n_ptr);

        /* append this argument to the end of the list */
        lptr = newnode(LIST);
        if (last == NULL)
            list.n_ptr = lptr;
        else
            last->n_listnext = lptr;
        lptr->n_listvalue = val.n_ptr;
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the list */
    return (list.n_ptr);
}

/* cond - builtin function cond */
static struct node *cond(args)
  struct node *args;
{
    struct node *oldstk,arg,list,*val;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&list,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* initialize the return value */
    val = NULL;

    /* find a predicate that is true */
    while (arg.n_ptr != NULL) {

        /* get the next conditional */
        list.n_ptr = xlmatch(LIST,&arg.n_ptr);

        /* evaluate the predicate part */
        if (xlevarg(&list.n_ptr) != NULL) {

            /* evaluate each expression */
            while (list.n_ptr != NULL)
                val = xlevarg(&list.n_ptr);

            /* exit the loop */
            break;
        }
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the value */
    return (val);
}

/* atom - is this an atom? */
static struct node *atom(args)
  struct node *args;
{
    struct node *arg;

    /* get the argument */
    if ((arg = xlevarg(&args)) == NULL || arg->n_type != LIST)
        return (t);
    else
        return (NULL);
}

/* null - is this null? */
static struct node *null(args)
  struct node *args;
{
    /* get the argument */
    if (xlevarg(&args) == NULL)
        return (t);
    else
        return (NULL);
}

/* type - return type of a thing */
static struct node *type(args)
    struct node *args;
{
    struct node *arg;

    if (!(arg = xlevarg(&args)))
        return (NULL);

    switch (arg->n_type) {
        case SUBR: return (a_subr);
        case LIST: return (a_list);
        case SYM: return (a_sym);
        case INT: return (a_int);
        case STR: return (a_str);
        case OBJ: return (a_obj);
        case FPTR: return (a_fptr);
        case KMAP: return (a_kmap);
        default: xlfail("Bad node.");
        }
}

/* listp - is this a list? */
static struct node *listp(args)
  struct node *args;
{
    /* get the argument */
    if (xlistp(xlevarg(&args)))
        return (t);
    else
        return (NULL);
}

/* xlistp - internal listp function */
static int xlistp(arg)
  struct node *arg;
{
    return (arg == NULL || arg->n_type == LIST);
}

/* eq - are these equal? */
static struct node *eq(args)
  struct node *args;
{
    struct node *oldstk,arg,arg1,arg2,*val;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&arg1,&arg2,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* first argument */
    arg1.n_ptr = xlevarg(&arg.n_ptr);

    /* second argument */
    arg2.n_ptr = xlevarg(&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* compare the arguments */
    if (xeq(arg1.n_ptr,arg2.n_ptr))
        val = t;
    else
        val = NULL;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result */
    return (val);
}

/* xeq - internal eq function */
static int xeq(arg1,arg2)
  struct node *arg1,*arg2;
{
    /* compare the arguments */
    if (arg1 != NULL && arg1->n_type == INT &&
        arg2 != NULL && arg2->n_type == INT)
        return (arg1->n_int == arg2->n_int);
    else
        return (arg1 == arg2);
}

/* equal - are these equal? */
static struct node *equal(args)
  struct node *args;
{
    struct node *oldstk,arg,arg1,arg2,*val;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&arg1,&arg2,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* first argument */
    arg1.n_ptr = xlevarg(&arg.n_ptr);

    /* second argument */
    arg2.n_ptr = xlevarg(&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* compare the arguments */
    if (xequal(arg1.n_ptr,arg2.n_ptr))
        val = t;
    else
        val = NULL;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result */
    return (val);
}

/* xequal - internal equal function */
static int xequal(arg1,arg2)
  struct node *arg1,*arg2;
{
    /* compare the arguments */
    if (xeq(arg1,arg2))
        return (TRUE);
    else if (xlistp(arg1) && xlistp(arg2))
        return (xequal(arg1->n_listvalue,arg2->n_listvalue) &&
                xequal(arg1->n_listnext, arg2->n_listnext));
    else
        return (FALSE);
}

/* head - return the head of a list */
static struct node *head(args)
  struct node *args;
{
    struct node *list;

    /* get the list */
    if ((list = xlevmatch(LIST,&args)) == NULL)
        xlfail("null list");

    /* make sure this is the only argument */
    xllastarg(args);

    /* return the head of the list */
    return (list->n_listvalue);
}

/* tail - return the tail of a list */
static struct node *tail(args)
  struct node *args;
{
    struct node *list;

    /* get the list */
    if ((list = xlevmatch(LIST,&args)) == NULL)
        xlfail("null list");

    /* make sure this is the only argument */
    xllastarg(args);

    /* return the tail of the list */
    return (list->n_listnext);
}

/* nth - return the nth element of a list */
static struct node *nth(args)
  struct node *args;
{
    struct node *oldstk,arg,list;
    int n;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&list,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get n */
    if ((n = xlevmatch(INT,&arg.n_ptr)->n_int) < 1)
        xlfail("invalid argument");

    /* get the list */
    if ((list.n_ptr = xlevmatch(LIST,&arg.n_ptr)) == NULL)
        xlfail("invalid argument");

    /* make sure this is the only argument */
    xllastarg(arg.n_ptr);

    /* find the nth element */
    for (; n > 1; n--) {
        list.n_ptr = list.n_ptr->n_listnext;
        if (list.n_ptr == NULL || list.n_ptr->n_type != LIST)
            xlfail("invalid argument");
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the list nth list element */
    return (list.n_ptr->n_listvalue);
}

/* length - return the length of a list */
static struct node *length(args)
  struct node *args;
{
    struct node *oldstk,list,*val;
    int n;

    /* create a new stack frame */
    oldstk = xlsave(&list,NULL);

    /* get the list */
    list.n_ptr = xlevmatch(LIST,&args);

    /* make sure this is the only argument */
    xllastarg(args);

    /* find the length */
    for (n = 0; list.n_ptr != NULL; n++)
        list.n_ptr = list.n_ptr->n_listnext;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* create the value node */
    val = newnode(INT);
    val->n_int = n;

    /* return the length */
    return (val);
}

/* append - builtin function append */
static struct node *append(args)
  struct node *args;
{
    struct node *oldstk,arg,list,last,val,*lptr;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&list,&last,&val,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* evaluate and append each argument */
    while (arg.n_ptr != NULL) {

        /* evaluate the next argument */
        list.n_ptr = xlevmatch(LIST,&arg.n_ptr);

        /* append each element of this list to the result list */
        while (list.n_ptr != NULL && list.n_ptr->n_type == LIST) {

            /* append this element */
            lptr = newnode(LIST);
            if (last.n_ptr == NULL)
                val.n_ptr = lptr;
            else
                last.n_ptr->n_listnext = lptr;
            lptr->n_listvalue = list.n_ptr->n_listvalue;

            /* save the new last element */
            last.n_ptr = lptr;

            /* move to the next element */
            list.n_ptr = list.n_ptr->n_listnext;
        }

        /* make sure the list ended in a nil */
        if (list.n_ptr != NULL)
            xlfail("bad list");
    }

    /* restore previous stack frame */
    xlstack = oldstk;

    /* return the list */
    return (val.n_ptr);
}

/* reverse - builtin function reverse */
static struct node *reverse(args)
  struct node *args;
{
    struct node *oldstk,list,val,*lptr;

    /* create a new stack frame */
    oldstk = xlsave(&list,&val,NULL);

    /* get the list to reverse */
    list.n_ptr = xlevmatch(LIST,&args);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* append each element of this list to the result list */
    while (list.n_ptr != NULL && list.n_ptr->n_type == LIST) {

        /* append this element */
        lptr = newnode(LIST);
        lptr->n_listvalue = list.n_ptr->n_listvalue;
        lptr->n_listnext = val.n_ptr;
        val.n_ptr = lptr;

        /* move to the next element */
        list.n_ptr = list.n_ptr->n_listnext;
    }

    /* make sure the list ended in a nil */
    if (list.n_ptr != NULL)
        xlfail("bad list");

    /* restore previous stack frame */
    xlstack = oldstk;

    /* return the list */
    return (val.n_ptr);
}

/* cons - construct a new list cell */
static struct node *cons(args)
  struct node *args;
{
    struct node *oldstk,arg,arg1,arg2,*lptr;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&arg1,&arg2,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* first argument */
    arg1.n_ptr = xlevarg(&arg.n_ptr);

    /* second argument */
    arg2.n_ptr = xlevarg(&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* construct a new list element */
    lptr = newnode(LIST);
    lptr->n_listvalue = arg1.n_ptr;
    lptr->n_listnext  = arg2.n_ptr;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the list */
    return (lptr);
}

/* xllinit - xlisp list initialization routine */
xllinit()
{
    /* define some symbols */
    t = xlenter("t");
    a_subr = xlenter("SUBR");
    a_list = xlenter("LIST");
    a_sym = xlenter("SYM");
    a_int = xlenter("INT");
    a_str = xlenter("STR");
    a_obj = xlenter("OBJ");
    a_fptr = xlenter("FPTR");
    a_kmap = xlenter("KMAP");

    /* functions with reasonable names */
    xlsubr("head",head);
    xlsubr("tail",tail);
    xlsubr("nth",nth);

    /* real lisp functions */
    xlsubr("atom",atom);
    xlsubr("eq",eq);
    xlsubr("equal",equal);
    xlsubr("null",null);
    xlsubr("type",type);
    xlsubr("listp",listp);
    xlsubr("cond",cond);
    xlsubr("list",xlist);
    xlsubr("cons",cons);
    xlsubr("car",head);
    xlsubr("cdr",tail);
    xlsubr("append",append);
    xlsubr("reverse",reverse);
    xlsubr("length",length);
}