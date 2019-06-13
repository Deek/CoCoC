#include "cuts.h"

/* PD implementation of getopt, I procurred off the net SrT */

/*
 * get option letter from argument vector
 */
int opterr = 0, /* if 1, don't print error message on illegal option */
    optind = 1, /* index into parent argv vector */
    optopt;     /* character checked for validity */
char *optarg;   /* argument associated with option */

#define BADCH   (int)'?'
#define EMSG    ""

#define tell0(s) fputs(*nargv,stderr);fputs(s,stderr);fputc(optopt,stderr)
#define tell(s)  { tell0(s); fputc('\n',stderr); }

getopt(nargc,nargv,ostr)
   int nargc;
   char **nargv,*ostr;
{
   static char *place = EMSG;  /* option letter processing */
   register char *oli;         /* option letter list index */
   char *strchr();

   /* update scanning pointer */
   if (!*place)
   {
      if (optind >= nargc 
         || *(place = nargv[optind]) != '-' || !*++place)
      {
         place = EMSG;
         return(EOF);
      }

      /* found "--" */
      if (*place == '-')
      {
         ++optind;
         place = EMSG;
         return(EOF);
      }
   }

   /* option letter okay? */
   if ((optopt = (int)*place++) == (int)':' 
         || !(oli = strchr(ostr,optopt)))
   {
      if (!*place)
         ++optind;
      if (!opterr)
         tell(": illegal option -- ");
      return(BADCH);
   }
   /* don't need argument */
   if (*++oli != ':')
   {
      optarg = NULL;
      if (!*place)
         ++optind;
   }
   /* need an argument */
   else
   {
      if (*place)                  /* no white space */
         optarg = place;
      else if (nargc <= ++optind)  /* missing argument */
      {
         if (!opterr)
            tell(": option requires an argument -- ");
         place = EMSG;
         return(BADCH);
      }
      else                         /* white space */
         optarg = nargv[optind];
      place = EMSG;
      ++optind;
   }
   /* dump back option letter */
   return(optopt);
}