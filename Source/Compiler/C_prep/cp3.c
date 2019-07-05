#include "cp.h"

doif(ln)
char *ln;
{
    if (iflvl==MAX_NEST_IF)
    {
        doerr(15,ln-line);    /* Too many nested #ifs */
        return;
    }
    if (procsw)
        ifstat[++iflvl]=procsw=gettorf(ln);  /* evaluate IF & return T or F */
    else
        ifstat[++iflvl]=DEAD;   /* unused #if */
    ifnest[iflvl]=2;
}

doelse()
{
    if (iflvl<0 || ifnest[iflvl]==1)
    {
        doerr(16,3);    /* improper nesting */
        return;
    }
    --ifnest[iflvl];
    if (ifstat[iflvl]!=DEAD)
        procsw=ifstat[iflvl]=1-ifstat[iflvl];   /* flip T or F status */
}

doendif()
{
    if (iflvl<0)
    {
        doerr(16,3);    /* nesting error */
        return;
    }
    if (ifstat[--iflvl]!=DEAD)
        procsw=ifstat[iflvl];
}

doelif(ln)
char *ln;
{
    if (iflvl<0 || ifnest[iflvl]==1)
    {
        doerr(16,ln-line);    /* improper nesting */
        return;
    }
    if (ifstat[iflvl]==TRUE)
    {
        ifstat[iflvl]=DEAD;
        procsw=FALSE;
    }
    if (ifstat[iflvl]==FALSE)
        procsw=ifstat[iflvl]=gettorf(ln);  /* evaluate IF & return T or F */
    ifnest[iflvl]=2;
}

gettorf(ln)
register char *ln;
{
    switch(ln[2])
    {
        case 'd':   /* ifdef */
            splittok(ln,6);
            return dodef(ln,6);
        break;
        case 'n':   /* ifndef */
            splittok(ln,7);
            return (1-dodef(ln,7));
        break;
        case 'i':   /* elif */
            return ifcalc(&ln[5]);  /* process elif expression */
        break;
        case ' ':   /* if */
            return ifcalc(&ln[3]);  /* process if expression */
        break;
    }
}

dodef(ln,b)
char *ln;
int b;
{
int i,tcnt;

    for (i=0;i<defcntr;++i)
    {
        if ((toksrch(ln,defnam[i],99,99,99,b,&tcnt))==b)
            return TRUE;
    }
    return FALSE;
}

ifcalc(ln)      /* processes #if/#elif expression and returns TRUE or FALSE */
char *ln;
{
unsigned x;

/*  fprintf(stderr,"IFCALC: ln=|%s|\n",ln); */
    splittok(ln,0);     /* tokenize if expression */
/*  fprintf(stderr,"IFCALC: (aft split) ln=|%s|\n",ln); */
    dodfined(ln);       /* replace defined commands with 0 or 1 */
/*  fprintf(stderr,"IFCALC: (aft dfned) ln=|%s|\n",ln); */
    expand(ln,NULL,NULL);         /* macro expand line */
/*  fprintf(stderr,"IFCALC: (aft exp) ln=|%s|\n",ln);   */
    zeroident(ln);      /* replace leftover identifiers with 0 */
/*  fprintf(stderr,"IFCALC: (aft zero) ln=|%s|\n",ln);  */
    x=solve(ln);
/*  fprintf(stderr,"IFCALC: x=%u\n",x); */
    if (x)
        return TRUE;
    else
        return FALSE;
}

dodfined(ln)    /* replaces defined commands with 0 or 1 */
char *ln;
{
int b,c,d,pflag;

    pflag=FALSE;
    while(b=findstr(1,ln," defined "))
    {
        c=b+8;  /* point to next token */
        if (ln[c]=='(')
        {
            c+=2;   /* point to identifier token */
            pflag=TRUE;
        }
        d=dodef(ln,c-1);  /* returns TRUE if defined, FALSE if not defined */
        while (ln[++c]!=' ')    /* get space at end of identifier */
            ;
        if (pflag)
        {
            if (ln[++c]==')' && ln[++c]==' ')
                ;
            else
                doerr(3,c);     /* No end ) */
        }
        ln[b]=d+'0';  /* replace defined with 0 or 1 */
        strcpy(&ln[++b],&ln[c]);
    }
}

zeroident(ln)
char *ln;
{
int b,c,d;

    b=skpbl(ln,0);
/*  fprintf(stderr,"ZERO: ln[b]=%c\n",ln[b]);   */
    while (ln[b])
    {
        c=b;
        while (ln[++c]!=' ')
            ;
        if (IDNT_INIT(ln[b]))
        {
            ln[b++]='0';
            strcpy(&ln[b],&ln[c]);
        }
        else
            b=c+1;
        b=skpbl(ln,b);
    }
}

lnprint(x,y)
int x,y;
{
char buf[35],*ptr,*ed;
int *flag;

    flag=y;
    if (*flag==-1)
        return;
    switch(x)
    {
        case 0: /* #line ## */
            if (*flag==-1)
                break;
            if (cflag)
                printf("#5\n%d\n",*flag);
            else
                printf("#line %d\n",*flag);
            *flag=-1;
        break;
        case 1: /* #line ## "filename" (new file) */
            if (cflag)
            {
                if ((ptr=strrchr(ifnbuf[fptr],'/')))
                    strcpy(buf,ptr+1);
                else
                    strcpy(buf,ifnbuf[fptr]);
                *(strrchr(buf,'.'))='_';
                printf("#7\n%s\n%s\n#5\n0\n",ifnbuf[fptr],buf);
            }
            else
                printf("#line 0 \"%s\"\n",ifnbuf[fptr]);
            _line_=0;
        break;
        case 2: /* #line ## "filename" (old file) */
            if (cflag)
            {
                if ((ptr=strrchr(ifnbuf[fptr],'/')))
                    strcpy(buf,ptr+1);
                else
                    strcpy(buf,ifnbuf[fptr]);
                *(strrchr(buf,'.'))='_';
                printf("#7\n%s\n%s\n#5\n%d\n",ifnbuf[fptr],buf,incln[fptr]);
            }
            else
                printf("#line %d \"%s\"\n",incln[fptr],ifnbuf[fptr]);
            _line_=incln[fptr];
        break;
        case 3: /* #line ## "filename" (root startup) */
            ed=y;
            if (cflag)
            {
                if ((ptr=strrchr(ifnbuf[fptr],'/')))
                    strcpy(buf,ptr+1);
                else
                    strcpy(buf,ifnbuf[fptr]);
                *(strrchr(buf,'.'))='_';
                printf("#7\n%s\n%s\n#P\n%s\n%s\n",ifnbuf[fptr],buf,buf,ed);
            }
            else
            {
                printf("#line %d \"%s\"\n",incln[fptr],ifnbuf[fptr]);
                printf("#pragma edn %s\n",ed);  /* edition pragma */
            }
            _line_=0;
        break;
    }
}

doerr(code,errptr)
int code,errptr;
{
int i,xflag;

        xflag=FALSE;
        fprintf(stderr,"\x7\n*** \"%s\" <Line #%d>   ",ifnbuf[fptr],_line_-1);
        switch(code)
        {
            case 1:
                fprintf(stderr,"#define error - missing identifier");
            break;
            case 2:
                fprintf(stderr,"#define error - missing arguments");
            break;
            case 3:
                fprintf(stderr,"#define error - missing , or )");
            break;
            case 4:
                fprintf(stderr,"#define error - missing space after args");
            break;
            case 5:
                fprintf(stderr,"Compiler error - define $tring table full");
                xflag=TRUE;
            break;
            case 6:
                fprintf(stderr,"#include error - bad file name");
            break;
            case 7:
                fprintf(stderr,"Compiler error - include too deeply nested");
            break;
/* UNUSED ERRORS
            case 8:
                fprintf(stderr,"Compiler error - line too long");
                xflag=TRUE;
            break;
 */
            case 9:
                fprintf(stderr,"Syntax error - Missing >");
            break;
            case 10:
                fprintf(stderr,"Warning - No end */");
            break;
            case 11:
                fprintf(stderr,"Warning - Improper redefinition");
            break;
 /*     UNUSED
            case 12:
                exit(fprintf(stderr,"Syntax error - unmatched () or []"));
            break;
*/
            case 13:
                fprintf(stderr,"Syntax error - macro syntax error");
            break;
            case 14:
                fprintf(stderr,"Syntax error - wrong # of args");
            break;
            case 15:
                fprintf(stderr,"Compiler error - too many nested #ifs");
            break;
            case 16:
                fprintf(stderr,"Syntax error - #if nesting error");
            break;
            case 17:
                fprintf(stderr,"Syntax error - Illegal # directive");
            break;
            case 18:
                fprintf(stderr,"Syntax error - No #endasm");
                xflag=TRUE;
            break;
            case 19:
                fprintf(stderr,"Syntax error - No end \" or \'");
            break;
            case 20:
                fprintf(stderr,"Syntax error - Illegal escape sequence");
            break;
            case 21:
                fprintf(stderr,"Define arrays full - more than %d\n",MAX_DEFS);
                xflag=TRUE;
            break;
        }
        fprintf(stderr," ***\n%s\n",line);

        for (i=0;i<errptr;++i)
            fprintf(stderr," ");
        fprintf(stderr,"^\n");

        if (xflag)
            exit(0);
        return ERROR;
}

/*
******************************************************
*
* lnread(path,s,n);
* int path,n;
* char *s;
*
* Reads up to 'n' characters.  Replaces the CR with a NULL.
*/
#asm
lnread:
 pshs y
 lda 5,s    (path)
 ldy 8,s    (n)
 ldx 6,s    (s)
 os9 $8b    (I_READLN)
 lbcs os9err (readln error)
 leay -1,y   (decrement # of bytes read value)
 ldx 6,s     (s)
 tfr y,d
 leax d,x    (add # of bytes read to &s and get end of string)
 lda ,x
 cmpa #13    (check for CR)
 lbeq clrbyt (good to go)
 leay 1,y    (restore original length of read bytes)
 cmpy 8,s    (is # of bytes read value less then n)
 lbeq os9er1 (no terminator and no room to add null)
 leax 1,x    (point x passed end of read string)
 leay -1,y   (decrement # of bytes read value)
clrbyt clr ,x (change CR or trash to NULL)
 tfr y,d     (return # of bytes read)
 puls y,pc

os9er1 ldb #55 (line read is too large)
os9err
 puls y
 clra
 std errno,y
 ldd #-1
 rts
#endasm

/*
******************************************************
*
* lnwrite(path,s,n);
* int path,n;
* char *s;
*
* Writes up to 'n' characters, stopping at the NULL character.
* Replaces the NULL with a CR.
*/
#asm
lnwrite:
 pshs u,y
 ldu 8,s    (s)
 pshs x
 lbsr strlen
 leas 2,s  (pop x)
 leau d,u  (get address of end of s)
 ldb  #13
 stb ,u    (replace NULL with CR)
 ldy 10,s  (n)
 ldx 8,s   (s)
 lda 7,s   (path)
 os9 $8c    (I_WRITLN)
 lbcs os9err (writeln error)
 clrb
 stb ,u    (replace CR with NULL)
 tfr y,d
 puls u,y,pc
os9er2
 puls u,y
 clra
 std errno,y
 ldd #-1
 rts
#endasm

