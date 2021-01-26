#include "op.h"

#ifdef UNIX
# include <unistd.h>
#endif

#define GRABSIZE 512

char *
grab(n)
unsigned n;
{
    char *ptr;

    if ((ptr = sbrk(n)) == (char *)-1) error("memory overflow");
    return (char *)ptr;
}

error(s1,s2,s3,s4)
char *s1,*s2,*s3,*s4;
{
    fprintf(stderr,"error: ");
    fprintf(stderr,s1,s2,s3,s4);
    putc('\n',stderr);
    exit(1);
}

#ifdef DEBUG
debug(fmt,s1,s2,s3,s4)
char *fmt,*s1,*s2,*s3,*s4;
{
    if (dbflag) fprintf(stderr,fmt,s1,s2,s3,s4);
}

prtins(i)
instruction *i;
{
    register label *l;

    if (dbflag) {
        fprintf(stderr,"@%p ",i);
        for (l = i->llist; l; l = l->nextl)
            fprintf(stderr,"%s%s",l->lname,l->nextl ? "/" : "");
		if (i && i->lab && i->lab->lname)
	        fprintf(stderr," %s %s\n",i->mnem,
                (i->itype & BRANCH ? i->lab->lname : i->args));
    }
}
#endif
