

/*  Data type definitions  */
#ifdef OSK
typedef  int      void;    /*  No value returned - for portability  */
typedef  short    bool;                /*  An integer boolean type  */
typedef  char     cbool;              /*  A character boolean type  */
#define  DIRECT
#else
typedef  int      void;    /*  No value returned - for portability  */
typedef  int      bool;                /*  An integer boolean type  */
typedef  char     cbool;              /*  A character boolean type  */
#define  DIRECT   direct
#endif

/*  Macro functions  */

#define  SWAP(a, b, t)  ((t) = (a), (a) = (b), (b) = (t))
#define  max(x, y)      (((x) < (y)) ? (y) : (x))
#define  min(x, y)      (((x) < (y)) ? (x) : (y))

/*  Some extra defines for often used constants  */

#define  FALSE    0
#define  TRUE     (FALSE == 0)
#define  YES      1
#define  NO       0
