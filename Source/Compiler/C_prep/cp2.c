#include "cp.h"

gettoken(ln,b)  /* b=pos (0-...) to search ln; assumes b points to 1st */
char *ln;       /*   char of token (no error chking except EOL) */
int b;          /* ln=search $tring */
{               /* Returns last char. pos of token (offset from ln) */
int i;          /* or ERROR (-1) if b is out of range */
register char *lnptr;

    lnptr=&ln[b];
    if (b>=strlen(ln))   /* prevent starting at end of line */
        return ERROR;   /* b is outside ln range */
    if (i=findchar(lnptr,' ')) /* token terminated w/space */
        return i-2+b;
    return (strlen(ln)-1); /* token terminated by NULL */
}

toksrch(ln,tk,pv,kv,bv,b,tcnt)  /* Searches for specified token under */
char *ln,*tk;           /* specified conditions.  */
int pv,kv,bv,b,*tcnt;   /* ln = search $tring  tk = token to search for */
{                       /* pv,kv,bv = (),[],{} cntrs (++/-- for open/close */
int c,d,mflag;          /* b = start pos of srch (0-...) must be 1st char */
int pcnt,kcnt,bcnt;     /*     of token (no error chking) */
register char *lnptr;   /* pv,kv,bv must match pcnt,kcnt,bcnt for exit */
                        /* also tk must match current token for exit */
    *tcnt=0;            /* If pv,kv or bv = 99 then that flag can have any */
    pcnt=kcnt=bcnt=0;   /* value upon exit (i.e. wildcard)  */
    do                  /* Returns pos of first char in token or -1 (fails) */
    {
        mflag=0;    /* no match */
/*      fprintf(stderr,"TOKSRCH: b4 gettoken\n");   */
        if ((c=gettoken(ln,b))==ERROR)
            return ERROR;   /* no match */
        ++*tcnt;    /* inc token counter */
        lnptr=&ln[b];
        if (!strncmp(lnptr,tk,max(c-b+1,strlen(tk))))
            ++mflag;    /* token match found */
        if (c==b)   /* token only 1 char */
        {
            if (*lnptr=='(')
                ++pcnt;
            else if (*lnptr==')')
                --pcnt;
            else if (*lnptr=='[')
                ++kcnt;
            else if (*lnptr==']')
                --kcnt;
            else if (*lnptr=='{')
                ++bcnt;
            else if (*lnptr=='}')
                --bcnt;
        }
        d=b;    /* save first char of token */
        b=c+2;      /* point to next token */
        if (pv==99)     /* if any of these are 99 they automatically match */
            pcnt=99;
        if (kv==99)
            kcnt=99;
        if (bv==99)
            bcnt=99;
    } while (!mflag || pcnt!=pv || kcnt!=kv || bcnt!=bv);
    return d;   /* return pos of first letter in token */
}

tstargs(i)  /* count # of args for defargs[i] */
int i;
{
char *ptr;
int b;

    b=0;
    ptr=defarg[i];
    while (*ptr)
    {
        if (*(ptr++)==' ')
            ++b;    /* # of spaces = # of args */
    }
 /*
    fprintf(stderr,"TSTARGS aft: defarg=|%s|\n",defarg[i]);
    fprintf(stderr,"TSTARGS aft: b=%d\n",b);
*/
    return b;
}

addqmac(mac)    /* Adds " to macro when # is found */
char *mac;
{
char buf[MAX_LENGTH],*bptr;
register char *mptr;

    bptr=buf;
    mptr=mac;
    *(bptr++)='"';
    while (*mptr)
    {
        if (*mptr=='\"' || *mptr=='\\')
            *(bptr++)='\\';
        if (*mptr!=' ')
            *(bptr++)=*(mptr++);
        else
        {
            *(bptr++)=VTAB;     /* convert spaces to VTAB */
            ++mptr;
        }
    }
    *(bptr++)='\"';
    *bptr=0;
    strcpy(mac,buf);
}

char *getoknum(tok,buf,num)     /* Counts tokens in tok[] for token # num */
char *tok,*buf;                 /* and stores it in buf[].  Returns NULL  */
int num;                        /* on failure to locate token # or will   */
{                               /* return ptr to buf[] */
int b,c,i;                      /* num must be 1-... */

    c=-2;
    i=0;
    if (num<=0)
        return NULL;    /* token # out of range */
    while (i++<num)
    {
        b=c+2;
        if ((c=gettoken(tok,b))==ERROR)
            return NULL;
    }
    strncpy(buf,&tok[b],c-b+1);
    buf[c-b+1]=0;
    return buf;
}

putdtbl(b,c)    /* Given B: Start pos. of identifier/ C: end pos. */
int b,c;        /* Puts identifier in define $trng tbl */
{
register int i;

    for (i=b;i<=c;++i)
    {
        if (dptr<&dstrtbl[STRNG_TBL_MAX-1])    /* test $trng table */
            *dptr++=line[i];
        else
            doerr(5,i);     /* $trng table full -- FATAL ERROR */
    }
    *dptr++=0;      /* NULL terminate $trng */
/*  fprintf(stderr,"dtbl bytes=%d\n",dptr-dstrtbl); */
}

putiddtbl(b,d)
int b,d;
{
register int i;
    if (d-b>30)     /* If identifier length is >31 ... */
        d=b+30;     /* set identifier length = 31 */
    for (i=b;i<=d;++i)  /* Put def name in $trng table */
    {
        if (dptr<&dstrtbl[STRNG_TBL_MAX-2])    /* test $trng table */
            *dptr++=line[i];
        else
            doerr(5,i);     /* $trng table full -- FATAL ERROR */
    }
    *dptr++=' ';    /* add ending space */
    *dptr++=0;      /* NULL terminate $trng */
}


dodefine(a,b)   /* Given A:strlen, B: pos. of char. after 1st space */
int a,b;        /* This rtn places define parms into proper tables */
{               /* for reading during macro expansion */
int c,d;        /* Returns NULL if no errors */

    if ((d=c=getident(line,b))==ERROR)  /* Get identifier */
        return (doerr(1,b));     /* Return if invalid identifier */
/*    fprintf(stderr,"In dodef, defining '%s'\n",&line[b]); */

/* Lets convert a crashed program into an error exit here folks, by
   checking to see if we've run out of deftbl room regardless of the
   status of the string table checked on later. Gene Heskett WDTV5CE. */

    if((defcntr+1)>=MAX_DEFS)
        doerr(21,b); /* doerr() will exit(0) on this one */
    defnam[defcntr]=dptr;   /* Put identifier addr in def name tbl */
    putiddtbl(b,d);           /* Put def name in $trng table */
/*  fprintf(stderr,"DODEFINE: line[c+1]=|%c|\n",line[c+1]);
    fprintf(stderr,"DODEFINE: line=%s\n",line); */
    if (line[c+1]=='(')     /* if def is macro... */
    {
        d=b=skpbl(line,c+2);     /* Align to 1st non-space char in macro */
        if (line[b]!=')')   /* if more than 0 arguments in macro */
        {
            --b;
            defarg[defcntr]=dptr;
            do                  /* Scan argument format */
            {
/*              fprintf(stderr,"DODEF (): b=|%d|\n",b); */
                ++b;
                b=skpbl(line,b);
/*              fprintf(stderr,"DODEF (2): b=|%d|\n",b); */
                if ((c=getident(line,b))==ERROR)  /* Get identifier (args) */
                    return (doerr(2,b));    /* return if any args missing */
                putdtbl(b,c);     /* put args in $trng table */
/*              fprintf(stderr,"DODEF (): dfarg=|%s|\n",defarg[defcntr]);*/
                *(dptr-1)=' ';
                b=skpbl(line,c+1);   /* Should now point at , or ) */
/*              fprintf(stderr,"DODEF (3): b=|%d|\n",b);    */
                if (line[b]!=',' && line[b]!=')')  /* If not ) or , error */
                    return (doerr(3,b));    /* so return */
            } while (line[b]!=')');
            *dptr++=0;
/*          fprintf(stderr,"DODEF: dfarg=|%s|\n",defarg[defcntr]);  */
        }
        else
        {
            if (dptr+5>=&dstrtbl[STRNG_TBL_MAX-1])    /* table overflow */
                doerr(5,b);
            strcpy(--dptr,"( ) ");
            dptr+=5;
            defarg[defcntr]=NULL;   /* 0 args in macro */
        }
    }
    else    /* define is not a macro */
    {
        b=c;
        defarg[defcntr]=NULL;
    }
    if (line[b+1]=='\n')     /* If EOL then define = 1 */
    {
        line[b+1]=' ';
        line[b+2]='1';      /* This does not test LINE to see if it is full */
        line[b+3]='\n';
    }
    b+=2;   /* locate 1st char of token-sequence */
    c=rskpbl(line,a-1);    /* locate last char of token-sequence */
    deftok[defcntr++]=dptr;     /* Put token addr in tok table */
    putdtbl(b,c);               /* Copy token into $trng table */
    return NULL;    /* No errors */
}

prep()     /* Check for preprocessor commands */
{
int b,c;
char *ln;

    ln=line;
    while(*(++ln)=='#' || *ln==' ')  /* locate first directive character */
        ;
    if (!*ln)      /* NULL directive */
        return (killine());
 /* fprintf(stderr,"prep - line=%s\n",ln); */
    if (strcmp2(ln,"if ") || strcmp2(ln,"ifdef ") || strcmp2(ln,"ifndef "))
    {
     /* fprintf(stderr,"prep - calling doif(%s)\n",ln); */
        doif(ln);
        return (killine());
    }
    if (strcmp2(ln,"else"))
    {
        doelse();
        return (killine());
    }
    if (strcmp2(ln,"endif"))
    {
        doendif();
        return (killine());
    }
    if (strcmp2(ln,"elif "))
    {
        doelif(ln);
        return (killine());
    }
    if (procsw)
    {
        if (strcmp2(ln,"define "))
        {
            c=getident(ln,7)+2;  /* get end of identifier */
            splittok(ln,c);    /* tokenize rest of line */
            dodefine(strlen(line),&ln[7]-line);     /* store #define info */
/*          fprintf(stderr,"PREP (after dodef): line=|%s|\n",line); */
            tstdupdef();    /* Check for def duplication and fix */
            return (killine());    /* Discard #define line */
        }
        if (strcmp2(ln,"include "))
        {
            doinclude(&ln[8]);     /* open include file */
            return (killine());    /* Discard #include line */
        }
        if (strcmp2(ln,"undef "))
        {
	 /* fprintf(stderr,"prep - undef found %s\n",ln); */
            doundef(&ln[6]);    /* remove undef identifier from def table */
	 /* fprintf(stderr,"prep - doundef done\n"); */
            return (killine());    /* Discard #undef line */
        }
        if (strcmp2(ln,"error "))
        {
            fprintf(stderr,"User error - %s\n",&ln[6]); /* print error */
            return (killine());    /* Discard #error line */
        }
        if (strcmp2(ln,"asm"))
        {
            for (;;)    /* send all following lines through for assembler */
            {
                getln(0);
                if (eflag)
                    break;
                if (findstr(1,line,"#endasm")!=NULL)
                    break;
                if (cflag)
                    puts("#2");
                else
                    printf("#pragma asm ");
                printf("%s\n",line);
            }
            if (eflag && cflag) /* error only in Microware mode (no #endasm)*/
                doerr(18,1);
            return (killine());
        }
        if (strcmp2(ln,"pragma "))
        {
            dopragma(ln+7);
            return (killine());
        }
        if (strcmp2(ln,"line "))
        {
            doline(ln+5);
            return (killine());
        }
        doerr(17,1);    /* Illegal preprocessor directive */
        return (killine());
    }
}

dopragma(ln)
char *ln;
{
    if (strcmp2(ln,"asm "))
    {
        if (cflag)
            puts("#2");
        else
            printf("#pragma asm ");
        if (*(ln+4)=='*')   /* removes label field null character */
            *(ln+4)=' ';
        printf("%s\n",ln+4);
        return (killine());
    }
    if (!cflag)
        printf("#pragma %s\n",ln);
    return (killine());
}

doline(ln)
char *ln;
{
char *lnptr, *fnptr;

    lnptr=ln-1;
    fnptr=ifnbuf[fptr];
    splittok(lnptr,1);      /* tokenize #line args */
    expand(lnptr,NULL,NULL);          /* expand any macro tokens */
    _line_=atoi(++lnptr);   /* set line number (no error checking) */
    while (*(++lnptr)!=' ')     /* get next argument */
        ;
    if (*(++lnptr)=='"')        /* if " then filename is loaded into ifnbuf */
        while (*(++lnptr)!='"')
            *(fnptr++)=lnptr;
    printf("#line %s\n",ln);    /* #line command is sent to compiler */
}

