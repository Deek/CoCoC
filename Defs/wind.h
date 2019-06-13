/* Wind.h - Defs for window data structure */

/* window type defs */
#define WT_NBOX 0
#define WT_FWIN 1
#define WT_FSWIN 2
#define WT_SBOX 3
#define WT_DBOX 4
#define WT_PBOX 5

#define MNENBL 1
#define MNDSBL 0 
#define WINSYNC 0xC0C0 

/* default menu ids */
#define MN_MOVE 1
#define MN_CLOS 2
#define MN_GROW 3
#define MN_USCRL 4
#define MN_DSCRL 5
#define MN_RSCRL 6
#define MN_LSCRL 7
#define MN_TNDY 20
#define MN_FILE 21
#define MN_EDIT 22
#define MN_STYL 23
#define MN_FONT 24

/* window - menu data structures */

                        /* menu item descriptor */
typedef struct mistr {
    char _mittl[15];    /* name of item */
    char _mienbl;       /* is this item available? */
    char _mires[5];     /* reserved */
} MIDSCR;

                        /* menu descriptor */
typedef struct mnstr {
    char _mnttl[15];    /* name of menu */
    char _mnid,         /* menu ID number */
         _mnxsiz,       /* width of menu */
         _mnnits,       /* number of items in menu */
         _mnenabl,      /* is this menu available? */
         _mnres[2];     /* reserved */
    struct mistr *_mnitems; /* pointer to array of menu item descriptors */
} MNDSCR;


/* window descriptor */
typedef struct wnstr {
    char _wnttl[20];    /* title of window */
    char _nmens;        /* number of menus on window */
    char _wxmin,        /* minimum width of window */
         _wymin;        /* minimum height of window */
    short _wnsync;      /* synch bytes $C0C0 */
    char _wnres[7];     /* reserved */
    struct mnstr *_wnmen; /* pointer to array of menu descriptors */
} WNDSCR;
