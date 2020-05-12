#include <stdio.h>

#define  VERSION     2
#define  MAJREV      5
#define  MINREV      2

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define  TWOPASS     TRUE    /* undefine for one pass compiler */
#define  ASSEMBLER   "rma"   /* change to "c.asm" if NOT Developer's Pak */
#define  NASSEMBLER  "r63"   /* alternate assembler for native .a source */
#define  LINKER      "rlink" /* change to "c.link" if NOT Developer's pak */

direct int  aflag = FALSE,
            Aflag = FALSE,
            bflag = FALSE,
            cflag = FALSE,
            fflag = FALSE,
            gtflg = FALSE,
            lgflg = FALSE,
            llflg = FALSE,
            lsflg = FALSE,
            mflag = FALSE,
            Nflag = FALSE,
            oflag = TRUE,
            o2flg = FALSE,
            pflag = FALSE,
            p2flg = FALSE,
            qflag = FALSE,
            rflag = FALSE,
            sflag = FALSE,
            s2flg = FALSE,
            tgfex = FALSE,
            tflag = FALSE,
            xflag = FALSE,
            zflag = FALSE,
            tmpdir = TRUE,

            childid = 0,
            childstat = 0,
            filcnt = 0,
            hello = FALSE,
            libcnt = 0,
            maccnt = 0,
            newopath = 0,
            nullflag = FALSE,
            rsufflg = FALSE,
            loopcnt;

direct char *thisfilp = 0,
            *lasfilp = 0;

direct int  frkprmsiz = 0;
direct char *frkprmp = 0;

char        *tmproot[] = {"/R", "/R0", "/DD/TEMP", "/DD/TMP"} ;
char        tmptail[] = "ctmp.XXXXXX";
char        tmpname[64] = "";
char        rlib[60] = "";

char        mainline[20] = "cstart.r";

char        *libarray[4],
            *namarray[100],
            suffarray[100],
            *macarray[100],
            ofn[60],
            *xtramem,
            *modname,
            *edition,
            destfile[60],
            srcfile[60],
            objname[60],
            parmbuf[4096],
            devnam1[20];
