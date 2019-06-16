/* xlmath - xlisp builtin arithmetic functions */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* external variables */
extern struct node *xlstack;

/* local variables */
static struct node *true;

/* forward declarations (the extern hack is for decusc) */
extern struct node *arith();
extern struct node *compare();

/* add - builtin function for addition */
static int xadd(val,arg)
  int val,arg;
{
    return (val + arg);
}
static struct node *add(args)
  struct node *args;
{
    return (arith(args,xadd));
}

/* sub - builtin function for subtraction */
static int xsub(val,arg)
  int val,arg;
{
    return (val - arg);
}
static struct node *sub(args)
  struct node *args;
{
    return (arith(args,xsub));
}

/* mul - builtin function for multiplication */
static int xmul(val,arg)
  int val,arg;
{
    return (val * arg);
}
static struct node *mul(args)
  struct node *args;
{
    return (arith(args,xmul));
}

/* div - builtin function for division */
static int xdiv(val,arg)
  int val,arg;
{
    return (val / arg);
}
static struct node *div(args)
  struct node *args;
{
    return (arith(args,xdiv));
}

/* mod - builtin function for modulus */
static int xmod(val,arg)
  int val,arg;
{
    return (val % arg);
}
static struct node *mod(args)
  struct node *args;
{
    return (arith(args,xmod));
}

/* and - builtin function for modulus */
static int xand(val,arg)
  int val,arg;
{
    return (val & arg);
}
static struct node *and(args)
  struct node *args;
{
    return (arith(args,xand));
}

/* or - builtin function for modulus */
static int xor(val,arg)
  int val,arg;
{
    return (val | arg);
}
static struct node *or(args)
  struct node *args;
{
    return (arith(args,xor));
}

/* not - bitwise not */
static struct node *not(args)
  struct node *args;
{
    struct node *rval;
    int val;

    /* evaluate the argument */
    val = xlevmatch(INT,&args)->n_int;

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* convert and check the value  */
    rval = newnode(INT);
    rval->n_int = ~val;

    /* return the result value */
    return (rval);
}

/* abs - absolute value */
static struct node *abs(args)
  struct node *args;
{
    struct node *rval;
    int val;

    /* evaluate the argument */
    val = xlevmatch(INT,&args)->n_int;

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* convert and check the value  */
    rval = newnode(INT);
    rval->n_int = val >= 0 ? val : -val ;

    /* return the result value */
    return (rval);
}

/* min - builtin function for minimum */
static int xmin(val,arg)
  int val,arg;
{
    return (val < arg ? val : arg);
}
static struct node *min(args)
  struct node *args;
{
    return (arith(args,xmin));
}

/* max - builtin function for maximum */
static int xmax(val,arg)
  int val,arg;
{
    return (val > arg ? val : arg);
}
static struct node *max(args)
  struct node *args;
{
    return (arith(args,xmax));
}

/* arith - common arithmetic function */
static struct node *arith(args,funct)
  struct node *args; int (*funct)();
{
    struct node *oldstk,arg,*val;
    int first,ival,iarg;

    /* create a new stack frame */
    oldstk = xlsave(&arg,NULL);

    /* initialize */
    arg.n_ptr = args;
    first = TRUE;
    ival = 0;

    /* evaluate and sum each argument */
    while (arg.n_ptr != NULL) {

        /* get the next argument */
        iarg = xlevmatch(INT,&arg.n_ptr)->n_int;

        /* accumulate the result value */
        if (first) {
            ival = iarg;
            first = FALSE;
        }
        else
            ival = (*funct)(ival,iarg);
    }

    /* initialize value */
    val = newnode(INT);
    val->n_int = ival;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result value */
    return (val);
}

/* land - logical and */
static struct node *land(args)
  struct node *args;
{
    struct node *oldstk,arg,*val;

    /* create a new stack frame */
    oldstk = xlsave(&arg,NULL);

    /* initialize */
    arg.n_ptr = args;
    val = true;

    /* evaluate each argument */
    while (arg.n_ptr != NULL)

        /* get the next argument */
        if (xlevarg(&arg.n_ptr) == NULL) {
            val = NULL;
            break;
        }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result value */
    return (val);
}

/* lor - logical or */
static struct node *lor(args)
  struct node *args;
{
    struct node *oldstk,arg,*val;

    /* create a new stack frame */
    oldstk = xlsave(&arg,NULL);

    /* initialize */
    arg.n_ptr = args;
    val = NULL;

    /* evaluate each argument */
    while (arg.n_ptr != NULL)
        if (xlevarg(&arg.n_ptr) != NULL) {
            val = true;
            break;
        }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result value */
    return (val);
}

/* lnot - logical not */
static struct node *lnot(args)
  struct node *args;
{
    struct node *val;

    /* evaluate the argument */
    val = xlevarg(&args);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* convert and check the value  */
    if (val == NULL)
        return (true);
    else
        return (NULL);
}

/* lss - builtin function for < */
static int xlss(cmp)
  int cmp;
{
    return (cmp < 0);
}
static struct node *lss(args)
  struct node *args;
{
    return (compare(args,xlss));
}

/* leq - builtin function for <= */
static int xleq(cmp)
  int cmp;
{
    return (cmp <= 0);
}
static struct node *leq(args)
  struct node *args;
{
    return (compare(args,xleq));
}

/* eql - builtin function for == */
static int xeql(cmp)
  int cmp;
{
    return (cmp == 0);
}
static struct node *eql(args)
  struct node *args;
{
    return (compare(args,xeql));
}

/* neq - builtin function for != */
static int xneq(cmp)
  int cmp;
{
    return (cmp != 0);
}
static struct node *neq(args)
  struct node *args;
{
    return (compare(args,xneq));
}

/* geq - builtin function for >= */
static int xgeq(cmp)
  int cmp;
{
    return (cmp >= 0);
}
static struct node *geq(args)
  struct node *args;
{
    return (compare(args,xgeq));
}

/* gtr - builtin function for > */
static int xgtr(cmp)
  int cmp;
{
    return (cmp > 0);
}
static struct node *gtr(args)
  struct node *args;
{
    return (compare(args,xgtr));
}

/* compare - common compare function */
static struct node *compare(args,funct)
  struct node *args; int (*funct)();
{
    struct node *oldstk,arg,arg1,arg2;
    int type1,type2,cmp;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&arg1,&arg2,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get argument 1 */
    arg1.n_ptr = xlevarg(&arg.n_ptr);
    type1 = gettype(arg1.n_ptr);

    /* get argument 2 */
    arg2.n_ptr = xlevarg(&arg.n_ptr);
    type2 = gettype(arg2.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* do the compare */
    if (type1 == STR && type2 == STR)
        cmp = strcmp(arg1.n_ptr->n_str,arg2.n_ptr->n_str);
    else if (type1 == INT && type2 == INT)
        cmp = arg1.n_ptr->n_int - arg2.n_ptr->n_int;
    else
        cmp = arg1.n_ptr - arg2.n_ptr;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return result of the compare */
    if ((*funct)(cmp))
        return (true);
    else
        return (NULL);
}

/* gettype - return the type of an argument */
static int gettype(arg)
  struct node *arg;
{
    if (arg == NULL)
        return (LIST);
    else
        return (arg->n_type);
}

/* xlminit - xlisp math initialization routine */
xlminit()
{
    xlsubr("+",add);
    xlsubr("-",sub);
    xlsubr("*",mul);
    xlsubr("/",div);
    xlsubr("%",mod);
    xlsubr("&",and);
    xlsubr("|",or);
    xlsubr("~",not);
    xlsubr("<",lss);
    xlsubr("<=",leq);
    xlsubr("==",eql);
    xlsubr("!=",neq);
    xlsubr(">=",geq);
    xlsubr(">",gtr);
    xlsubr("&&",land);
    xlsubr("||",lor);
    xlsubr("!",lnot);
    xlsubr("min",min);
    xlsubr("max",max);
    xlsubr("abs",abs);
    true = xlenter("t");
    true->n_symvalue = true;
}