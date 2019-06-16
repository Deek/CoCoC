/* xleval - xlisp evaluator */

#ifdef AZTEC
#include "a:stdio.h"
#include "a:setjmp.h"
#else
#include <stdio.h>
#include <setjmp.h>
#endif

#include "xlisp.h"

/* global variables */
struct node *xlstack;

/* trace stack */
static struct node *trace_stack[TDEPTH];
static int trace_pointer;

/* external variables */
extern jmp_buf xljmpbuf;
extern struct node *xlenv;

/* local variables */
static struct node *slash;

/* forward declarations (the extern hack is for decusc) */
extern struct node *evlist();
extern struct node *evsym();
extern struct node *evfun();

/* eval - the builtin function 'eval' */
static struct node *eval(args)
  struct node *args;
{
    struct node *oldstk,expr,*val;

    /* create a new stack frame */
    oldstk = xlsave(&expr,NULL);

    /* get the expression to evaluate */
    expr.n_ptr = xlevarg(&args);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* evaluate the expression */
    val = xleval(expr.n_ptr);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the expression evaluated */
    return (val);
}

/* xleval - evaluate an xlisp expression */
struct node *xleval(expr)
  struct node *expr;
{
    /* evaluate null to itself */
    if (expr == NULL)
        return (NULL);

    /* check type of value */
    switch (expr->n_type) {
    case LIST:
            return (evlist(expr));
    case SYM:
            return (evsym(expr));
    case INT:
    case STR:
    case SUBR:
            return (expr);
    default:
            xlfail("can't evaluate expression");
    }
}

/* xlsave - save nodes on the stack */
struct node *xlsave(n)
  struct node *n;
{
    struct node **nptr,*oldstk;

    /* save the old stack pointer */
    oldstk = xlstack;

    /* save each node */
    for (nptr = &n; *nptr != NULL; nptr++) {
        (*nptr)->n_type = LIST;
        (*nptr)->n_listvalue = NULL;
        (*nptr)->n_listnext = xlstack;
        xlstack = *nptr;
    }

    /* return the old stack pointer */
    return (oldstk);
}

/* evlist - evaluate a list */
static struct node *evlist(nptr)
  struct node *nptr;
{
    struct node *oldstk,fun,args,*val;

    /* create a stack frame */
    oldstk = xlsave(&fun,&args,NULL);

    /* get the function and the argument list */
    fun.n_ptr = nptr->n_listvalue;
    args.n_ptr = nptr->n_listnext;

    /* add trace entry */
    tpush(nptr);

    /* evaluate the first expression */
    if ((fun.n_ptr = xleval(fun.n_ptr)) == NULL)
        xlfail("null function");

    /* evaluate the function */
    switch (fun.n_ptr->n_type) {
    case SUBR:
            val = (*fun.n_ptr->n_subr)(args.n_ptr);
            break;
    case LIST:
            val = evfun(fun.n_ptr,args.n_ptr);
            break;
    case OBJ:
            val = xlsend(fun.n_ptr,args.n_ptr);
            break;
    default:
            xlfail("bad function");
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* remove trace entry */
    tpop();

    /* return the result value */
    return (val);
}

/* evsym - evaluate a symbol */
static struct node *evsym(sym)
  struct node *sym;
{
    struct node *lptr;

    /* check for a current object */
    if ((lptr = xlobsym(sym)) != NULL)
        return (lptr->n_listvalue);
    else
        return (sym->n_symvalue);
}

/* evfun - evaluate a function */
static struct node *evfun(fun,args)
  struct node *fun,*args;
{
    struct node *oldenv,*oldstk,cptr,*fargs,*val;

    /* create a stack frame */
    oldstk = xlsave(&cptr,NULL);

    /* get the formal argument list */
    if ((fargs = fun->n_listvalue) != NULL && fargs->n_type != LIST)
        xlfail("bad formal argument list");

    /* bind the formal parameters */
    oldenv = xlenv;
    xlabind(fargs,args);
    xlfixbindings(oldenv);

    /* execute the code */
    for (cptr.n_ptr = fun->n_listnext; cptr.n_ptr != NULL; )
        val = xlevarg(&cptr.n_ptr);

    /* restore the environment */
    xlunbind(oldenv);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result value */
    return (val);
}

/* xlabind - bind the arguments for a function */
xlabind(fargs,aargs)
  struct node *fargs,*aargs;
{
    struct node *oldstk,farg,aarg,val;

    /* create a stack frame */
    oldstk = xlsave(&farg,&aarg,&val,NULL);

    /* initialize the pointers */
    farg.n_ptr = fargs;
    aarg.n_ptr = aargs;

    /* evaluate and bind each argument */
    while (farg.n_ptr != NULL && aarg.n_ptr != NULL) {

        /* check for local variable separator */
        if (farg.n_ptr->n_listvalue == slash)
            break;

        /* evaluate the argument */
        val.n_ptr = xlevarg(&aarg.n_ptr);

        /* bind the formal variable to the argument value */
        xlbind(farg.n_ptr->n_listvalue,val.n_ptr);

        /* move the formal argument list pointer ahead */
        farg.n_ptr = farg.n_ptr->n_listnext;
    }

    /* check for local variables */
    if (farg.n_ptr != NULL && farg.n_ptr->n_listvalue == slash)
        while ((farg.n_ptr = farg.n_ptr->n_listnext) != NULL)
            xlbind(farg.n_ptr->n_listvalue,NULL);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* make sure the correct number of arguments were supplied */
    if (farg.n_ptr != aarg.n_ptr)
        xlfail("incorrect number of arguments to a function");
}

/* xlfail - error handling routine */
xlfail(err)
  char *err;
{
    /* print the error message */
    printf("error: %s\n",err);

    /* unbind bound symbols */
    xlunbind(NULL);

    /* restore input to the terminal */
    xltin(TRUE);

    /* do the back trace */
    trace();
    trace_pointer = -1;

    /* restart */
    longjmp(xljmpbuf,1);
}

/* tpush - add an entry to the trace stack */
static tpush(nptr)
    struct node *nptr;
{
    if (++trace_pointer < TDEPTH)
        trace_stack[trace_pointer] = nptr;
}

/* tpop - pop an entry from the trace stack */
static tpop()
{
    trace_pointer--;
}

/* trace - do a back trace */
static trace()
{
    for (; trace_pointer >= 0; trace_pointer--)
        if (trace_pointer < TDEPTH) {
            xlprint(trace_stack[trace_pointer],TRUE);
            putchar('\n');
        }
}

/* xleinit - initialize the evaluator */
xleinit()
{
    /* enter the local variable separator symbol */
    slash = xlenter("/");

    /* initialize debugging stuff */
    trace_pointer = -1;

    /* builtin functions */
    xlsubr("eval",eval);
}