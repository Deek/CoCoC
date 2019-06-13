/*#define  OS9   /* Define for OS-9 Level I/II   */
/*#define  UNIX  /* Define for UNIX SYSV, Ultrix, BSD */
/*#define  VMS   /* Define for VMS x.x */
/*#define  IBMPC /* Define for IBM-PC under Quick C */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* System-specific defines */

#ifdef UNIX
#include <time.h>
#define NAMELEN 29
#endif

#ifdef OS9
#include <utime.h>
#define fputc putc
#define NAMELEN 29
#endif

#ifdef VMS
#include <time.h>
#define NAMELEN 55  /* Actually larger -- limited by size of I-packet */
#endif

#ifdef IBMPC
#include <time.h>
#define  NAMELEN 29
#endif

#define  BUFLEN     128
#define  MAXNAMELEN  55
#define  MAXNAMES    25
#define  FALSE        0
#define  TRUE         1
#define  NO           0
#define  YES          1

/* Three distinct file types recognized by CUTS:
 * ASC -- ASCII text files, CUTS will do EOL conversion
 * BIN -- portable binary files, e.g. graphics, etc.
 * MAC -- machine-specific binary files, e.g. executables
 *                        ( Changed 1 Nov 89 -- TBK)
 */
#define  ASCIIFILE    1
#define  BINARYFILE   2
#define  MACHINEFILE  3

#define  CR          13
#define  LF          10

#define IPACKET   (buff[0]=='.' && buff[5]=='.' && buff[6]=='I')
#define PREFIX(x) (((x)>=0x21) && ((x)<=0x29))
#define SAME(x) ((((x)>=0x2a)&&((x)<=0x5a))||(((x)>=0x61)&&((x)<= 0x7a)))
#define CLOSEIN(x)  if ((x) != stdin) fclose(x)
#define CLOSEOUT(x) if (!output_file) fclose(x)

extern int quiet;  /* added 28Mar89 -TBK */

