/* xlisp - a small subset of lisp */

/* system specific definitions */

/* DEFEXT       define to enable default extension of '.lsp' on 'load' */
/* FGETNAME     define if system supports 'fgetname' */
/* NNODES       number of nodes to allocate in each request */
/* TDEPTH       trace stack depth */
/* KEYMAPCLASS  define to include the 'Keymap' class */

/* for the VAX-11 C compiler */
#ifdef vms
#define DEFEXT
#define FGETNAME
#define KEYMAPCLASS
#define NNODES  2000
#define TDEPTH  1000
#endif

/* for the DECUS C compiler */
#ifdef decus
#define DEFEXT
#define KEYMAPCLASS
#define NNODES  200
#define TDEPTH  100
#endif

/* for unix compilers */
#ifdef unix
#define KEYMAPCLASS
#define NNODES  200
#define TDEPTH  100
#endif

/* for the AZTEC C compiler */
#ifdef AZTEC
#define DEFEXT
#define KEYMAPCLASS
#define NNODES  200
#define TDEPTH  100
#define getc(fp)        getch(fp)
#define kbin()          CPM(6,0xFF)
#define malloc          alloc
#define strchr          index
#endif

/* default important definitions */
#ifndef NNODES
#define NNODES  200
#endif
#ifndef TDEPTH
#define TDEPTH  100
#endif

/* useful definitions */
#define TRUE    1
#define FALSE   0

/* program limits */
#define STRMAX  100     /* maximum length of a string constant */

/* node types */
#define FREE    0
#define SUBR    1
#define LIST    2
#define SYM     3
#define INT     4
#define STR     5
#define OBJ     6
#define FPTR    7
#define KMAP    8

/* node flags */
#define MARK    1
#define LEFT    2

/* string types */
#define DYNAMIC 0
#define STATIC  1

/* symbol structure */
struct xsym {
    char *xsy_name;             /* symbol name */
    struct node *xsy_value;     /* the current value */
};

/* subr node structure */
struct xsubr {
    struct node *(*xsu_subr)(); /* pointer to an internal routine */
};

/* list node structure */
struct xlist {
    struct node *xl_value;      /* value at this node */
    struct node *xl_next;       /* next node */
};

/* integer node structure */
struct xint {
    int xi_int;                 /* integer value */
};

/* string node structure */
struct xstr {
    int xst_type;               /* string type */
    char *xst_str;              /* string pointer */
};

/* object node structure */
struct xobj {
    struct node *xo_obclass;    /* class of object */
    struct node *xo_obdata;     /* instance data */
};

/* file pointer node structure */
struct xfptr {
    FILE *xf_fp;                /* the file pointer */
};

/* keymap structure */
struct xkmap {
    struct node *(*xkm_map)[];  /* selection pointer */
};


/* shorthand macros for accessing node substructures */

/* symbol node */
#define n_symname       n_info.n_xsym.xsy_name
#define n_symvalue      n_info.n_xsym.xsy_value

/* subr node */
#define n_subr          n_info.n_xsubr.xsu_subr

/* list node (and message node and binding node) */
#define n_listvalue     n_info.n_xlist.xl_value
#define n_listnext      n_info.n_xlist.xl_next
#define n_msg           n_info.n_xlist.xl_value
#define n_msgcode       n_info.n_xlist.xl_next
#define n_bndsym        n_info.n_xlist.xl_value
#define n_bndvalue      n_info.n_xlist.xl_next
#define n_left          n_info.n_xlist.xl_value
#define n_right         n_info.n_xlist.xl_next
#define n_ptr           n_info.n_xlist.xl_value

/* integer node */
#define n_int           n_info.n_xint.xi_int

/* string node */
#define n_str           n_info.n_xstr.xst_str
#define n_strtype       n_info.n_xstr.xst_type

/* object node */
#define n_obclass       n_info.n_xobj.xo_obclass
#define n_obdata        n_info.n_xobj.xo_obdata

/* file pointer node */
#define n_fname         n_info.n_xfptr.xf_name
#define n_fp            n_info.n_xfptr.xf_fp

/* node structure */
struct node {
    char n_type;                /* type of node */
    char n_flags;               /* flag bits */
    union {                     /* value */
        struct xsym n_xsym;     /*     symbol node */
        struct xsubr n_xsubr;   /*     subr node */
        struct xlist n_xlist;   /*     list node */
        struct xint n_xint;     /*     integer node */
        struct xstr n_xstr;     /*     string node */
        struct xobj n_xobj;     /*     object node */
        struct xfptr n_xfptr;   /*     file pointer node */
        struct xkmap n_xkmap;   /*     key map node */
    } n_info;
};

/* key map node */
#define n_kmap          n_info.n_xkmap.xkm_map

/* external procedure declarations */
extern struct node *xlread();           /* read an expression */
extern struct node *xleval();           /* evaluate an expression */
extern struct node *xlarg();            /* fetch an argument */
extern struct node *xlevarg();          /* fetch and evaluate an argument */
extern struct node *xlmatch();          /* fetch an typed argument */
extern struct node *xlevmatch();        /* fetch and evaluate a typed arg */
extern struct node *xlsend();           /* send a message to an object */
extern struct node *xlmfind();          /* find the method for a message */
extern struct node *xlxsend();          /* execute a message method */
extern struct node *xlenter();          /* enter a symbol into the oblist */
extern struct node *xlsave();           /* generate a stack frame */
extern struct node *xlobsym();          /* find an object's class or instance
                                           variable */
extern struct node *xlclass();          /* enter a class definition */
extern struct node *xlivar();           /* get an instance variable */
extern struct node *xlcvar();           /* get an instance variable */
extern struct node *newnode();          /* allocate a new node */

extern char *stralloc();                /* allocate string space */
extern char *strsave();                 /* make a safe copy of a string */