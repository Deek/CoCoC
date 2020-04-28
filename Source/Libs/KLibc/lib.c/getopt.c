#include <stdio.h>

#define index strchr
/*
 * get option letter from argument vector
 */
int opterr = 1,   /* useless, never set or used */
    optind = 1,   /* index into parent argv vector */
    optopt;       /* character checked for validity */
char *optarg;     /* argument associated with option */

#define BADCH   (int)'?'
#define EMSG    ""

#define tell0(s) fputs(*nargv,stderr); fputs(s,stderr); putc(optopt,stderr)
#define tell(s)  tell0(s); putc('\n',stderr); return(BADCH);

getopt(nargc,nargv,ostr)
   int nargc;
   char **nargv,*ostr;
{
   static char *place = EMSG;  /* option letter processing */
   register char *oli;         /* option letter list index */
   char *index();

   /* update scanning pointer */
   if (!*place)
   {
      if (optind >= nargc || *(place = nargv[optind]) != '-' || !*++place)
         return(EOF);

      /* found "--" */
      if (*place == '-')
      {
         ++optind;
         return(EOF);
      }
   } /* option letter okay? */

   if ((optopt = (int)*place++) == (int)':' || !(oli = index(ostr,optopt)))
   {
      if (!*place)
         ++optind;
      tell(": illegal option -- ");
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
      if (*place) 
         /* no white space */
         optarg = place;
      /* no arg */
      else if (nargc <= ++optind)
      {
         place = EMSG;
         tell(": option requires an argument -- ");
      }
      else 
         /* white space */
         optarg = nargv[optind];
      place = EMSG;
      ++optind;
   }
   /* dump back option letter */
   return(optopt);
}

