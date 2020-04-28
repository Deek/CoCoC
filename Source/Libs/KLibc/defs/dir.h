
struct direct {
   long  d_addr;        /* file desc addr */
   char  d_name[30];    /* dierctory entry name */
   } ;

typedef struct {
   int   dd_fd;         /* fd for open directory */
   char  dd_buf[32];    /* a one entry buffer */
   } DIR;

#define DIRECT       struct direct
#define rewinddir(a) seekdir(a, 0L)

extern DIR           *opendir();
extern DIRECT        *readdir();
extern long          telldir();
extern /* void */    seekdir(), closedir();
