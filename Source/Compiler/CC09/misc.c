/*
     Modification history for misc.c:
          25-May-83      Make better use of register variables.
          01-Sep-83      Write errors to stderr.
          17-Apr-84 LAC  Conversion for UNIX.
*/

/* miscellaneous routines for the c compiler */

#include "cj.h"


/* to push down an outer block declaration
 */
pushdown(sptr)
register symnode *sptr;
{
    register symnode *nptr;

    if (nptr = freedown) freedown = nptr->snext;
    else nptr = (symnode *) grab(DOWNSIZE);

    move(sptr,nptr,DOWNSIZE);
    sptr->downptr = nptr;
    sptr->type = sptr->size = sptr->dimptr = sptr->offset = sptr->storage = 
        sptr->x.labflg = sptr->blklev = sptr->snext = 0;
}


/* to recover outer block declaration
 */
pullup(sptr)
register symnode *sptr;
{
    register symnode *nptr;

    nptr = sptr->downptr;
    move(nptr,sptr,DOWNSIZE);

    nptr->snext = freedown;
    freedown = nptr;
}


/* byte move by count
 */
move(p1,p2,count)
register char *p1, *p2;
int count;
{
    while (count--) *p2++ = *p1++;
}


fatal(errstr)
char *errstr;
{
    error(errstr);
    fflush(stderr); /* because 'tidy()' uses '_exit()' i.e. no flush */
    tidy();         /* get rid of temp files and exit */
}


multidef()
{
    error("multiple definition");
}


error(s)
char s[];
{
    doerr(symptr-line,s,symline);
}


warn(s)
char s[];
{
    dowarn(symptr-line,s,symline);
}


comperr(node,errstr)
register expnode *node;
char *errstr;
{
    char newstr[50];

    strcpy(newstr,"compiler error - ");
    strcat(newstr,errstr);
    terror(node,newstr);
}


terror(node,errstr)
register expnode *node;
char *errstr;
{
    doerr(node->pnt - line,errstr,node->lno);
}


dowarn(n,wstr,lno)
register int n;
char wstr[];
{
    eprintf("%s:%d: *** warning: %s ***\n", filename, lno, wstr);
    if (lno == lineno) {
        eputs(line);
        goto dopoint;
    } else if (lno == lineno - 1) {
        eputs(lastline);
dopoint:
        for ( ; n > 0; --n) eputchar(' ');
        eputs("^");
    }
}


doerr(n,errstr,lno)
register int n;
char errstr[];
{
    eprintf("%s:%d: *** %s ***\n", filename, lno, errstr);
    if (lno == lineno) {
        eputs(line);
        goto dopoint;
    } else if (lno == lineno - 1) {
        eputs(lastline);
dopoint:
        for ( ; n > 0; --n) eputchar(' ');
        eputs("^");
    }
    if(++errcount>30) {
        fflush(stderr);
        eputs("too many errors");
        tidy();
    }
}


eprintf(msg,s1,s2,s3,s4)
char *msg;
{
    fprintf(stderr,msg,s1,s2,s3,s4);
}


eputs(s)
char *s;
{
    fputs(s,stderr);
    eputchar('\n');
}


eputchar(c)
char c;
{
    putc(c,stderr);
}


reltree(tree)
register expnode *tree;
{
    if (tree) {
#ifdef DEBUG
        if (dflag) {
            printf("reltree: releasing %x\n",&tree);
            pnode(tree);
            fflush(stdout);
        }
#endif
        reltree(tree->left);
        reltree(tree->right);
        release(tree);
    }
}


release(node)
register expnode *node;
{
    if(node) {
        node->left=freenode;
        freenode=node;
    }
}


nodecopy(n1,n2)
char *n1,*n2;
{
    move(n1,n2,NODESIZE);
}


istype()
{
    if (sym == KEYWORD)
        switch (symval) {
            case INT:
            case CHAR:
            case UNSIGN:
            case SIGN:
            case SHORT:
            case LONG:
            case STRUCT:
            case UNION:
#ifdef  DOFLOATS
            case DOUBLE:
            case FLOAT:
#endif
                return 1;
        }
    else if (sym == NAME) {
        if (((symnode *)symval)->storage == TYPEDEF) return 1;
    }
    return 0;
}


issclass()
{
    if(sym==KEYWORD) switch(symval) {
        case EXTERN:
        case AUTO:
        case TYPEDEF:
        case REG:
        case STATIC:
        case DIRECT:
            return 1;
    }
    return 0;
}


decref(t)
{
    return ((t >> 2) & (~BASICT)) + btype(t) ;
}


incref(t)
{
    return ((t & (~BASICT)) << 2 ) + POINTER + btype(t) ;
}


isbin(op)
register int op;
{
    return (op>=UMOD && op<=UGT);
}


dimnode *
dimwalk(dptr)
register dimnode *dptr;
{
    return dptr ? dptr->dptr : 0;
}


need(key)
{
    static char ptr[] = "x expected";
    register int i;

    if(sym==key) {
        getsym();
        return 0;
    }
    for (i = 0; i < 128; ++i) if (chartab[i] == key) break;
    *ptr=i;
    error(ptr);
/*
    for ( ; ; ) {
        if(sym==key)
            getsym();
        else switch(sym) {
            case SEMICOL: case RBRACE: case EOF:
                break;
            default:
                getsym();
                continue;
        }
        break;
    }
*/
    return 1;
}


junk()
{
    while (sym != SEMICOL && sym != RBRACE && sym != EOF) getsym();
}


#ifdef PTREE
getkeys()
{
    char kname[20];
    register char *p;
    FILE *in1;
    int c;
    int i;

    for (i=0;i<200; kw[i++]="UNKN") ;

    if((in1=fopen(CKEYSFILE,"r")) == NULL) {
        fprintf(stderr,"keys file '%s' not found\n", CKEYSFILE);
        errexit();
    }
    for(i=c=0; ++i<200 && c!=EOF ;){
        for (p=kname; (c=getc(in1))!='\n' && c!=EOF ; *p++=c) ;
        *p='\0';
//      fprintf (stderr, "read string: %d[%s]\n", i, kname);
        if(*kname)
            kw[i] = strcpy(grab(strlen(kname)+1),kname);
    }
    fclose(in1);
}

prtree(node,title)
expnode *node;
char *title;
{
    if (dflag) {
        fflush(stdout);
        printf("%s\naddress  op        value           type     size sux left     right\n",title);
        ptree(node);
    }
}


ptree(node)
register expnode *node;
{
    if (node) {
        pnode(node);
        ptree(node->left);
        ptree(node->right);
    }
}


pnode(node)
register expnode *node;
{
    int op,val,i;

    op=node->op;
    printf("%08x ",node);
    if (op == NAME)
        printf("%-10.8s$%-8x",node->val.sp->sname,node->modifier);
    else printf("%-10s$%-8x",op>0?kw[op]:"EVIL",node->val.num);
    val=node->type;
    for (i=14; i>=4 ; i-=2) {
        switch ((val>>i)  & 3) {
            case 1: putchar('P');break;
            case 2: putchar('A'); break;
            case 3: putchar('F'); break;
            case 0: putchar(' ');
        }
    }
    printf(" %-8s %-4d %2d  %-8x %-8x\n",kw[val & BASICT],node->size,
                                      node->sux, node->left,node->right);
}
#endif
