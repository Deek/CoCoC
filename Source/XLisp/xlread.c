/* xlread - xlisp expression input routine */

#ifdef AZTEC
#include "a:stdio.h"
#else
#include <stdio.h>
#include <ctype.h>
#endif

#include "xlisp.h"

/* global variables */
struct node *oblist;

/* external variables */
extern struct node *xlstack;
extern int (*xlgetc)();
extern int xlplevel;

/* local variables */
static int savech;

/* forward declarations (the extern hack is for decusc) */
extern struct node *parse();
extern struct node *plist();
extern struct node *pstring();
extern struct node *pnumber();
extern struct node *pquote();
extern struct node *pname();

/* xlread - read an xlisp expression */
struct node *xlread()
{
    /* initialize */
    savech = -1;
    xlplevel = 0;

    /* parse an expression */
    return (parse());
}

/* parse - parse an xlisp expression */
static struct node *parse()
{
    int ch;

    /* keep looking for a node skipping comments */
    while (TRUE)

        /* check next character for type of node */
        switch (ch = nextch()) {
        case '\'':                      /* a quoted expression */
                return (pquote());
        case '(':                       /* a sublist */
                return (plist());
        case ')':                       /* closing paren - shouldn't happen */
                xlfail("extra right paren");
        case '.':                       /* dot - shouldn't happen */
                xlfail("misplaced dot");
        case ';':                       /* a comment */
                pcomment();
                break;
        case '"':                       /* a string */
                return (pstring());
        default:
                if (isdigit(ch))        /* a number */
                    return (pnumber(1));
                else if (issym(ch))     /* a name */
                    return (pname());
                else
                    xlfail("invalid character");
        }
}

/* pcomment - parse a comment */
static pcomment()
{
    /* skip to end of line */
    while (getch() != '\n')
        ;
}

/* plist - parse a list */
static struct node *plist()
{
    struct node *oldstk,val,*lastnptr,*nptr;
    int ch;

    /* increment the nesting level */
    xlplevel += 1;

    /* create a new stack frame */
    oldstk = xlsave(&val,NULL);

    /* skip the opening paren */
    savech = -1;

    /* keep appending nodes until a closing paren is found */
    for (lastnptr = NULL; (ch = nextch()) > 0 && ch != ')'; lastnptr = nptr) {

        /* check for a dotted pair */
        if (ch == '.') {

            /* skip the dot */
            savech = -1;

            /* make sure there's a node */
            if (lastnptr == NULL)
                xlfail("invalid dotted pair");

            /* parse the expression after the dot */
            lastnptr->n_listnext = parse();

            /* make sure its followed by a close paren */
            if (nextch() != ')')
                xlfail("invalid dotted pair");

            /* done with this list */
            break;
        }

        /* allocate a new node and link it into the list */
        nptr = newnode(LIST);
        if (lastnptr == NULL)
            val.n_ptr = nptr;
        else
            lastnptr->n_listnext = nptr;

        /* initialize the new node */
        nptr->n_listvalue = parse();
    }

    /* skip the closing paren */
    savech = -1;

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* decrement the nesting level */
    xlplevel -= 1;

    /* return successfully */
    return (val.n_ptr);
}

/* pstring - parse a string */
static struct node *pstring()
{
    struct node *oldstk,val;
    char sbuf[STRMAX+1];
    int ch,i,d1,d2,d3;

    /* create a new stack frame */
    oldstk = xlsave(&val,NULL);

    /* skip the opening quote */
    savech = -1;

    /* loop looking for a closing quote */
    for (i = 0; i < STRMAX && (ch = getch()) != '"'; i++) {
        switch (ch) {
        case '\\':
                switch (ch = getch()) {
                case 'e':
                        ch = '\033';
                        break;
                case 'n':
                        ch = '\n';
                        break;
                case 'r':
                        ch = '\r';
                        break;
                case 't':
                        ch = '\t';
                        break;
                default:
                        if (ch >= '0' && ch <= '7') {
                            d1 = ch - '0';
                            d2 = getch() - '0';
                            d3 = getch() - '0';
                            ch = (d1 << 6) + (d2 << 3) + d3;
                        }
                        break;
                }
        }
        sbuf[i] = ch;
    }
    sbuf[i] = 0;

    /* initialize the node */
    val.n_ptr = newnode(STR);
    val.n_ptr->n_str = strsave(sbuf);

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the new string */
    return (val.n_ptr);
}

/* pnumber - parse a number */
static struct node *pnumber(sign)
  int sign;
{
    struct node *val;
    int ch,ival;

    /* loop looking for digits */
    for (ival = 0; isdigit(ch = thisch()); savech = -1)
        ival = ival * 10 + ch - '0';

    /* make sure the number terminated correctly */
    if (issym(ch))
        xlfail("badly formed number");

    /* initialize the node */
    val = newnode(INT);
    val->n_int = sign * ival;

    /* return the new number */
    return (val);
}

/* xlenter - enter a symbol into the symbol table */
struct node *xlenter(sname)
  char *sname;
{
    struct node *sptr;

    /* check for nil */
    if (strcmp(sname,"nil") == 0)
        return (NULL);

    /* check for the oblist being undefined */
    if (oblist == NULL) {
        oblist = newnode(SYM);
        oblist->n_symname = strsave("oblist");
        oblist->n_symvalue = newnode(LIST);
        oblist->n_symvalue->n_listvalue = oblist;
    }

    /* check for symbol already in table */
    for (sptr = oblist->n_symvalue; sptr != NULL; sptr = sptr->n_listnext)
        if (sptr->n_listvalue == NULL)
            printf("bad oblist\n");
        else if (sptr->n_listvalue->n_symname == NULL)
            printf("bad oblist symbol\n");
        else
        if (strcmp(sptr->n_listvalue->n_symname,sname) == 0)
            return (sptr->n_listvalue);

    /* enter a new symbol and link it into the symbol list */
    sptr = newnode(LIST);
    sptr->n_listnext = oblist->n_symvalue;
    oblist->n_symvalue = sptr;
    sptr->n_listvalue = newnode(SYM);
    sptr->n_listvalue->n_symname = strsave(sname);

    /* return the new symbol */
    return (sptr->n_listvalue);
}

/* pquote - parse a quoted expression */
static struct node *pquote()
{
    struct node *oldstk,val;

    /* create a new stack frame */
    oldstk = xlsave(&val,NULL);

    /* skip the quote character */
    savech = -1;

    /* allocate two nodes */
    val.n_ptr = newnode(LIST);
    val.n_ptr->n_listvalue = xlenter("quote");
    val.n_ptr->n_listnext = newnode(LIST);

    /* initialize the second to point to the quoted expression */
    val.n_ptr->n_listnext->n_listvalue = parse();

    /* restore the previous stack frame */
    xlstack = oldstk;

    /* return the quoted expression */
    return (val.n_ptr);
}

/* pname - parse a symbol name */
static struct node *pname()
{
    char sname[STRMAX+1];
    int ch,i;

    /* get the first character */
    ch = sname[0] = getch();

    /* check for signed number */
    if (ch == '+' || ch == '-') {
        if (isdigit(thisch()))
            return (pnumber(ch == '+' ? 1 : -1));
    }

    /* get symbol name */
    for (i = 1; i < STRMAX && issym(thisch()); i++)
        sname[i] = getch();
    sname[i] = 0;

    /* initialize value */
    return (xlenter(sname));
}

/* nextch - look at the next non-blank character */
static int nextch()
{
    /* look for a non-blank character */
    while (isspace(thisch()))
        savech = -1;

    /* return the character */
    return (thisch());
}

/* thisch - look at the current character */
static int thisch()
{
    /* return and save the current character */
    return (savech = getch());
}

/* getch - get the next character */
static int getch()
{
    int ch;

    /* check for a saved character */
    if ((ch = savech) >= 0)
        savech = -1;
    else
        ch = (*xlgetc)();

    /* check for the abort character */
    if (ch == EOF)
        if (xlplevel > 0) {
            putchar('\n');
            xltin(FALSE);
            xlfail("input aborted");
        }
        else
            exit();

    /* return the character */
    return (ch);
}

/* issym - check whether a character if valid in a symbol name */
static int issym(ch)
  int ch;
{
    if (isspace(ch) ||
        ch <  ' ' ||
        ch == '(' ||
        ch == ')' ||
        ch == ';' || 
        ch == '.' ||
        ch == '"' ||
        ch == '\'')
        return (FALSE);
    else
        return (TRUE);
}