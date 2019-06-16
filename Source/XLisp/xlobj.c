/* xlobj - xlisp object functions */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* global variables */
struct node *self;

/* external variables */
extern struct node *xlstack;
extern struct node *xlenv;

/* local variables */
static struct node *class;
static struct node *object;
static struct node *new;
static struct node *isnew;
static struct node *msgcls;
static struct node *msgclass;
static int varcnt;

/* instance variable numbers for the class 'Class' */
#define MESSAGES        0       /* list of messages */
#define IVARS           1       /* list of instance variable names */
#define CVARS           2       /* list of class variable names */
#define CVALS           3       /* list of class variable values */
#define SUPERCLASS      4       /* pointer to the superclass */
#define IVARCNT         5       /* number of class instance variables */
#define IVARTOTAL       6       /* total number of instance variables */

/* number of instance variables for the class 'Class' */
#define CLASSSIZE       7

/* forward declarations (the extern hack is because of decusc) */
extern struct node *findmsg();
extern struct node *findvar();
extern struct node *defvars();
extern struct node *makelist();

/* xlclass - define a class */
struct node *xlclass(name,vcnt)
  char *name; int vcnt;
{
    struct node *sym,*cls;

    /* create the class */
    sym = xlenter(name);
    cls = sym->n_symvalue = newnode(OBJ);
    cls->n_obclass = class;
    cls->n_obdata = makelist(CLASSSIZE);

    /* set the instance variable counts */
    if (vcnt > 0) {
        (xlivar(cls,IVARCNT)->n_listvalue = newnode(INT))->n_int = vcnt;
        (xlivar(cls,IVARTOTAL)->n_listvalue = newnode(INT))->n_int = vcnt;
    }

    /* set the superclass to 'Object' */
    xlivar(cls,SUPERCLASS)->n_listvalue = object;

    /* return the new class */
    return (cls);
}

/* xlmfind - find the message binding for a message to an object */
struct node *xlmfind(obj,msym)
  struct node *obj,*msym;
{
    return (findmsg(obj->n_obclass,msym));
}

/* xlxsend - send a message to an object */
struct node *xlxsend(obj,msg,args)
  struct node *obj,*msg,*args;
{
    struct node *oldstk,method,cptr,val,*isnewmsg,*oldenv;

    /* save the old environment */
    oldenv = xlenv;

    /* create a new stack frame */
    oldstk = xlsave(&method,&cptr,&val,NULL);

    /* get the method for this message */
    method.n_ptr = msg->n_msgcode;

    /* make sure its a function or a subr */
    if (method.n_ptr->n_type != SUBR && method.n_ptr->n_type != LIST)
        xlfail("bad method");

    /* bind the symbols 'self' and 'msgclass' */
    xlbind(self,obj);
    xlbind(msgclass,msgcls);

    /* evaluate the function call */
    if (method.n_ptr->n_type == SUBR) {
        xlfixbindings(oldenv);
        val.n_ptr = (*method.n_ptr->n_subr)(args);
    }
    else {

        /* bind the formal arguments */
        xlabind(method.n_ptr->n_listvalue,args);
        xlfixbindings(oldenv);

        /* execute the code */
        cptr.n_ptr = method.n_ptr->n_listnext;
        while (cptr.n_ptr != NULL)
            val.n_ptr = xlevarg(&cptr.n_ptr);
    }

    /* restore the environment */
    xlunbind(oldenv);

    /* after creating an object, send it the "isnew" message */
    if (msg->n_msg == new && val.n_ptr != NULL) {
        if ((isnewmsg = xlmfind(val.n_ptr,isnew)) == NULL)
            xlfail("no method for the isnew message");
        val.n_ptr = xlxsend(val.n_ptr,isnewmsg,args);
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the result value */
    return (val.n_ptr);
}

/* xlsend - send a message to an object (message in arg list) */
struct node *xlsend(obj,args)
  struct node *obj,*args;
{
    struct node *msg;

    /* find the message binding for this message */
    if ((msg = xlmfind(obj,xlevmatch(SYM,&args))) == NULL)
        xlfail("no method for this message");

    /* send the message */
    return (xlxsend(obj,msg,args));
}

/* xlobsym - find a class or instance variable for the current object */
struct node *xlobsym(sym)
  struct node *sym;
{
    struct node *obj;

    if ((obj = self->n_symvalue) != NULL && obj->n_type == OBJ)
        return (findvar(obj,sym));
    else
        return (NULL);
}

/* mnew - create a new object instance */
static struct node *mnew()
{
    struct node *oldstk,obj,*cls;

    /* create a new stack frame */
    oldstk = xlsave(&obj,NULL);

    /* get the class */
    cls = self->n_symvalue;

    /* generate a new object */
    obj.n_ptr = newnode(OBJ);
    obj.n_ptr->n_obclass = cls;
    obj.n_ptr->n_obdata = makelist(getivcnt(cls,IVARTOTAL));

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the new object */
    return (obj.n_ptr);
}

/* misnew - initialize a new class */
static struct node *misnew(args)
  struct node *args;
{
    struct node *oldstk,super,*obj;

    /* create a new stack frame */
    oldstk = xlsave(&super,NULL);

    /* get the superclass if there is one */
    if (args != NULL)
        super.n_ptr = xlevmatch(OBJ,&args);
    else
        super.n_ptr = object;

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* get the object */
    obj = self->n_symvalue;

    /* store the superclass */
    xlivar(obj,SUPERCLASS)->n_listvalue = super.n_ptr;
    (xlivar(obj,IVARTOTAL)->n_listvalue = newnode(INT))->n_int =
         getivcnt(super.n_ptr,IVARTOTAL);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the new object */
    return (obj);
}

/* xladdivar - enter an instance variable */
xladdivar(cls,var)
  struct node *cls; char *var;
{
    struct node *ivar,*lptr;

    /* find the 'ivars' instance variable */
    ivar = xlivar(cls,IVARS);

    /* add the instance variable */
    lptr = newnode(LIST);
    lptr->n_listnext = ivar->n_listvalue;
    ivar->n_listvalue = lptr;
    lptr->n_listvalue = xlenter(var);
}

/* entermsg - add a message to a class */
static struct node *entermsg(cls,msg)
  struct node *cls,*msg;
{
    struct node *ivar,*lptr,*mptr;

    /* find the 'messages' instance variable */
    ivar = xlivar(cls,MESSAGES);

    /* lookup the message */
    for (lptr = ivar->n_listvalue; lptr != NULL; lptr = lptr->n_listnext)
        if ((mptr = lptr->n_listvalue)->n_msg == msg)
            return (mptr);

    /* allocate a new message entry if one wasn't found */
    lptr = newnode(LIST);
    lptr->n_listnext = ivar->n_listvalue;
    ivar->n_listvalue = lptr;
    lptr->n_listvalue = mptr = newnode(LIST);
    mptr->n_msg = msg;

    /* return the symbol node */
    return (mptr);
}

/* answer - define a method for answering a message */
static struct node *answer(args)
  struct node *args;
{
    struct node *oldstk,arg,msg,fargs,code;
    struct node *obj,*mptr,*fptr;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&msg,&fargs,&code,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* message symbol */
    msg.n_ptr = xlevmatch(SYM,&arg.n_ptr);

    /* get the formal argument list */
    fargs.n_ptr = xlevmatch(LIST,&arg.n_ptr);

    /* get the code */
    code.n_ptr = xlevmatch(LIST,&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* get the object node */
    obj = self->n_symvalue;

    /* make a new message list entry */
    mptr = entermsg(obj,msg.n_ptr);

    /* setup the message node */
    mptr->n_msgcode = fptr = newnode(LIST);
    fptr->n_listvalue = fargs.n_ptr;
    fptr->n_listnext = code.n_ptr;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the object */
    return (obj);
}

/* mivars - define the list of instance variables */
static struct node *mivars(args)
  struct node *args;
{
    struct node *cls,*super;
    int scnt;

    /* define the list of instance variables */
    cls = defvars(args,IVARS);

    /* get the superclass instance variable count */
    if ((super = xlivar(cls,SUPERCLASS)->n_listvalue) != NULL)
        scnt = getivcnt(super,IVARTOTAL);
    else
        scnt = 0;

    /* save the number of instance variables */
    (xlivar(cls,IVARCNT)->n_listvalue = newnode(INT))->n_int = varcnt;
    (xlivar(cls,IVARTOTAL)->n_listvalue = newnode(INT))->n_int = scnt+varcnt;

    /* return the class */
    return (cls);
}

/* getivcnt - get the number of instance variables for a class */
static int getivcnt(cls,ivar)
  struct node *cls; int ivar;
{
    struct node *cnt;

    if ((cnt = xlivar(cls,ivar)->n_listvalue) != NULL)
        if (cnt->n_type == INT)
            return (cnt->n_int);
        else
            xlfail("bad value for instance variable count");
    else
        return (0);
}

/* mcvars - define the list of class variables */
static struct node *mcvars(args)
  struct node *args;
{
    struct node *cls;

    /* define the list of class variables */
    cls = defvars(args,CVARS);

    /* make a new list of values */
    xlivar(cls,CVALS)->n_listvalue = makelist(varcnt);

    /* return the class */
    return (cls);
}

/* defvars - define a class or instance variable list */
static struct node *defvars(args,varnum)
  struct node *args; int varnum;
{
    struct node *oldstk,vars,*vptr,*cls,*sym;

    /* create a new stack frame */
    oldstk = xlsave(&vars,NULL);

    /* get ivar list */
    vars.n_ptr = xlevmatch(LIST,&args);

    /* make sure there aren't any more arguments */
    xllastarg(args);

    /* get the class node */
    cls = self->n_symvalue;

    /* check each variable in the list */
    varcnt = 0;
    for (vptr = vars.n_ptr;
         vptr != NULL && vptr->n_type == LIST;
         vptr = vptr->n_listnext) {

        /* make sure this is a valid symbol in the list */
        if ((sym = vptr->n_listvalue) == NULL || sym->n_type != SYM)
            xlfail("bad variable list");

        /* make sure its not already defined */
        if (checkvar(cls,sym))
            xlfail("multiply defined variable");

        /* count the variable */
        varcnt++;
    }

    /* make sure the list ended properly */
    if (vptr != NULL)
        xlfail("bad variable list");

    /* define the new variable list */
    xlivar(cls,varnum)->n_listvalue = vars.n_ptr;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the class */
    return (cls);
}

/* xladdmsg - add a message to a class */
xladdmsg(cls,msg,code)
  struct node *cls; char *msg; struct node *(*code)();
{
    struct node *mptr;

    /* enter the message selector */
    mptr = entermsg(cls,xlenter(msg));

    /* store the method for this message */
    mptr->n_msgcode = newnode(SUBR);
    mptr->n_msgcode->n_subr = code;
}

/* getclass - get the class of an object */
static struct node *getclass(args)
  struct node *args;
{
    /* make sure there aren't any arguments */
    xllastarg(args);

    /* return the object's class */
    return (self->n_symvalue->n_obclass);
}

/* obprint - print an object */
static struct node *obprint(args)
  struct node *args;
{
    /* make sure there aren't any arguments */
    xllastarg(args);

    /* print the object */
    printf("<Object: #%o>",self->n_symvalue);

    /* return the object */
    return (self->n_symvalue);
}

/* obshow - show the instance variables of an object */
static struct node *obshow(args)
  struct node *args;
{
    /* make sure there aren't any arguments */
    xllastarg(args);

    /* print the object's instance variables */
    xlprint(self->n_symvalue->n_obdata,TRUE);

    /* return the object */
    return (self->n_symvalue);
}

/* defisnew - default 'isnew' method */
static struct node *defisnew(args)
  struct node *args;
{
    /* make sure there aren't any arguments */
    xllastarg(args);

    /* return the object */
    return (self->n_symvalue);
}

/* sendsuper - send a message to an object's superclass */
static struct node *sendsuper(args)
  struct node *args;
{
    struct node *obj,*super,*msg;

    /* get the object */
    obj = self->n_symvalue;

    /* get the object's superclass */
    super = xlivar(obj->n_obclass,SUPERCLASS)->n_listvalue;

    /* find the message binding for this message */
    if ((msg = findmsg(super,xlevmatch(SYM,&args))) == NULL)
        xlfail("no method for this message");

    /* send the message */
    return (xlxsend(obj,msg,args));
}

/* findmsg - find the message binding given an object and a class */
static struct node *findmsg(cls,sym)
  struct node *cls,*sym;
{
    struct node *lptr,*msg;

    /* start at the specified class */
    msgcls = cls;

    /* look for the message in the class or superclasses */
    while (msgcls != NULL) {

        /* lookup the message in this class */
        for (lptr = xlivar(msgcls,MESSAGES)->n_listvalue;
             lptr != NULL;
             lptr = lptr->n_listnext)
            if ((msg = lptr->n_listvalue) != NULL && msg->n_msg == sym)
                return (msg);

        /* look in class's superclass */
        msgcls = xlivar(msgcls,SUPERCLASS)->n_listvalue;
    }

    /* message not found */
    return (NULL);
}

/* findvar - find a class or instance variable */
static struct node *findvar(obj,sym)
  struct node *obj,*sym;
{
    struct node *cls,*lptr;
    int base,varnum;
    int found;

    /* get the class of the object */
    cls = obj->n_obclass;

    /* get the total number of instance variables */
    base = getivcnt(cls,IVARTOTAL);

    /* find the variable */
    found = FALSE;
    for (; cls != NULL; cls = xlivar(cls,SUPERCLASS)->n_listvalue) {

        /* get the number of instance variables for this class */
        if ((base -= getivcnt(cls,IVARCNT)) < 0)
            xlfail("error finding instance variable");

        /* check for finding the class of the current message */
        if (!found && cls == msgclass->n_symvalue)
            found = TRUE;

        /* lookup the instance variable */
        varnum = 0;
        for (lptr = xlivar(cls,IVARS)->n_listvalue;
             lptr != NULL;
             lptr = lptr->n_listnext)
            if (found && lptr->n_listvalue == sym)
                return (xlivar(obj,base + varnum));
            else
                varnum++;

        /* skip the class variables if the message class hasn't been found */
        if (!found)
            continue;

        /* lookup the class variable */
        varnum = 0;
        for (lptr = xlivar(cls,CVARS)->n_listvalue;
             lptr != NULL;
             lptr = lptr->n_listnext)
            if (lptr->n_listvalue == sym)
                return (xlcvar(cls,varnum));
            else
                varnum++;
    }

    /* variable not found */
    return (NULL);
}

/* checkvar - check for an existing class or instance variable */
static int checkvar(cls,sym)
  struct node *cls,*sym;
{
    struct node *lptr;

    /* find the variable */
    for (; cls != NULL; cls = xlivar(cls,SUPERCLASS)->n_listvalue) {

        /* lookup the instance variable */
        for (lptr = xlivar(cls,IVARS)->n_listvalue;
             lptr != NULL;
             lptr = lptr->n_listnext)
            if (lptr->n_listvalue == sym)
                return (TRUE);

        /* lookup the class variable */
        for (lptr = xlivar(cls,CVARS)->n_listvalue;
             lptr != NULL;
             lptr = lptr->n_listnext)
            if (lptr->n_listvalue == sym)
                return (TRUE);
    }

    /* variable not found */
    return (FALSE);
}

/* xlivar - get an instance variable */
struct node *xlivar(obj,num)
  struct node *obj; int num;
{
    struct node *ivar;

    /* get the instance variable */
    for (ivar = obj->n_obdata; num > 0; num--)
        if (ivar != NULL)
            ivar = ivar->n_listnext;
        else
            xlfail("bad instance variable list");

    /* return the instance variable */
    return (ivar);
}

/* xlcvar - get a class variable */
struct node *xlcvar(cls,num)
  struct node *cls; int num;
{
    struct node *cvar;

    /* get the class variable */
    for (cvar = xlivar(cls,CVALS)->n_listvalue; num > 0; num--)
        if (cvar != NULL)
            cvar = cvar->n_listnext;
        else
            xlfail("bad class variable list");

    /* return the class variable */
    return (cvar);
}

/* makelist - make a list of nodes */
static struct node *makelist(cnt)
  int cnt;
{
    struct node *oldstk,list,*lnew;

    /* create a new stack frame */
    oldstk = xlsave(&list,NULL);

    /* make the list */
    for (; cnt > 0; cnt--) {
        lnew = newnode(LIST);
        lnew->n_listnext = list.n_ptr;
        list.n_ptr = lnew;
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the list */
    return (list.n_ptr);
}

/* xloinit - object function initialization routine */
xloinit()
{
    /* don't confuse the garbage collector */
    class = NULL;
    object = NULL;

    /* enter the object related symbols */
    new = xlenter("new");
    isnew = xlenter("isnew");
    self = xlenter("self");
    msgclass = xlenter("msgclass");

    /* create the 'Class' object */
    class = xlclass("Class",CLASSSIZE);
    class->n_obclass = class;

    /* create the 'Object' object */
    object = xlclass("Object",0);

    /* finish initializing 'class' */
    xlivar(class,SUPERCLASS)->n_listvalue = object;
    xladdivar(class,"ivartotal");       /* ivar number 6 */
    xladdivar(class,"ivarcnt");         /* ivar number 5 */
    xladdivar(class,"superclass");      /* ivar number 4 */
    xladdivar(class,"cvals");           /* ivar number 3 */
    xladdivar(class,"cvars");           /* ivar number 2 */
    xladdivar(class,"ivars");           /* ivar number 1 */
    xladdivar(class,"messages");        /* ivar number 0 */
    xladdmsg(class,"new",mnew);
    xladdmsg(class,"answer",answer);
    xladdmsg(class,"ivars",mivars);
    xladdmsg(class,"cvars",mcvars);
    xladdmsg(class,"isnew",misnew);

    /* finish initializing 'object' */
    xladdmsg(object,"class",getclass);
    xladdmsg(object,"print",obprint);
    xladdmsg(object,"show",obshow);
    xladdmsg(object,"isnew",defisnew);
    xladdmsg(object,"sendsuper",sendsuper);
}