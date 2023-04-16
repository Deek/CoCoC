#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <endian.h>

#if !(defined(_OS9) || defined(_OSK))
# define direct
# define register
#endif

#ifdef __GCC__
# define packed __attribute__((__packed__))
#else
# define packed
#endif

/* rdump:  prints formatted dump of .r and .l files

     rdump [opts] <file>[ <file>] [opts]

     options:
          -g - add global definition info
          -r - add reference info
          -o - add reference and local offset info
          -a - all of the above
*/

/* symbol table types */

/* symbol definition/reference type/location */

/* type flags */
#define CODENT      0x04        /* data/code flag */
/* data type flags */
#define DIRENT      0x02        /* global/direct flag */
#define INIENT      0x01        /* clear/init. data flag */

/* location flags */
#define CODLOC      0x20        /* data/code flag */
#define DIRLOC      0x10        /* global/direct flag */
#define LOC1BYT     0x08        /* two/one byte size flag */
#define LOCMASK     (CODLOC|DIRLOC)

#define NEGMASK     0x40        /* negate on resolution */
#define RELATIVE    0x80        /* relative reference */

/* misc. constants */
#if BYTE_ORDER == LITTLE_ENDIAN
# define ROFSYNC	0x8723CD62
#else
# define ROFSYNC	0x62CD2387
#endif
#define SYMLEN      9         /* Length of symbols */
#define MAXNAME     16        /* length of module name */

/* definition/reference */
typedef struct {
     char r_flag; /* type/location */
     uint16_t r_offset;
} def_ref;

/* rof header structure */
typedef packed struct {
     uint32_t h_sync;
     uint16_t h_tylan;
     char h_valid;
     char h_date[5];
     char h_edit;
     char h_vers;
     uint16_t h_glbl;
     uint16_t h_dglbl;
     uint16_t h_data;
     uint16_t h_ddata;
     uint16_t h_ocode;
     uint16_t h_stack;
     uint16_t h_entry;
} binhead;

#define MAXSOURCE 20
#define puts(s) fputs(s,stdout)
#define mc(c) ((unsigned char) (c))

#define DEF 1
#define REF 2

direct char *snames[MAXSOURCE],*fname;
direct int scount;
direct int gflag,rflag,oflag;

binhead hd;
FILE *in;

#ifdef _OS9
# define mgw getw
#else
int16_t
mgw (inf)
FILE *inf;
{
	unsigned char high = getc (inf);
	unsigned char low = getc (inf);
	return (high << 8) + low;
}
#endif

getref (out)
def_ref *out;
{
#ifdef _OS9
	fread (out, sizeof (def_ref), 1, in);
#else
	out->r_flag = getc (in);
	out->r_offset = mgw (in);

	if (ferror (in)) ferr (fname);
#endif
}

int
gethead (out)
binhead *out;
{
#ifdef _OS9
	return fread (out, sizeof (binhead), 1, in);
#else
	if (fread (out, 4, 1, in) == 0)
		return 0;

	out->h_tylan = mgw (in);
	out->h_valid = getc (in);
	fread (out->h_date, 5, 1, in);
	out->h_edit = getc (in);
	out->h_vers = getc (in);
	out->h_glbl = mgw (in);
	out->h_dglbl = mgw (in);
	out->h_data = mgw (in);
	out->h_ddata = mgw (in);
	out->h_ocode = mgw (in);
	out->h_stack = mgw (in);
	out->h_entry = mgw (in);

	return 1;
#endif
}

int
main(argc,argv)
int argc;
char **argv;
{
     register char *p;

     while (--argc>0) {
          if(*(p=*++argv)=='-') {
               while(*++p) switch(*p) {
                    case 'g': gflag=1; break;
                    case 'r': rflag=1; break;
                    case 'o': oflag=1; break;
                    case 'a': gflag=rflag=oflag=1; break;
                    default:  error("unknown option -%c",*p);
               }
               done: ;
          }
          else {
               if(scount==MAXSOURCE) error("too many source files");
               snames[scount++]=*argv;
          }
     }
     pass1();
}

char *
plural(num)
{
     static char *yes = "s";
     static char *no = "";

     return num==1 ? no : yes;
}

pass1()
{
     int count;

     if(scount == 0)
          return;

     for(count=0; count<scount; ++count) {
          fname = snames[count];
          if((in = fopen(fname,"r")) == NULL) {
               error ("can't open '%s'", fname);
               continue;
          }

          for(;;) {
               if (!gethead (&hd)) break;

               if (hd.h_sync != ROFSYNC) {
                    error ("'%s' is not a relocatable module", fname);
                    break;
               }

               showhead();
               showglobs();

/* skip code and initialized data */
               fseek(in, (long) (hd.h_ocode + hd.h_ddata + hd.h_data), SEEK_CUR);

               showrefs();
               showlcls();

               switch (hd.h_vers) {
                    case 1:
                         showcomm();
			     case 0:
                         break;
                    default:
                         error("can't process version %d object files", hd.h_vers);
               }
          }
          fclose(in);
     }
}

showhead()
{
     int c;

     puts("\nModule name: ");
     while(c=getc(in))
          putchar(c);
     if(ferror(in)) ferr(fname);

     printf("\nTyLa/RvAt:   %02x/%02x\n",mc(hd.h_tylan>>8),mc(hd.h_tylan));
     printf("Asm valid:   %s\n",hd.h_valid ? "No" : "Yes");
     printf("Create date: %.3s %2d, %4d %02d:%02d\n",
          &("JanFebMarAprMayJunJulAugSepOctNovDec"[(mc(hd.h_date[1])-1)*3]),
          mc(hd.h_date[2]),1900+mc(hd.h_date[0]),
          mc(hd.h_date[3]),mc(hd.h_date[4]));
     printf("Edition/ROF: %d/%d\n", hd.h_edit, hd.h_vers);
       puts("  Section    Init Uninit\n");
     printf("   Code:     %04x\n",hd.h_ocode);
     printf("     DP:       %02x   %02x\n",hd.h_ddata,hd.h_dglbl);
     printf("   Data:     %04x %04x\n",hd.h_data,hd.h_glbl);
     printf("  Stack:     %04x\n",hd.h_stack);
     printf("Entry point: %04x\n",hd.h_entry);
}

showglobs()
{
     register uint16_t count, offset;
     char sym[SYMLEN+1],flag;

     count = mgw (in);        /* global def count */
     if(gflag)
          printf("\n%u global symbol%s defined:\n",count,plural(count));

     while(count--) {
          getname(sym);
          flag=getc(in);
          offset = mgw (in);
          if(gflag) {
               printf(" %9s %04x ",sym,offset);
               ftext(flag,DEF);
          }
     }
}

getname(s)
register char *s;
{
     while(*s++ = getc(in));
     *s = '\0';
     if(ferror(in)) ferr(fname);
}

ftext(c,ref)
char c;
int ref;
{

     printf("(%02x) ",mc(c));
     if(ref & REF) {
          if(c & CODLOC)
               puts("in code");
          else puts(c & DIRLOC ? "in dp data" : "in non-dp data");

          puts(c & LOC1BYT ? "/byte" : "/word");
          if(c & NEGMASK)
               puts("/neg");
          if(c & RELATIVE)
               puts("/pcr");
     }
     if(ref & DEF) {
          if(ref & REF)
               puts(" - ");
          if(c & CODENT)
               puts("to code");
          else {
               puts(c & DIRENT ? "to dp" : "to non-dp");
               puts(c & INIENT ? " data" : " bss");
          }
     }
     putchar('\n');
}

showrefs()
{
     register uint16_t count, rcount;
     def_ref ref;
     char sym[SYMLEN+1];
     int fflag;

     count = mgw (in);
     if(rflag)
          printf("\n%u external reference%s:\n",count, plural(count));

     while(count--) {
          getname(sym);
          rcount = mgw (in);

          if(rflag)
               printf(" %9s ",sym);
          fflag=0;
          while(rcount--) {
               getref (&ref);

               if(rflag && oflag) {
                    if (fflag)
                         puts("           ");
                    else fflag=1;
                    printf("%04x ",ref.r_offset);
                    ftext(ref.r_flag,REF);
               }
          }
          if(rflag && !oflag)
               putchar('\n');
     }
}

showlcls()
{
     register unsigned count;
     def_ref ref;

     count = mgw (in);
     if(oflag)
          printf("\n%u local reference%s:\n",count, plural(count));

     while(count--) {
          getref (&ref);

          if(oflag) {
               printf("   %04x ",ref.r_offset);
               ftext(ref.r_flag,DEF | REF);
          }
     }
}

showcomm()
{
     register unsigned count,rcount,rsize;
     def_ref ref;
     char sym[SYMLEN+1];
     int fflag;

     count = mgw (in);
     if(rflag)
          printf("\n%u common block%s:\n",count, plural(count));

     while(count--) {
          getname(sym);
          rsize = mgw (in);
          rcount = mgw (in);
          if(rflag)
               printf(" %9s, size %u, %u reference%s:\n",sym,rsize,rcount,plural(rcount));
          fflag=1;
          while(rcount--) {
               getref (&ref);

               if(rflag && oflag) {
                    puts("           ");
                    printf("%04x ",ref.r_offset);
                    ftext(ref.r_flag,REF);
               }
          }
          if(rflag && !oflag)
               putchar('\n');
     }
}


error(s1,s2,s3,s4)
{
     fprintf(stderr,"rdump: ");
     fprintf(stderr,s1,s2,s3,s4);
     putc('\n',stderr);
     exit(1);
}

ferr(s)
{
     error("error reading '%s'",s);
}

