#include <modes.h>

#define  DIRMAK   S_IFDIR+S_IOEXEC+S_IOREAD+S_IEXEC+S_IWRITE+S_IREAD
#define  DIRCHK   S_IFDIR+S_IREAD

#ifdef   OSK
#define  MSYNC    0x4AFC
#else
#define  MSYNC    0x87CD
#endif

#define  FALSE    (0)
#define  TRUE     (!FALSE)
#define  ERROR    (-1)
#define  HID      "+AR0.0+"
#define  HIDSIZ   7
#define  SUF      ".ar"
#define  SUFSIZ   3
#define  FNSIZ    65
#define  MAXLINE  256

#define  PLAIN    0                         /* plain text or object */
#define  SQ       1                      /* old fashion CPM squeeze */
#define  COMP1    2                        /* LZ compression type 1 */
#define  COMP2    3                        /* LZ comperssion type 2 */



typedef struct {
      char     dir_name[28];
      long     dir_addr;          /* assumes that 29th char is null */
      } DIRENT;


typedef struct {                             /* obvious definitions */
      char     fd_attr;
      char     fd_own[2];
      char     fd_date[5];
      char     fd_link;
      char     fd_fsize[4];
      char     fd_dcr[3];
      } FILDES;


/* NOTE that a_size is on an even byte boundary */
typedef struct {
      char     a_hid[HIDSIZ+1];                 /* header id string */
      char     a_name[FNSIZ+1];        /* name of the archived file */
      long     a_size;         /* size of archive (not virgin) file */
      char     a_type;          /* archive type - virg, packed, etc */
      char     a_stat;        /* status of file - good, deleted, .. */
      FILDES   a_attr;           /* attributes of the archived file */
      } HEADER;


typedef struct fn {
      struct fn *fn_link;                 /* link to next file name */
      char     fn_name[1];                       /* the name itself */
      } FN;

