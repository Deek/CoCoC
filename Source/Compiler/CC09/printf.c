/*
     Modification History for printf.c:
      24-May-83      Use some static directs.
*/

/* printf cut down for compiler use */

/* DO NOT USE FOR ANYTHING ELSE */

#include <stdio.h>
#ifdef __unix__
#define direct
#endif

static _dofp(fmt,args);
static itoa(n);
static xtoa(n);
static utoa(s,n);
static putstr(s,len);
static putstr1(s,len,len1);
static putch(c);

#define DIGIT  107

#ifdef  PASS2
# include <ctype.h>
#else
extern char chartab[];
# define isdigit(c)  (chartab[c]==DIGIT)
#endif

static direct char *pointer;
static char buf0[10],buf[10];
static direct int leftflag,pad,flag;

#define file    1
#define string  2

#ifndef __unix__
printf(fmt,args)
char *fmt;
int args;
{
    pointer=stdout;
    flag = file;
    _dofp(fmt,&args);
}


fprintf(f,fmt,args)
char *fmt;
FILE *f;
int args;
{
    pointer=f;
    flag = file;
    _dofp(fmt,&args);
}


sprintf(s,fmt,args)
char *s,*fmt;
{
    pointer = s;
    flag = string;
    _dofp(fmt,&args);
    *pointer = '\0';
}


static 
_dofp(fmt,args)
register char *fmt;
register int *args;
{
    char *ptr,c;
    int digits1,point,digits2;
    int x;

    for(;;){
        while((c= *fmt++)!='%') {
            if (c=='\0') return;
            putch(c);
        }
        c= *fmt++;

        digits1=digits2=0;

        if (c=='-') {
            leftflag=1;
            c = *fmt++;
        } else leftflag=0;

        pad = (c=='0') ? '0' : ' ';

        while(isdigit(c)) {
            digits1=digits1*10 +(c-'0');
            c = *fmt++;
        }

        if(c=='.') {
            point=1;
            c= *fmt++;
            while(isdigit(c)) {
                digits2=digits2*10 + (c-'0');
                c = *fmt++;
            }
        }
        else point=0;

        switch(c){
        case 'd':
            putstr(itoa(*args++),digits1);
            break;
        case 'x':
            putstr(xtoa(*args++),digits1);
            break;
        case 'c': putch(*args++);
            break;
        case 's': ptr= *args++;
            if(point) {
                for(point = ptr;digits2--;ptr++)
                    if(*ptr == '\0')
                        break;
                putstr1(point,(int)ptr - point,digits1);
            }
            else
                putstr(ptr,digits1);
            break;
        default:
            putch(c);
        }
    }
}


static
itoa(n)
{
    char *p;

    p=buf0;
    if (n<0) {
        if((n= -n)<0) return strcpy(buf0,"-32768");
        *p++='-';
    }
    utoa(p,n);
    return buf0;
}

static int tens[] = {
    10000,
    1000,
    100,
    10
};

static direct int *tensend = &tens[4];

static
utoa(s,n)
char *s;
{
    register char *p;
    register int *ip;
    int flag,count;

    p=s;
    flag=0;

    count=0;
    while(n<0) {
        count++;
        n -= tens[0];
    }
    for(ip=tens;ip!=tensend;++ip) {
        while((n -= *ip) >= 0) ++count;
        n += *ip;

        if(count) flag=1;
        if(flag) *p++ = count + '0';

        count=0;
    }

    *p++ = n + '0';
    *p='\0';

    return s;
}


static
xtoa(n)
unsigned n;
{
    register char *p,*s1;
    int n1;

    s1 = buf0;
    p = buf;

    do {
        *p++ = (n1 = (n & 0xf)) + (n1 > 9 ? ('a'-10) : '0');
    } while (n = (n >> 4) & 0xfff);

    while (--p >= buf) *s1++ = *p;
    *s1 = '\0';

    return buf0;
}


static
putstr(s,len)
register int len;
register char *s;
{
    int c;

    len -= strlen(s);

    if (!leftflag) while(len-- > 0) putch(pad);

    while (c = *s++) putch(c);

    if (leftflag) while(len-- >0 ) putch(pad);
}


static
putstr1(s,len,len1)
register char *s;
register int len;
{
    int padding = len1 - len;

    if(!leftflag)
        while(padding-- > 0)
            putch(pad);
    while(len--)
        putch(*s++);

    if(leftflag)
        while(padding-- > 0)
            putch(pad);
}


static
putch(c)
{
    if(flag == string)
        *pointer++ = c;
    else
        putc(c,pointer);
}
#endif
