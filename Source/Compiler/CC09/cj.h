#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*	Canonicalize our system definitions: __unix__, _OS9, _OSK, _OS9000
 */
#if defined(unix) || defined(__linux__) || defined(__MACH__)
# ifndef __unix__
#  define __unix__	1
# endif
#endif

#ifdef OS9
# ifndef _OS9
#  define _OS9	1
# endif
#endif

#ifdef OSK
# ifndef _OSK
#  define _OSK	1
# endif
#endif

/*	In some cases, we just want to know if we're running on a Microware-style
 *	operating system, because that means we've got paths like /dd/foo.
 */
#if defined(_OS9) || defined(_OSK) || defined(_OS9000)
# define MWOS	1
#endif

#ifdef MAIN
# define global
#else
# define global	extern
#endif

/*	Floating-point format
 *
 *	The native compiler uses a variant of PDP-11 D_floating format for
 *	'double'. The cross-compiler tries to convert the system's native double
 *	format into as close a form as possible. 'float' isn't really a problem
 *	until we have prototypes.
 */
#ifdef __STDC_IEC_559__
# define IEEE_FLOATS
#endif /* __STDC_IEC_559__ */

/*	Endianness definitions.
 *
 *	We want the UNIX-style BYTE_ORDER/foo_ENDIAN definitions as well as the
 *	simple flag-based ones OSK/OS-9000 use today.
 */
#ifdef __unix__
# include <unistd.h>
# include <endian.h>
# if BYTE_ORDER == BIG_ENDIAN
#  define _BIG_END 1
# elif BYTE_ORDER == LITTLE_ENDIAN
#  define _LIL_END 1
# elif BYTE_ORDER == PDP_ENDIAN
#  define _PDP_END 1
# else
#  error "unknown endianness, can't continue"
# endif
#else /* not UNIX */
# define LITTLE_ENDIAN	1234
# define BIG_ENDIAN	4321
# define PDP_ENDIAN	3412
# if defined(MWOS)
#  if defined(_OS9) || defined(_OSK) || defined(_BIG_END) /* predefined bigendian */
#   define BYTE_ORDER	BIG_ENDIAN
#   ifndef _BIG_END
#    define _BIG_END 1
#   endif
#  elif defined(_LIL_END)
#   define BYTE_ORDER	LITTLE_ENDIAN
#  endif /* done with MWOS */
# elif defined(_WIN32)
#  define BYTE_ORDER	LITTLE_ENDIAN
#  define _LIL_END 1 /* done with Win32 */
# else
#  warning "unknown system, arbitrarily assuming big-endian"
#  define BYTE_ORDER	BIG_ENDIAN
#  define _BIG_END 1
# endif
#endif /* not UNIX */

/*  Machine/Operating System dependent definitions.
 *
 *  see also 'local.c'.
 */

#ifdef _OS9
# define INTTYPE	int
# define LONGTYPE	long
# define FLOATYPE	float
# define DBLETYPE	double
# define ALLOCATE	ibrk
#endif

#ifdef _OSK
# define INTTYPE	short
# define LONGTYPE	long
# define FLOATYPE	float
# define DBLETYPE	double
# define ALLOCATE	ebrk
#endif

#ifdef __unix__
# include <string.h>
# include <fcntl.h>

# define direct
# define register
# define INTTYPE	short
# define LONGTYPE	long
# define FLOATYPE	float
# define DBLETYPE	double
# define ALLOCATE	sbrk
#endif

#define MICRO               /* suppresses word boundary forcing */
#define DOFLOATS            /*  include floating point code */
#define FNAMESIZE   30      /* file name size */

#define LINESIZE    256
#define MODNAMSIZE  15      /* module name */

#define NAMESIZE    8       /* identifiers for assembler */
#define UNIQUE      '_'     /* for local labels etc. */
#define NEWLINE     13

typedef struct structstruct {
    struct structstruct *strnext;   /* next element in structure */
    struct symstr *element;         /* ptr to mos entry */
} elem;

typedef struct dimstr {
    short dim;              /* size of this dimension */
    struct dimstr *dptr;    /* ptr to next dimension */
} dimnode;

typedef struct downsym {
    short type;             /* type of entry */
    int size;               /* size of entry */
    dimnode *dimptr;        /* ptr to list of dimensions */
    int offset;             /* general purpose */
    int storage;            /* storage class */
    union {
        elem *elems;        /* chain of structure members */
        int labflg;         /* label flags */
    } x;
    short blklev;           /* block level of declaration */
    struct symstr *downptr, /* ptr to 'hidden' outer declaration */
                  *snext;   /* ptr to entry in various lists */
} pshnode;

#define DOWNSIZE sizeof(pshnode)

typedef struct symstr {
    short type;             /* type of entry */
    int size;               /* size of entry */
    dimnode *dimptr;        /* ptr to list of dimensions */
    int offset;             /* general purpose */
    int storage;            /* storage class */
    union {
        elem *elems;        /* chain of structure members */
        int labflg;         /* label flag */
    } x;
    short blklev;           /* block level of declaration */
    struct symstr *downptr, /* ptr to 'hidden' outer declaration */
                  *snext,   /* ptr to entry in various lists */
                  *hlink;   /* ptr to next symbol in hash chain */
    char sname[NAMESIZE+1]; /* name entry */
} symnode;

#define SYMSIZE sizeof(symnode)

typedef struct expstr {
    short type;             /* type of node */
    int size;               /* size of basic type */
    dimnode *dimptr;        /* ptr to list of dimensions */
    short op;               /* operator */
    union {
        int num;            /* numeric value */
        symnode *sp;        /* symbol pointer */
        long *lp;           /* long constant ptr */
#ifdef DOFLOATS
        FLOATYPE *fp;       /* float constant ptr */
        DBLETYPE *dp;       /* double constant ptr */
#endif
    } val;                  /* value or symbol ptr */
    struct expstr *left,    /* left node ptr */
                  *right;   /* right node ptr */
    short lno;              /* line # of token causing this node */
    char *pnt;              /* ptr to line position of token */
    short sux;              /* pseudo Sethi-Ullman number */
    short modifier;         /* added to NAME references */
} expnode;

#define NODESIZE sizeof(expnode)

typedef struct initstruct {
    struct initstruct *initnext;    /* next initializer */
    expnode *initp;                 /* ptr to initializer expression */
    symnode *initname;              /* ptr to dude being initialized */
} initnode;

/* object sizes - char is by definition 1 */
#define INTSIZE     2
#define LONGSIZE    4

#ifdef  DOFLOATS
#define FLOATSIZE   4
#define DOUBLESIZE  8
#endif

#define POINTSIZE   2

/* operator precedence levels & associativity */
#define LEV_15  15  /* post-inc/dec, calls, array [], struct ./-> (ltr) */
#define LEV_14  14  /* pre-inc/dec, !/~, unary +/-, casts, ptr &* sizeof (rtl) */
#define LEV_13  13  /* multiply/divide/modulus (ltr) */
#define LEV_12  12  /* addition/subtraction (ltr) */
#define LEV_11  11  /* bitwise shifts (ltr) */
#define LEV_10  10  /* relational tests, < > <= >= (ltr) */
#define LEV_9   9   /* Test for (in)equality (ltr) */
#define LEV_8   8   /* bitwise AND (ltr) */
#define LEV_7   7   /* bitwise XOR (ltr) */
#define LEV_6   6   /* bitwise OR (ltr) */
#define LEV_5   5   /* logical AND (ltr) */
#define LEV_4   4   /* logical OR (ltr) */
#define LEV_3   3   /* ternary conditional (rtl) */
#define LEV_2   2   /* assignments (rtl) */
#define LEV_1   1   /* comma (ltr) */
#define LEV_0   0

/*  symbols  */
#define POINTER     0x10    /* type is "pointer to [basic type]" */
#define ARRAY       0x20    /* type is "array of [basic type]" */
#define FUNCTION    0x30    /* type is "function returning [basic type]" */

#define XTYPE       0x30    /* bit mask for type modifiers */
#define BASICT      0x0F    /* bit mask for just the basic type */

/* basic types */
#define UNDECL      0   /* type hasn't been declared */
#define INT         1   /* most convenient integer type */
#define CHAR        2   /* smallest unit of memory */
#define LONG        3   /* long int (and type modifier token) */
#define UNSIGN      4   /* unsigned int (and unsigned modifier token) */
#define UCHAR       5   /* unsigned char */
#define ULONG       6   /* unsigned long int */
#ifdef  DOFLOATS
#define FLOAT       7   /* 32-bit floating point */
#define DOUBLE      8   /* 64-bit floating point */
#endif
#define STRUCT      9
#define UNION       10
#define LABEL       11
#define USTRUCT     12  /* incomplete struct or union type */
/* 13, 14, 15 free */

/* storage classes */
#define ARG         16  /* function arguments */
#define AUTO        17  /* "auto" storage (on stack) */
#define STRTAG      18  /* struct tag */
#define EXTERN      19  /* stored somewhere else */
#define STATIC      20  /* global but "hidden" */
#define REG         21  /* register storage */
#define MOS         22  /* member of struct or union */
#define DIRECT      23  /* on direct page */
#define EXTERND     24  /* extern direct */
#define STATICD     25  /* static direct */
#define EXTDEF      26
/* 27..31 free */
#define RETURN      32
#define IF          33
#define WHILE       34
#define ELSE        35
#define SWITCH      36
#define CASE        37
#define BREAK       38
#define CONTIN      39
#define DO          40
#define DEFAULT     41
#define FOR         42
#define GOTO        43
#define TYPEDEF     44
#define DEFTYPE     45
#define CAST        46

#define SIGN        47  /* mostly ignored */
#define SHORT       48  /* short type modifier */

/* 49..63 reserved to avoid conflicting with functions returning types */

#define SEMICOL     64  /* ; expression terminator */
#define LBRACE      65  /* { block start */
#define RBRACE      66  /* } block end */
#define LBRACK      67  /* [ array index start */
#define RBRACK      68  /* ] array index end */
#define LPAREN      69  /* ( left parenthesis */
#define RPAREN      70  /* ) right paren */
#define COLON       71  /* : lots of uses */
#define COMMA       72  /* , comma operator */
#define KEYWORD     73
#define NAME        74
#define CONST       75  /* int constant */
#define STRING      76  /* string constant */
#define SIZEOF      77
#define INCBEF      78  /* ++x */
#define DECBEF      79  /* --x */
#define INCAFT      80  /* x++ */
#define DECAFT      81  /* x-- */
#define NOT         82  /* ! logical NOT */
#define AMPER       83  /* & address-of operator */
#define STAR        84  /* * pointer dereference, multiply */
#define NEG         85  /* - minus/negative */
#define COMPL       86  /* ~ complement */
#define DOT         87  /* . struct member access */
#define ARROW       88  /* -> struct member pointer access */
#define DBLAND      89  /* && logical AND */
#define DBLOR       90  /* || logical OR */
#define LCONST      91  /* long constant */
#ifdef DOFLOATS
#define FCONST      92  /* float constant */
#endif
/* HACK ALERT
    These token numbers must be in the same order as the DIV/MOD/SHR numbers.
    When processing division and shifts, the compiler sometimes subtracts the
    relation DIV-UDIV to get the unsigned operation.
*/
#define UDIV        93  /* unsigned integer division */
#define UMOD        94  /* % modulus (unsigned) */
#define USHR        95  /* >> (unsigned) */
#define RSUB        96
/* HACK ALERT
    These token numbers must be in the same order as the ASSPLUS numbers, or
    stuff *will* break. When processing assignment operators, the compiler
    generates the math operation by subtracting the difference between ASSPLUS
    and PLUS.
*/
#define PLUS        97  /* + add/positive */
#define MINUS       98
#define TIMES       99
#define DIV         100  /* / division character */
#define MOD         101  /* % modulus character */
#define SHR         102
#define SHL         103
#define AND         104
#define OR          105
#define XOR         106  /* ^ caret character */

/* conditional branch types */
#define EQ          107
#define NEQ         108
/* HACK ALERT
    The fact that these numbers are in exactly this order is used by the code.
    Don't change these numbers in relation to one another!
*/
#define LEQ         109
#define LT          110  /* < less than character */
#define GEQ         111
#define GT          112  /* > greater than character */
#define ULEQ        113
#define ULT         114
#define UGEQ        115
#define UGT         116

#define QUERY       117 /* ? conditional operator */
#define CALL        118
#define BSLASH      119 /* \ backslash character */
#define SHARP       120 /* # pound char (unused) */
#define PRIME       121 /* ' char constant start/end */
#define QUOTE       122 /* " quote mark */
#define LETTER      123
#define DIGIT       124
#define NEWLN       125 /* newline character (unused) */
#define SPACE       126 /* ' ' -- space character */

#define ASSIGN      127 /* = equals character */

/* code generator symbols */
#define STACK       128
#define DREG        129 /* in register D */
#define UREG        130 /* in register U */
#define XREG        131 /* in register X */
#define YREG        132 /* in register Y */
#define EXG         133 /* exchange two registers */
#define LEA         134
#define LOAD        135 /* load data into register */
#define NODE        136
#define STORE       137
#define PUSH        138
#define LEAX        139
#define JMP         140
#define JMPEQ       141
#define JMPNE       142
#define LOADIM      143 /* load immediate value into register */
#define FREG        144

#define COMPARE     145
#define CNDJMP      146 /* conditional jump */
#define ITOL        147 /* int to long */
#define LTOI        148 /* long to int */
#define CTOI        149 /* char to int */
#define UTOI        150 /* unsigned char to int */
#define UTOL        151 /* unsigned to long */
#define DBLOP       152 /* double operation (any) */
#define LONGOP      153 /* long operation (any) */
#define MOVE        154
#define STOI        155 /* short to int? (unused) */
#define TEST        156
#ifdef DOFLOATS
#define FTOD        157 /* float to double */
#define DTOF        158 /* double to float */
#define ITOD        159 /* int to double */
#define DTOI        160 /* double to int or unsigned */
#define LTOD        161 /* long to double */
#define DTOL        162 /* double to long */
#define UTOD        163 /* unsigned to double */
#endif
#define XIND        164 /* x indexed */
#define YIND        165 /* y indexed */
#define UIND        166 /* u indexed */

#define HALVE       167 /* division by two */
#define UHALVE      168 /* same (unsigned) */
#define IDOUBLE     169 /* multiplication by two */
#define CHALVE      170 /* char division by two */
#define UCHALVE     171 /* uchar division by two */

/* 172..189 reserved for expansion */

#define ASSPLUS     190 /* += */
#define ASSMIN      191 /* -= */
#define ASSMUL      192 /* *= */
#define ASSDIV      193 /* /= */
#define ASSMOD      194 /* %= */
#define ASSSHR      195 /* >>= */
#define ASSSHL      196 /* <<= */
#define ASSAND      197 /* &= */
#define ASSOR       198 /* |= */
#define ASSXOR      199 /* ^= -- rarely seen */
/*
 * THIS IS THE END.
 * Do not add tokens higher than 199 without also changing the size of the kw[]
 * array below and in misc.c!
 */

/* indirection masks */
#define INDIRECT    0x8000
#define NOTIND      0x7FFF

/* label tests */
#define DEFINED     1
#define GONETO      2

/* dp indicators */
#define DDP         1  /* on dp */
#define NDP         0  /* not on dp */

#define UNKN        0

#ifdef TRUE
#undef TRUE
#undef FALSE
#endif

#define TRUE        1
#define FALSE       0
#define ERR         0xFFFF

#define TAB_CHAR    9
#define LINEFEED    10
#define FORMFEED    12
#define RET_CHAR    13
#define VTAB_CHAR   11
#define BACKSPACE   8

#define btype(t)    ((t) & BASICT)
#define isptr(t)    (((t) & XTYPE) == POINTER)
#define isary(t)    (((t) & XTYPE) == ARRAY)
#define isftn(t)    (((t) & XTYPE) == FUNCTION)
#define modtype(t,b) (((t) & (~BASICT)) + b)

#define getlabel(a) (++curlabel)

#define islong(t)   ((t)->type == LONG || (t)->type == ULONG)
#define isuint(t)   ((t)->type == UNSIGN || (t)->type == UCHAR || (t)->type == ULONG)
#ifdef DOFLOATS
#define isfloat(t)  ((t)->type == FLOAT || (t)->type == DOUBLE)
#endif

global direct FILE  *in,    /* input file pointer */
                    *code;  /* code file pointer */

global direct int   lineno,     /* current line number */
                    errcount,
                    curlabel,   /* next internal label no. */
                    sp,         /* current stack pointer offset */
                    stlev,      /* stack reservation level */
                    callflag,   /* function call flag */
                    sflag,      /* suppress stack checking code */
#ifdef PROF
                    pflag,      /* if set generate profile calls */
#endif
                    ftype,      /* type of current function */
                    lastst,     /* last statement type */
                    blklev,     /* current block level */
                    breaklab,   /* break label */
                    cntlab,     /* continue label */
                    swflag,     /* current switch level */
                    mosflg,     /* member of structure flag */
                    sym,        /* current lexical token */
                    symval,     /* and its associated value */
                    scount,     /* chars on current line of string */
                    symline,    /* lineno of current symbol */
                    maxpush;    /* maximum push level */

global char filename[FNAMESIZE];    /* current filename buffer */

global direct symnode *freedown,  /* list of free push-down entries */
                    *arglist,   /* list of arguments */
                    *labelist,  /* list of labels    */
                    *vlist;     /* list of declared variables */

global direct char  *symptr,    /* start of current symbol */
                    cc,         /* current character */
                    *lobrk,     /* lower store bound */
                    *hibrk,     /* upper store bound */
                    *lptr;      /* input line pointer */

global direct FILE  *strfile;   /* strings output file */

extern char     chartab[128],   /* character translater table */
                valtab[128];    /* operator precedence table */

global symnode  *hashtab[128],  /* main hash table */
                *mostab[128];   /* structure hash table */

#ifdef PTREE
global char *kw[200];           /* pointers to symbol names */
# ifndef CKEYSFILE
#  ifdef MWOS
#   define CKEYSFILE "/dd/lib/ckeys"
#  else
#   define CKEYSFILE "/usr/local/share/dcc/ckeys"
#  endif
# endif
#endif

global direct int   dflag,  /* debug flag; if set causes tree prints */
                    lflag,  /* if set, source lines copied to code file */
                    tcount;

global symnode  sdummy;         /* dummy expression node */

global expnode  *freenode;      /* free expression node list */

global char line[LINESIZE],     /* input line buffer */
            lastline[LINESIZE]; /* saved last line */

extern  char    strname[];      /* string file name */

/*  label state structure  */
typedef struct {
    int     labnum,
            labsp;
#ifdef REGCONTS
    expnode *labdreg,
            *labxreg;
#endif
} labstruc;


/*  function type definitions  */
char *grab();

extern  expnode *parsexp(), *primary(), *explist(), *getcast(),
                *newnode(), *optim(), *fold(), *chtype(), *fixup(),
                *tranexp(), *tranbool(), *treecopy();

extern  dimnode *dimwalk();
