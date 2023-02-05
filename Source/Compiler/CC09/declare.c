/*
    Modification history for declare.c:
        23-May-83       Handle direct initializers.
        25-May-83       Make initstruct pointers be DIRECT.
                        Better use of register variables.
        14-Apr-84 LAC   Conversion for UNIX.
        11-Feb-86 LAC   fixed array subscript ordering bug.
*/

#include "cj.h"
#define  MPOINT 1

#ifdef USE_YREG
# define MAXREG	2
#else
# define MAXREG	1
#endif

/**********************************
 *                                *
 *  declarations and definitions  *
 *                                *
 **********************************/

static direct int reguse;
static direct struct initstruct *initlist,*initlast,*initfree;

#ifdef FUNCNAME
direct int fnline;
#endif


extdef()
{
    register symnode *ptr;
    symnode *ptemp;
    dimnode *dimptr, *tdp;
    int size,ssize,tsc,sclass,type,temp;
    elem *eptr;

    while (sym == RBRACE) {
        error("too many brackets");
        getsym();
    }

    if (sym == LBRACE) {
        error("function header missing");
        block(0);
        clear(&labelist);
        getsym();
        return;
    }

    switch (sclass = setclass()) {
        case REG:
        case AUTO:
            error("storage error");
        case 0:
            sclass = EXTDEF;
    }

    if ((type = settype(&size,&dimptr,&eptr)) == UNDECL) type = INT;

#ifdef FUNCNAME
     fnline = symline;
#endif
    for ( ; ; ) {
        tdp = dimptr;
        temp = declarator(&ptemp,&tdp,type);
        ptr = ptemp;     /* decl ptr to register */

        if (ptr == NULL) {
            if (temp != STRUCT && temp != UNION) identerr();
            goto next;
        }
/*
        if(isftn(temp))
            tsc = (sym==COMMA || sym==SEMICOL) ? EXTERN : EXTDEF;
        else tsc=sclass;
*/
        tsc = isftn(temp) ? ((sym == COMMA || sym == SEMICOL) ? EXTERN
                                                        : EXTDEF) : sclass;

        if (ptr->type != UNDECL) {
            if (check_same(ptr,temp,eptr)) ;
            else if (tsc != EXTERN) {
                if (ptr->storage != EXTERN && ptr->storage != EXTERND)
                    multidef();
                else {
                    ptr->storage = tsc;
                    ptr->dimptr = tdp;
                    ptr->x.elems = eptr;
                    goto onlist;
                }
            }
        } else {
            ptr->type = temp;
            ptr->blklev = 0;
            ptr->storage = tsc;
            ptr->x.elems = eptr;
onlist:
            ssize = sizeup(ptr,tdp,size);
            if (!isftn(temp)) {
                if (sym == ASSIGN) initialise(ptr,tsc,temp);
                else if (ssize == 0 && tsc!=EXTERN) sizerr();
                else switch(tsc) {
                        case STATICD:
                        case STATIC:
                            extstat(ptr,ssize,(tsc == STATICD));
                            break;
                        case DIRECT:
                        case EXTDEF:
                            defglob(ptr,ssize,(tsc == DIRECT));
                            break;
                    }
                if (tsc == STATIC) ptr->storage = EXTDEF;
                else if (tsc == STATICD) ptr->storage = DIRECT;
            }
        }
        if (isftn(temp)) {
            temp = decref(temp);
            if (isary(temp) || isftn(temp)
                            || temp == STRUCT || temp == UNION) {
                error("function type error");
                ptr->type = INT | FUNCTION;
                temp = INT;
            }
            if (tsc == EXTERN) clear(&arglist);
            else {
                ftype = temp;
                newfunc(ptr,sclass);
                return;
            }
        }
next:
        if (sym != COMMA) break;
        getsym();
    }
    if (need(SEMICOL)) junk();
}


argdef()
{
    register symnode *ptr;
    symnode *ptemp;
    dimnode *dimptr, *tdp;
    int sclass,type,size,stemp,temp;
    elem *eptr;

    switch (sclass = setclass()) {
        default:    error("argument storage");
        case 0:     sclass = AUTO;
        case REG:   break;
    }
    if ((type = settype(&size,&dimptr,&eptr)) == UNDECL) type = INT;

    for ( ; ; ) {
        tdp = dimptr;
        temp = declarator(&ptemp,&tdp,type);
        ptr = ptemp;        /* decl ptr to register */

        if (isftn(temp) || temp == STRUCT || temp == USTRUCT) {
            error("argument error");
            goto next;
        } else if (isary(temp)) {
            temp = incref(decref(temp));
            tdp = tdp ? tdp->dptr : 0;
        }
#ifdef DOFLOATS
        else if (temp == FLOAT) temp = DOUBLE;
#endif
        if (ptr == NULL) {
            identerr();
            goto next;
        }
        stemp = chkreg(sclass,temp);

        switch (ptr->storage) {
            case ARG:
                ptr->type = temp;
                ptr->storage = stemp;
                ptr->x.elems = eptr;
                if (!sizeup(ptr,tdp,size)) sizerr();
                break;
            case AUTO:
            case REG:
                multidef();
                break;
            default:
                error("not an argument");
        }
next:
        if (sym == ASSIGN) initerr();
        if (sym != COMMA) break;
        getsym();
    }
    if (need(SEMICOL)) junk();
}


blkdef()
{
    register symnode *ptr;
    symnode *ptemp;
    dimnode *dimptr, *tdp;
    expnode *ep;
    int sclass,type,temp,size,stemp,ssize;
    elem *eptr;
    initnode *i;

    switch (sclass = setclass()) {
        case DIRECT:
            error("storage error");
            /* fall through to.. */
        case 0:
            sclass = AUTO;
    }
    if ((type = settype(&size,&dimptr,&eptr)) == UNDECL) type = INT;

    for ( ; ; ) {
        tdp = dimptr;
        temp = declarator(&ptemp,&tdp,type);
        ptr = ptemp;
        if (ptr == NULL) {
            if (temp != STRUCT && temp != UNION) identerr();
            goto next;
        }
        if (isftn(temp) || sclass == EXTERN) {
            if (ptr->type == UNDECL){
                ptr->type = temp;
                ptr->storage = EXTERN;
                ptr->blklev = 0;
                ptr->x.elems = eptr;
                sizeup(ptr,tdp,size);
            } else check_same(ptr,temp,eptr);
            goto next;
        }
        stemp = chkreg(sclass,temp);

        if (ptr->type != UNDECL) {
            if (ptr->blklev == blklev) {
                multidef();
                goto next;
            }
            pushdown(ptr);
        }
        ptr->type = temp;
        ptr->storage = stemp;
        ptr->x.elems = eptr;
        ptr->blklev = blklev;
        ptr->snext = vlist;
        vlist = ptr;

        if (!(ssize = sizeup(ptr,tdp,size)) && sym != ASSIGN) sizerr();
        switch (stemp) {
            case AUTO:
                stlev -= ssize;
                ptr->offset = stlev;
                break;
            case STATICD:
            case STATIC:
                ptr->offset = getlabel();
        }

        if (sym == ASSIGN) {
            if (stemp == STATIC || stemp == STATICD)
                initialise(ptr,stemp,temp);
            else {
                getsym();
                if (isary(temp) || temp == STRUCT) initerr();
                else {
                    if (ep = parsexp(LEV_2)) {
                        if (initfree) {
                            initfree = (i = initfree)->initnext;
                            i->initnext = NULL;
                        } else i = (initnode *) grab(sizeof(initnode));
                        i->initp = ep;
                        i->initname = ptr;
                        if (initlist) initlast->initnext = i;
                        else initlist = i;
                        initlast = i;
                    } else initerr();
                }
            }
        } else switch(stemp) {
                case STATIC:
                case STATICD:
                    locstat(ptr->offset,ssize,(stemp == STATICD));
            }
next:
        if (sym != COMMA) break;
        getsym();
    }
    need(SEMICOL);
}


check_same(ptr,type,eptr)
register symnode *ptr;
register int type;
register elem *eptr;
{
     if (ptr->type != type
                || (type == STRUCT && ptr->x.elems != eptr)) {
          error("declaration mismatch");
          return TRUE;
     }
     return FALSE;
}


chkreg(sclass,type)
{
    /* if the user wants a register variable
     * and it is allowed allocate one.
     */
    if (sclass == REG) {
        /* user wants a register variable */
        if (reguse < MAXREG)          /* still registers free */
            switch (type) {
                /* types allowed: int, unsigned and pointer */
                default:
                    if (!isptr(type)) break;
                    /* else fall through to... */
                case INT:
                case UNSIGN:
                    /* he can have one! */
#ifdef USE_YREG
                    return ++reguse == 1 ? UREG : YREG;
#else
                    ++reguse; return UREG;
#endif
            }
        /* otherwise make it auto - this is only called
         * inside a function or its argument declaration
         * area.
         */
        return AUTO;
    }
    /* he did not want one */
    return sclass;
}


newfunc(fptr,sclass)
symnode *fptr;
{
    register symnode *p1;
    symnode *p2;
    int offset,stemp,paramlev;
    char *cp;
#ifdef PROF
    int prlab;
#endif

    blklev = 1;
    callflag = reguse = stlev = maxpush = sp = 0;

    while (sym != LBRACE) argdef();

    cp = fptr->sname;

#ifdef PROF
    /* make the profiler function name string */
    if (pflag) profname(cp,prlab=getlabel());
#endif

    paramlev = 0;

    p1 = arglist;
#ifdef REGPARMS /* FIXME: see if we can do this for static funcs */
    if (p1) {
# ifdef DEBUG
        if (dflag) {printf("newfunc node:\n"); pnode(p1);}
# endif
        switch (p1->type) {
            case LONG:
            case ULONG:
# ifdef DOFLOATS
            case FLOAT:
            case DOUBLE:
# endif
                /* generate register save & stack checking */
# ifdef PROF
                startfunc(cp,(sclass != STATIC),0,prlab);
# else
                startfunc(cp,(sclass != STATIC),0);
# endif
                break;
            default:
                switch (p1->storage) {
                    case UREG:
#ifdef USE_YREG
                    case YREG:
#endif
# ifdef PROF
                        startfunc(cp,(sclass != STATIC),p1->storage,prlab);
# else
                        startfunc(cp,(sclass != STATIC),p1->storage);
# endif
                        break;
                    case ARG:
                        p1->storage = AUTO;
                    case AUTO:
# ifdef PROF
                        startfunc(cp,(sclass != STATIC),DREG,prlab);
# else
                        startfunc(cp,(sclass != STATIC),DREG);
# endif
                        p1->offset = (paramlev -= 2);
                        if (p1->type == CHAR || p1->type == UCHAR) p1->offset += 1;
                        break;
                    default:
                        error("argument type in \"newfunc\"");
                }
                p1 = p1->snext;
        }
    } else {
# ifdef PROF
        startfunc(cp,(sclass != STATIC),prlab);
# else
        startfunc(cp,(sclass != STATIC));
# endif
    }
#else   /* !REGPARMS */
# ifdef PROF
    startfunc(cp,(sclass != STATIC),prlab);
# else
    startfunc(cp,(sclass != STATIC));
# endif
#endif

    stlev = sp = paramlev;
    offset = 2 + (MAXREG * 2);
    while (p1) {
        int size;

        p1->offset = offset;
        switch (p1->type) {
            case LONG:
            case ULONG:
                size = LONGSIZE;
                break;
#ifdef  DOFLOATS
            case FLOAT:
                size = FLOATSIZE;
                break;
            case DOUBLE:
                size = DOUBLESIZE;
                break;
#endif
            case CHAR:
            case UCHAR:
                ++p1->offset;
            default:
                size = INTSIZE;
        }
        switch (stemp = p1->storage) {
            case UREG:
#ifdef USE_YREG
            case YREG:
#endif
                gen(stemp,offset-sp);
                break;
            case ARG:
                p1->storage = AUTO;
        }
        offset += size;
        p1 = p1->snext;
    }
    p2 = arglist;
    arglist = NULL;

#ifdef REGCONTS
    clrconts();         /* clear register contents */
#endif
    block(-paramlev);

    clear(&p2);
    clear(&labelist);
    if (lastst != RETURN) gen(RETURN,0,0);
    lastst = 0;

    /* generate stack reservation */
    endfunc();

    blklev = 0;
    if (sym == EOF) error("function unfinished");
    getsym();
}


block(stkadj)
int stkadj;
{
    register expnode *p;
    struct initstruct *i;
    register symnode *varlist;
    int savlev;

    varlist = vlist;
    vlist = NULL;
    getsym();
    ++blklev;
    savlev = stlev + stkadj;

    while (issclass() || istype()) blkdef();
    if (maxpush > stlev) maxpush = stlev;
    sp = modstk(stlev);

    while (initlist) {
        i = initlist;
        p = i->initp;
        p = newnode(NAME,0,0,i->initname,p->lno,p->pnt);
        p = newnode(ASSIGN,p,i->initp,0,p->lno,p->pnt);
        reltree(tranexp(optim(p)));
        i = i->initnext;
        initlist->initnext = initfree;
        initfree = initlist;
        initlist = i;
    }

    while (sym != RBRACE && sym != EOF) statement();

#ifdef REGCONTS
    if (vlist) clrconts();
#endif
     clear(&vlist);
     vlist = varlist;
     --blklev;
     stlev = savlev;
/*
     if(lastst!=RETURN)
          sp=modstk(savlev);
     else sp=savlev;
*/
     sp = (lastst != RETURN) ? modstk(savlev) : savlev;
}


declist(list)
register symnode **list;
{
    register symnode *ptr,*last;

    *list = NULL;
    getsym();

    for (;;) {
        if (sym == RPAREN) break;
        if (sym == NAME) {
            ptr = (symnode *) symval;
            if (ptr->storage == ARG) error("named twice");
            else if (ptr->type != UNDECL) pushdown(ptr);
            ptr->type = INT;
            ptr->storage = ARG;
            ptr->blklev = 1;
            ptr->size = 2;
            if (*list) last->snext = ptr;
            else *list = ptr;
            ptr->snext = NULL;
            last = ptr;
            getsym();
        } else identerr();

        if (sym != COMMA) break;
        getsym();
    }
    need(RPAREN);
}


setclass()
{
     int class;
     if(issclass()) {
          class=symval;
          getsym();
          if(sym==KEYWORD && symval==DIRECT) {
               switch(class) {
                    case STATIC: class=STATICD; break;
                    case EXTERN: class=EXTERND; break;
               }
               getsym();
          }
          return class;
     }
     return 0;
}

int
modifier (size)
int *size;
{
    int is_long=0, isshort=0, issign=0, isunsign=0;
    int type = UNDECL, tsize = 0;

    while (sym == KEYWORD) {
        switch (symval) {
        default:    /* some keyword other than what we're interested in... */
            goto done;
        /* type modifiers (nonterminal) */
        case SIGN:
            if (issign || isunsign) goto err;
            issign=1;
            break;
        case UNSIGN:
            if (issign || isunsign) goto err;
            isunsign=1;
            break;
        case LONG:
            if (is_long || isshort) goto err;
            is_long=1;
            break;
        case SHORT:
            if (is_long || isshort) goto err;
            isshort=1;
            break;
        /* final types (terminal) */
        case CHAR:
            if (is_long || isshort) goto err;
            tsize = 1;
            type = isunsign ? UCHAR : CHAR;
            getsym();
            goto done;
        case INT:
            if (is_long) {
                tsize = LONGSIZE;
                type = isunsign ? ULONG : LONG;
            } else {
                tsize = INTSIZE;
                type = isunsign ? UNSIGN : INT;
            }
            getsym();
            goto done;
#ifdef DOFLOATS
        case DOUBLE:
            if (isshort || issign || isunsign) goto err;
            tsize = DOUBLESIZE;
            type = DOUBLE;
            getsym();
            goto done;
        case FLOAT:
            if (isshort || issign || isunsign) goto err;
            if (is_long) {
                tsize = DOUBLESIZE;
                type = DOUBLE;
            } else {
                tsize = FLOATSIZE;
                type = FLOAT;
            }
            getsym();
            goto done;
#endif
        }
        getsym();
    }
done:
    if (!type) {    /* allow only modifiers, default "int" size */
        if (is_long) {
            tsize = LONGSIZE;
            type = isunsign ? ULONG : LONG;
        } else {
            tsize = INTSIZE;
            type = isunsign ? UNSIGN : INT;
        }
    }
    *size = tsize;
    return type;
err:
    error ("bad type");
    return 0;
}

settype(size,dimptr,ellist)
int *size;
dimnode **dimptr;
elem **ellist;
{
    register symnode *ptr, *tagptr;
    dimnode *dptr;
    int offset,msize,mtype,savflg,dtype,s;
    elem *eptr,*elast,*elocal;
    int type = UNDECL, tsize = INTSIZE;

    *ellist = 0;
    if (sym==KEYWORD) {
        switch (type=symval) {
            case LONG:
            case SHORT:
            case SIGN:
            case UNSIGN:
                type = modifier(&tsize);
                break;
            case CHAR:
                tsize=1;
            case INT:
                getsym();
                break;
#ifdef DOFLOATS
            case DOUBLE:
                tsize=DOUBLESIZE;
                getsym();
                break;
            case FLOAT:
                tsize=FLOATSIZE;
                getsym();
#endif
                break;
default:
                type = UNDECL;
                break;
            case UNION:
            case STRUCT:
                tsize=offset=0;
                ++mosflg;	/* go into special "struct" lexer mode */
                tagptr=NULL;
                getsym();
                --mosflg;	/* and get back out */
                if (sym==NAME) {
                    tagptr= (symnode *) symval;
                    if (tagptr->type==UNDECL) {
                        tagptr->type=USTRUCT;
                        tagptr->storage=STRTAG;
                    } else if (tagptr->storage!=STRTAG)
                        error("name clash");
                    getsym();
                    if (sym != LBRACE) {
                        if(tagptr->type==STRUCT) {
                            *size=tagptr->size;
                            *ellist=tagptr->x.elems;
                            return STRUCT;
                        }
                        *size = (int) tagptr;
                        return USTRUCT;
                    } else if(tagptr->type==STRUCT) multidef();
                }
                if (sym!=LBRACE) error("struct syntax");
                else {
                    ++mosflg;	/* go into special "struct" lexer mode */
                    do {
                        savflg=mosflg;
                        mosflg=0;
                        getsym();
                        mosflg=savflg;
                        if (sym==RBRACE) break;
                        mtype=settype(&msize,&dptr,&elocal);
                        while (1) {
                            symnode *ptemp;
                            dimnode *tdptr;

                            tdptr = dptr;
                            if (sym == SEMICOL) break;
                            ++blklev;
                            dtype = declarator(&ptemp,&tdptr,mtype);
                            ptr = ptemp;     /* decl ptr to register */
                            --blklev;
                            if (ptr == NULL) {
                                identerr();
                                goto next;
                            }
                            if (ptr->type != UNDECL) {
                                if (ptr->blklev == blklev) {
                                    if (ptr->type != dtype
                                                || ptr->storage != MOS
                                                || ptr->offset != offset)
                                        error("struct member mismatch");
                                } else pushdown(ptr);
                            }
                            if (dtype == USTRUCT)
                                    error("undefined structure");
                            ptr->type = dtype;
                            ptr->storage = MOS;
                            ptr->offset = offset;
                            ptr->x.elems = elocal;
                            if (s = sizeup(ptr,tdptr,msize)) {
                                if (type == STRUCT) tsize = (offset += s);
                                else tsize = s > tsize ? s : tsize;
                            } else sizerr();
                            ptr->blklev = blklev;
                            ptr->snext = vlist;
                            vlist = ptr;

                            if (type == STRUCT) {
                                eptr = (elem *) grab(sizeof(elem));
                                eptr->element = ptr;
                                if (*ellist) elast->strnext = eptr;
                                else *ellist = eptr;
                                elast = eptr;
                            }
next:
                            if (sym != COMMA) break;
                            getsym();
                        }
                    } while(sym == SEMICOL);
                    --mosflg;	/* get back out of "struct mode" */
                    if (tagptr) {
                        tagptr->size = tsize;
                        tagptr->type = STRUCT;
                        tagptr->x.elems = *ellist;
                    }
                    need(RBRACE);
                    break;
                }
        }
    } else if (sym == NAME) {
        ptr = (symnode *) symval;
        if (ptr->storage == TYPEDEF) {
            *size = ptr->size;
            *dimptr = ptr->dimptr;
            *ellist = ptr->x.elems;
            getsym();
            return ptr->type;
        }
    }
    *size = tsize;
    *dimptr = NULL;
    return type;
}


declarator(ptr,dptr,bastype)
symnode **ptr;
dimnode **dptr;
{
    register dimnode *tempdim, *p, *p1;
    int dtype, savmos, count;
    auto dimnode *dummy;

    *ptr = NULL;
    dtype = 0;

    while (sym == STAR) {
        dtype = incref(dtype);
        getsym();
    }
    if (sym == NAME) {
        *ptr = (symnode *) symval;
        getsym();
    } else if (sym == LPAREN) {
        getsym();
        ++blklev;
        bastype = declarator(ptr,dptr,bastype);
        --blklev;
        need(RPAREN);
    }

    if (sym == LPAREN) {
        dtype = (dtype<<2) + FUNCTION;
        if (blklev) {
            declist(&dummy);
            clear(&dummy);
        } else declist(&arglist);
    } else {
        tempdim = p1 = NULL;
        savmos = mosflg;
        count = mosflg = 0;
        while (sym == LBRACK) {
            dtype = (dtype << 2) + ARRAY;
            getsym();
            p = (dimnode *) grab(sizeof(dimnode));
            if (count == 0 && sym == RBRACK) p->dim = 0;
            else p->dim = constexp(0);

            if (p1) p1->dptr = p;
            else tempdim = p;
            p1 = p;
            need(RBRACK);
            ++count;
        }
        mosflg = savmos;
        if (tempdim) {
            if (dummy = *dptr) {
                while (dummy->dptr) dummy = dummy->dptr;
                dummy->dptr = tempdim;
            } else {
                p->dptr = NULL;
                *dptr = tempdim;
            }
        }
    }

    return shiftin(bastype,dtype);
}


shiftin (a,b)
{
    int temp;

    temp = a;
    while (temp & XTYPE) {
        temp >>= 2;
        b <<= 2;
    }
    return (a + b);
}


sizeup(ptr,dimptr,size)
register symnode *ptr;
register dimnode *dimptr;
{
    register int n,temp;

    switch (btype(temp = ptr->type)) {
        case CHAR:
        case UCHAR:     n = 1; break;
        case INT:
        case UNSIGN:    n = INTSIZE; break;
        case LONG:
        case ULONG:     n = LONGSIZE; break;
#ifdef  DOFLOATS
        case FLOAT:     n = FLOATSIZE; break;
        case DOUBLE:    n = DOUBLESIZE; break;
#endif
        case UNION:
        case STRUCT:    n = size; break;
        case USTRUCT:   n = 0; break;
    }
    ptr->size = n ? n : size;

    return getsize(temp,n,ptr->dimptr = dimptr);
}


getsize(t,size,dptr)
register dimnode *dptr;
{
    int n;

    if (isptr(t) || isftn(t)) return POINTSIZE;

    if (isary(t)) {
        n = 1;
        do {
            n *= dptr->dim;
            dptr = dptr->dptr;
        } while (isary(t = decref(t)));

        return n * (isptr(t) ? POINTSIZE : size);
    }
    return size;
}


clear(list)
symnode **list;
{
    register symnode *this, *next, *p, **pp;
    char err[60];

    this = *list;

    while (this) {
        next = this->snext;
        if (this->type == LABEL) {
/*
            if(!(this[ELEMS] & GONETO)) {
                strncpy(err,this+(SNAME>>1),8);
                strcat(err," : label unused");
                error(err);
            }
*/
            if(!(this->x.labflg & DEFINED)) {
                error(strncat(strcpy(err,"label undefined : "),
                                this->sname,8));
            }
        }

        switch (this->storage) {
#ifdef USE_YREG
            case YREG:
#endif
            case UREG: --reguse;
        }

        pp = (symnode **) this->downptr;
        if ((pp >= hashtab && pp < hashtab + 128)
                        || (pp >= mostab && pp < mostab + 128)) {
            extern symnode *freesym;

            if (*pp == this) *pp = this->hlink;
            else {
                for (p = *pp; p->hlink != this; p = p->hlink) ;
                p->hlink = this->hlink;
            }
            this->hlink = freesym;
            freesym = this;
        } else pullup(this);
        this = next;
    }

    *list = NULL;
}


sizerr()
{
    error("cannot evaluate size");
}


identerr()
{
    error("identifier missing");
}

