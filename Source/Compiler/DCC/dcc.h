#include <stdio.h>

#if defined(_OS9) || defined(_OSK) || defined(_OS9000)
# define MWOS	1	/* running on a Microware OS */
#endif

#ifndef MWOS
# define direct
#endif

#ifndef TRUE
# define TRUE		1
# define FALSE		0
#endif

#define VERSION		1
#define REVISION	0
#define PATCHLVL	0

#define PROGNAME	"dcc"
#define PREPROC		"dcpp"
#define COMPILER	"dcc68"
#define OPTIMIZER	"dco68"
#define ASSEMBLER	"rma"
#define LINKER		"rlink"

direct int	aflag = FALSE,
			bflag = FALSE,
			cflag = FALSE,
			fflag = FALSE,
			kflag = FALSE,
			lgflg = FALSE,
			llflg = FALSE,
			lv2flag = FALSE,
			lsflg = FALSE,
			mflag = FALSE,
			oflag = TRUE,
			o2flg = FALSE,
			pflag = FALSE,
			p2flg = FALSE,
			qflag = FALSE,
			rflag = FALSE,
			sflag = FALSE,
			s2flg = FALSE,
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

direct char	*thisfilp = 0,
			*lasfilp = 0;

direct int	frkprmsiz = 0;
direct char	*frkprmp = 0;

direct char	*tmproot[] = { "/R", "/R0", "/DD/TMP"};

direct char	*tmptail = "ctmp.XXXXXX";
direct char	*libdir = "/lib/";
direct char	*defdrive = "/dd";

char		tmpname[64] = "";
char		rlib[60] = "";

char		mainline[30] = "cstart.r";

char		*libarray[4],
			*xtramem,
			*modname,
			*edition;

char		*namarray[100],
			suffarray[100],
			*macarray[100],
			ofn[60],
			destfile[60],
			srcfile[60],
			objname[60],
			parmbuf[4096],
			devnam1[20];

char *chkccdev();
