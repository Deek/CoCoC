/* xlkmap - xlisp key map functions */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#endif

#include "xlisp.h"

/* external variables */
extern struct node *xlstack;
extern struct node *xlenv;
extern struct node *self;

/* local definitions */
#define KMSIZE  256     /* number of characters in a keymap */
#define KMAX    20      /* maximum number of characters in a key sequence */
#define KEYMAP  0       /* instance variable number for 'keymap' */

/* local variables */
static struct node *currentenv;

/* forward declarations (the extern hack is because of decusc) */
extern struct node *sendmsg();

/* isnew - initialize a new keymap */
static struct node *isnew(args)
  struct node *args;
{
    /* make sure there aren't any arguments */
    xllastarg(args);

    /* create a keymap node */
    xlivar(self->n_symvalue,KEYMAP)->n_listvalue = newnode(KMAP);

    /* return the keymap object */
    return (self->n_symvalue);
}

/* newkmap - allocate memory for a new key map vector */
static struct node *(*newkmap())[]
{
    struct node *(*map)[];

    /* allocate the vector */
    if ((map = (struct node *(*)[]) calloc(1,sizeof(struct node *) * KMSIZE))
                         == NULL) {
        printf("insufficient memory");
        exit();
    }

    /* return the new vector */
    return (map);
}

/* key - define a key */
static struct node *key(args)
  struct node *args;
{
    struct node *oldstk,arg,kstr,ksym,*kmap,*kmptr;
    struct node *(*map)[];
    char *sptr;
    int ch;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&kstr,&ksym,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get the keymap */
    kmap = xlivar(self->n_symvalue,KEYMAP)->n_listvalue;
    if (kmap == NULL && kmap->n_type != KMAP)
        xlfail("bad keymap object");

    /* get the key string */
    kstr.n_ptr = xlevmatch(STR,&arg.n_ptr);

    /* get the key symbol */
    ksym.n_ptr = xlevmatch(SYM,&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* process each character in the key string */
    for (kmptr = kmap, sptr = kstr.n_ptr->n_str;
         *sptr != 0;
         kmptr = (*map)[ch]) {

        /* get a character */
        ch = *sptr++;

        /* allocate a key map vector if non currently exists */
        if ((map = kmptr->n_kmap) == NULL)
            map = kmptr->n_kmap = newkmap();

        /* check for this being the last character in the string */
        if (*sptr == 0)
            (*map)[ch] = ksym.n_ptr;
        else
            if ((*map)[ch] == NULL || (*map)[ch]->n_type != KMAP) {
                (*map)[ch] = newnode(KMAP);
                (*map)[ch]->n_kmap = newkmap();
            }
    }

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the keymap object */
    return (self->n_symvalue);
}

/* process - process input characters using a key map */
static struct node *process(args)
  struct node *args;
{
    struct node *oldstk,arg,env,margs,*kmap,*kmptr,*nptr,*oldenv;
    struct node *(*map)[];
    char keys[KMAX+1];
    int ch,kndx;

    /* create a new stack frame */
    oldstk = xlsave(&arg,&env,&margs,NULL);

    /* initialize */
    arg.n_ptr = args;

    /* get the keymap */
    kmap = xlivar(self->n_symvalue,KEYMAP)->n_listvalue;
    if (kmap == NULL && kmap->n_type != KMAP)
        xlfail("bad keymap object");

    /* get the environment */
    env.n_ptr = xlevmatch(LIST,&arg.n_ptr);

    /* make sure there aren't any more arguments */
    xllastarg(arg.n_ptr);

    /* bind the current environment variable */
    oldenv = xlenv;
    xlbind(currentenv,env.n_ptr);
    xlfixbindings(oldenv);

    /* make sure the key map is defined */
    if (kmap->n_kmap == NULL)
        xlfail("empty keymap");

    /* create an argument list to send with key messages */
    margs.n_ptr = newnode(LIST);
    margs.n_ptr->n_listvalue = newnode(STR);
    margs.n_ptr->n_listvalue->n_str = keys;
    margs.n_ptr->n_listvalue->n_strtype = STATIC;

    /* character processing loop */
    for (kmptr = kmap, kndx = 0; TRUE; ) {

        /* flush pending output */
        fflush(stdout);

        /* get a character */
        if ((ch = kbin()) < 0)
            break;

        /* put it in the key sequence */
        if (kndx < KMAX)
            keys[kndx++] = ch;
        else
            xlfail("key sequence too long");

        /* dispatch on character code */
        if ((map = kmptr->n_kmap) == NULL)
            xlfail("bad keymap");
        else if ((nptr = (*map)[ch]) == NULL) {
            kmptr = kmap;
            kndx = 0;
        }
        else if (nptr->n_type == KMAP)
            kmptr = (*map)[ch];
        else if (nptr->n_type == SYM) {
            keys[kndx] = 0;
            if (sendmsg(nptr,currentenv->n_symvalue,margs.n_ptr) == NULL)
                break;
            kmptr = kmap;
            kndx = 0;
        }
        else
            xlfail("bad keymap");
    }

    /* unbind */
    xlunbind(oldenv);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the keymap object */
    return (self->n_symvalue);
}

/* sendmsg - send a message given an environment list */
static struct node *sendmsg(msym,env,args)
  struct node *msym,*env,*args;
{
    struct node *eptr,*obj,*msg;

    /* look for an object that answers the message */
    for (eptr = env; eptr != NULL; eptr = eptr->n_listnext)
        if ((obj = eptr->n_listvalue) != NULL && obj->n_type == OBJ)
            if ((msg = xlmfind(obj,msym)) != NULL)
                return (xlxsend(obj,msg,args));

    /* return the message if no object answered it */
    return (msym);
}

/* xlkmmark - mark a keymap */
xlkmmark(km)
  struct node *km;
{
    struct node *(*map)[];
    int i;

    /* mark the keymap node */
    km->n_flags |= MARK;

    /* check for a null keymap */
    if ((map = km->n_kmap) == NULL)
        return;

    /* loop through each keymap entry */
    for (i = 0; i < KMSIZE; i++)
        if (((*map)[i] != NULL) && (*map)[i]->n_type == KMAP)
            xlkmmark((*map)[i]);
}

/* xlkmfree - free a keymap */
xlkmfree(km)
  struct node *km;
{
    struct node *(*map)[];
    int i;

    /* check for a null keymap */
    if ((map = km->n_kmap) == NULL)
        return;

    /* loop through each keymap entry */
    for (i = 0; i < KMSIZE; i++)
        if (((*map)[i] != NULL) && (*map)[i]->n_type == KMAP)
            xlkmfree((*map)[i]);

    /* free this keymap */
    free(km->n_kmap);
}

/* xlkinit - key map function initialization routine */
xlkinit()
{
    struct node *keymap;

    /* define the xlisp variables */
    currentenv = xlenter("currentenv");

    /* define the keymap class */
    keymap = xlclass("Keymap",1);
    xladdivar(keymap,"keymap");
    xladdmsg(keymap,"isnew",isnew);
    xladdmsg(keymap,"key",key);
    xladdmsg(keymap,"process",process);
}