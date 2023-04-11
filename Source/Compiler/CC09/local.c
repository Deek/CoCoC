/*
     Modification history for local.c:
          24-May-83      Added direct page initializers.
                         Make dumpstrings use fopen() update mode.
                         Made stklab be DIRECT.
          25-May-83      Make better use of register variables.
          08-Jun-83      Profile option sometimes output garbage
                         to the asm file if function name was exactly 8
                         chars.
          09-Jun-83      Made tidy() exit with signal code.
          18-Aug-83      Add conditional to dump function names to stdout
          01-Sep-83      Put "fail" directive in asmb output for errors
*/
#include <errno.h>

#include "cj.h"

#define GBLB ':'    /* global asm declarator */
#define LCLB ' '    /* local asm declarator */

static direct unsigned stklab;       /* label for stack check equ value */



epilogue()
{
     dumpstrings();
     endsect();
     if(errcount)
          ol("fail source errors");
}


locstat(l,size,area)
{
     vsect(area);
     olbl(l);
     fprintf(code," rmb %d\n",size);
     endsect();
}


defglob(ptr,size,area)
{
     vsect(area);
     defvar(ptr,size,GBLB);
     endsect();
}


extstat(ptr,size,area)
{
     vsect(area);
     defvar(ptr,size,LCLB);
     endsect();
}


defvar(ptr,size,scope)
symnode *ptr;
char scope;
{
     fprintf(code,"%.8s%c rmb %d\n",ptr->sname,scope,size);
}


#ifdef PROF
profname(name,lab)
char *name;
{
     olbl(lab);
     fprintf(code," fcc \"%.8s\"\n fcb 0\n",name);
}
#endif


#ifdef REGPARMS
# ifdef PROF
startfunc(name,flag,paramreg,lab)
# else
startfunc(name,flag,paramreg)
# endif
#else
# ifdef PROF
startfunc(name,flag,lab)
# else
startfunc(name,flag)
# endif
#endif
register char *name;
#ifdef REGPARMS
int paramreg;
#endif
{
#ifdef FUNCNAME
    extern direct int fnline;

    fprintf(code," ttl %.8s,%.10s,%d\n",name,filename,fnline);
#else
    fprintf(code," ttl %.8s\n",name);
#endif
    nlabel(name,flag);

#ifdef REGPARMS
    /* push possible register variables */
    if (paramreg == DREG) ol("pshs d,u");
    else ol("pshs u");
    if (paramreg == UREG) ol("tfr d,u");
# ifdef USE_YREG
#  error "USE_YREG not compatible with REGPARMS at this time."
# endif
#else
# ifdef USE_YREG
    ol("pshs y,u");
# else
    ol("pshs u");
# endif
#endif

    if (!sflag)
        fprintf(code," ldd #_%d\n lbsr _stkcheck\n",stklab=getlabel());

#ifdef PROF
     if(pflag) {
          ot("leax ");
          olbl(lab);
          os(",pcr\n pshs x\n leax ");
          on(name);
          os(",pcr\n pshs x\n lbsr _prof\n leas 4,s\n");
     }
#endif
}


endfunc()
{
        /* generate stack reservation value */
     if(!sflag)
          fprintf(code,"_%d equ %d\n\n",stklab,maxpush-callflag-64);
}


defbyte()
{
        ot("fcb ");
}


defword()
{
        ot("fdb ");
}


comment()
{
        os("* ");
}


vsect(area)
{
     ol(area ? "vsect dp" : "vsect");
}


endsect()
{
     ol("endsect");
}


/*
dumpstrings()
{
     register int c;

     if(strfile) {
          fclose(strfile);
          if((strfile=fopen(strname,"r")) == NULL)
               fatal("can't read strings file");
          while((c = getc(strfile)) != EOF)
               putc(c,code);
          fclose(strfile);
          unlink(strname);
     }
}
*/
dumpstrings()
{
     register int c;

     if(strfile) {
          rewind(strfile);
          while((c = getc(strfile)) != EOF)
               putc(c,code);
          if(ferror(strfile)) fatal("dumpstrings");
          fclose(strfile);
          unlink(strname);
     }
}


tidy()
{
     int err = errno ? errno : 1;

     if(strfile) {
          fclose(strfile);
          unlink(strname);
     }
     _exit(err);
}

