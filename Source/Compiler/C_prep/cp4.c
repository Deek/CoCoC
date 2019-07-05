#include "cp.h"

expand(ln,loop,lpcntr)   /* Macro expansion rtn */
char *ln;    /* ln[] must start and end with a space character */
int *loop,*lpcntr;
{
int c,i,x,os,m,n,lp[32],lpc,lpflag;
int d,e,f,j,k,l,tcnt;
char tptr[80],*dptr,*lnptr,*lnptr2,*lptr;
char macargs[MAX_ARGS][MAX_LENGTH];
char exmacargs[MAX_ARGS][MAX_LENGTH];
char buf[LINEMAX+3], dtok[LINEMAX+3];
char *idptr[80];
int mname,mname2,idcntr;

/* SETUP LOOP AND LPCNTR */
/* the following test USED to have a * between the ! and lpcntr which
   resulted in one of the most dificult, time consuming bugs I've ever
   traced. It was ok if the caller had a "0" in the third argument
   position, but crashed if the caller used a "NULL". The problem then
   was that the data at address $0000 was not zero at times which resulted
   if the vars loop and lpcntr being left at $0000. The input file pathtable
   lived near $0000, and got trashed because the writes this routine did
   were then at or near $0000. Gene Heskett, WDTV5CE.
*/
    if (!lpcntr)
    {
        lpc=0;
        lpcntr=&lpc;
        loop=lp;
    }

/* PRESCAN LN TO MARK IDENTIFIER FOR MACRO SEARCHES */

    idcntr=0;
    lptr=ln;
    while(lptr=strchr(lptr,' '))
    {
        ++lptr;
        if (IDNT_INIT(*lptr))
        {
            if (*lptr!='L' || (*(lptr+1)!='\"' && *(lptr+1)!='\''))
                idptr[idcntr++]=lptr;
        }
    }

/* SEARCH LN FOR MACRO CALLS */

    for (x=0;x<idcntr;++x)  /* loop for each identifier */
    {
/*      fprintf(stderr,"EXPAND: idptr[%d]=|%s|\n",x,idptr[x]);  */
    for (i=0;i<defcntr;++i) /* loop for each definition name */
    {
/*
        fprintf(stderr,"EXPAND: defnam=|%s|\n",defnam[i]);
        if (defarg[i])
            fprintf(stderr,"EXPAND: defarg=%s\n",defarg[i]);
        fprintf(stderr,"EXP: deftok=|%s|\n",deftok[i]);
        fprintf(stderr,"EXP: line=%s\n",ln);
*/
        if (strcmp2(idptr[x],defnam[i]))  /* returns 1 if match else 0 */
        {
            lpflag=FALSE;
            for (n=0;n<*lpcntr;++n)
            {
/*
                fprintf(stderr,"loop[%d]=%d i=%d\n",n,loop[n],i);
*/
                if (loop[n]==i)
                {
                    lpflag=TRUE;
                    break;
                }
            }
            if (lpflag)
                continue;
            loop[(*lpcntr)++]=i;

            lptr=idptr[x];
            c=lptr-ln;
            *buf=0;
            if (defarg[i])   /* If a macro */
            {
                e=0;
                f=c+strlen(defnam[i]);  /* skip to ( */
                if (ln[f]!='(')
                {
                    doerr(13,f);    /* macro syntax error */
                    return;
                }
                f+=2;   /* point to 1st arg */
                do      /* loop for each arg */
                {
/*
            fprintf(stderr,"EXPAND : b4 toksrch 1\n");
*/
                    l=toksrch(ln,",",0,99,99,f,&tcnt);
                    if (l==ERROR)
                    {   /* either get , separator or ) arg terminator */
                        l=toksrch(ln,")",-1,99,99,f,&tcnt);
                        if (l==ERROR)
                        {
                            doerr(13,f);  /* macro syntax error */
                            return;
                        }
                    }
                    if (l==f)   /* not sure of result when l pts to ) */
                    {
                        doerr(13,f); /* macro syntax error */
                        return;
                    }
                    strncpy(macargs[e],&ln[f],l-f-1);
                    macargs[e][l-f-1]=0;    /* put arg in array */
                    strcpy(exmacargs[e],macargs[e]); /* put in expand array */
                    ++e;
                    f=l+2;
                } while (ln[l]==',');
                for (j=0;j<e;++j)   /* loop to expand each arg */
                {
/*
                    fprintf(stderr,"EXPAND (b4 exp(exmac))\n");
*/
                    expln(exmacargs[j],NULL,NULL);
/*
                    fprintf(stderr,"EXPAND (aft for): mc[%d]=|%s| ex[%d]=|%s|\n",j,macargs[j],j,exmacargs[j]);
*/
                }
/*
        fprintf(stderr,"EXPAND (e tst): e=%d tst=%d\n",e,tstargs(i));
*/
                if (tstargs(i)!=e)
                {
                    doerr(14,d);    /* wrong # of args */
                    return;
                }
                k=1;

        /* Search token sequence for args */

                while (getoknum(deftok[i],tptr,k++))
                {
                    if (toksrch(defarg[i],tptr,99,99,99,0,&tcnt)!=ERROR)
                    {       /* token found is arg */
                        if (!strcmp(getoknum(deftok[i],tptr,k-2),"#"))
                        {
/*
                    fprintf(stderr,"EXP pre#\n");
*/
                            addqmac(macargs[tcnt-1]);
/*
                fprintf(stderr,"EXP addqmac: macarg=|%s|\n",macargs[tcnt-1]);
*/
                            strcat(buf,macargs[tcnt-1]);
                            strcat(buf," ");
                        }
                        else if (!strcmp(getoknum(deftok[i],tptr,k-2),"##") || !strcmp(getoknum(deftok[i],tptr,k),"##"))
                        {
/*
                    fprintf(stderr,"EXP pre/post##\n");
*/
                            strcat(buf,macargs[tcnt-1]);
                            strcat(buf," ");
                        }
                        else
                        {
/*
                    fprintf(stderr,"EXP not#or##\n");
*/
                            strcat(buf,exmacargs[tcnt-1]);
                            strcat(buf," ");    /* use expanded args */
                        }
                    }
                    else    /* token is not argument */
                    {
                        if (strlen(tptr)==1 && *tptr=='#')
                            ;
                        else if (strlen(tptr)==2 && *tptr=='#' && *(tptr+1)=='#')
                            buf[strlen(buf)-1]=0;
                        else
                        {
                            strcat(buf,tptr);   /* substitute in token */
                            strcat(buf," ");
                        }
                    }
/*
            fprintf(stderr,"EXPAND (after if): buf=%s\n",buf);
*/
                }
                /* substitute entire macro into ln[] */
                buf[strlen(buf)-1]=0;   /* drop final space */
                mname=l-c+1;    /* get length of macro */
                strcpy(dtok,&ln[l+1]);  /* save end of line */
                lnptr=&ln[c];

                strcpy(lnptr,buf);       /* put expanded tok-seq in ln[] */

                expln(lnptr,loop,lpcntr);          /* expand tok-seq */
                mname2=strlen(lnptr);

                strcat(lnptr,dtok);   /* replace end of ln */

/*
                fprintf(stderr,"EXPAND (ln): |%s|\n",ln);
                fprintf(stderr,"EXPAND :mn=%d mn2=%d x=%d ic=%d\n",mname,mname2,x,idcntr);
*/
    os=0;
    for (m=x+1;m<idcntr;++m)    /* adjust idptr */
    {
        if (idptr[m]<&ln[l])
            ++os;
        else
            idptr[m-os]=idptr[m]+mname2-mname;
    }
    idcntr-=os;
/*
                fprintf(stderr,"EXPAND (ln): |%s|\n",ln);
                fprintf(stderr,"EXPAND2:mn=%d mn2=%d x=%d ic=%d\n",mname,mname2,x,idcntr);
        fprintf(stderr,"EXPAND (b4 exp2): ln=%s\n",ln);
*/
            }
            else    /* Not a macro */
            {
                mname=strlen(defnam[i])-1;  /* get length of identifier */
                strcpy(buf,lptr+mname);     /* save end of line */
                switch(i)
                {
                    case 0: /* __LINE__ */
                        itoa(_line_-1,lptr);
                    break;
                    case 1: /* __FILE__ */ /*should be quoted 5/22/2011*/
                        strcpy(lptr,"\"");
                        strcat(lptr,ifnbuf[fptr]);
                        strcat(lptr,"\"");
                    break;
                    default:
                        strcpy(lptr,deftok[i]);   /* copy tok-seq into line */
                        expln(lptr,loop,lpcntr);  /* expand token sequence */
                    break;
                }
                mname2=strlen(lptr);
                strcat(ln,buf);   /* replace end of line */
                for (l=x+1;l<idcntr;++l)   /* adjust idptr */
                    idptr[l]+=mname2-mname;
            }
/*
            fprintf(stderr,"lpcntr=%d\n",*lpcntr);
*/
            if (*lpcntr)
                --*lpcntr;
            break;
        }
    }
    }
}

expln(ln,l,lc)   /* Inserts beginning and ending space then calls expand */
char *ln;
int *l,*lc;
{
char buf[LINEMAX+3];

    *buf=' ';
    strcpy(&buf[1],ln);
    strcat(buf," ");
/*  fprintf(stderr,"EXPLN (buf): |%s|\n",buf);  */
    expand(buf,l,lc);
/*  fprintf(stderr,"EXPLN (buf)2: |%s|\n",buf); */
    buf[strlen(buf)-1]=0;   /* Kill final space in line */
    strcpy(ln,&buf[1]);
}

#asm
strcmp2:
 pshs u
 ldu 4,s   (s1)
 ldx 6,s   (s2)
STRLP0
 ldb ,x+
 beq STRMTCH
 cmpb ,u+
 beq STRLP0
 clra
 clrb
 puls u,pc
STRMTCH
 ldd #1
 puls u,pc
#endasm

