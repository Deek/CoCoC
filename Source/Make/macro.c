/*
 *     Macro control for make
 */


#include "h.h"


struct macro *         macrohead;

#ifdef OS9
char *          object_dir=(char *)0;
char *          rel_dir=(char *)0;
#endif

struct macro *
getmp(name)
char *                 name;
{
       register struct macro * rp;

       for (rp = macrohead; rp; rp = rp->m_next)
               if (strcmp(name, rp->m_name) == 0)
                       return rp;
       return (struct macro *)0;
}


char *
getmacro(name)
char *                 name;
{
       struct macro *          mp;

       if (mp = getmp(name))
               return mp->m_val;
       else
               return "";
}


struct macro *
setmacro(name, val)
char *                 name;
char *                 val;
{
       register struct macro * rp;
       register char *         cp;

#ifdef DEBUG
       if (debug) printf("Setting macro '%s' = '%s'.\n",name,val);
#endif

                       /*  Replace macro definition if it exists  */
       for (rp = macrohead; rp; rp = rp->m_next)
               if (strcmp(name, rp->m_name) == 0)
               {
                       free(rp->m_val);        /*  Free space from old  */
                       break;
               }

       if (!rp)                /*  If not defined, allocate space for new  */
       {
               if ((rp = (struct macro *)malloc(sizeof (struct macro)))
                                        == (struct macro *)0)
                       fatal("No memory for macro");

               rp->m_next = macrohead;
               macrohead = rp;
               rp->m_flag = FALSE;

               if ((cp = malloc(strlen(name)+1)) == (char *)0)
                       fatal("No memory for macro");
               strcpy(cp, name);
               rp->m_name = cp;
       }

       if ((cp = malloc(strlen(val)+1)) == (char *)0)
               fatal("No memory for macro");
       strcpy(cp, val);                /*  Copy in new value  */

#ifdef OS9
      /* Under OS9, we recognize the special macro ODIR as setting the
      directory to look for object files (ones with no extension) */
       if (strcmp(name,"ODIR")==0) {
            object_dir = cp;
            if (debug) printf("Setting Object Dir to \"%s\".\n",object_dir);
       }
      /* RDIR is the directory where we look for .r files */
       if (strcmp(name,"RDIR")==0) {
            rel_dir = cp;
            if (debug) printf("Setting Rel Dir to \"%s\".\n",rel_dir);
       }
#endif
       rp->m_val = fix_macro(cp); /* Strip leading and trailing spaces. */

       return rp;
}

/*
 * Clean up a macro by removing leading and trailing spaces.
 */
fix_macro(string)
char *string;
{
   char  *p,*q,*r;

   p=q=r=string;
   while( *p == ' ' ) p++ ;  /* Skip leading spaces */
   while( *p != '\0') {
      *q++ = *p ;
      if ( *p != ' ' ) {  /* If the char is not a space */
         r=q;             /* then remember where it is. */
      }
      p++;
   }
   *r = '\0';      /* Terminate with null after last non-space */
   return(string);
}

/*
 *     Do the dirty work for expand
 */
void
doexp(to, from, len, buf)
char **                        to;
char *                 from;
int *                  len;
char *                 buf;
{
       register char *         rp;
       register char *         p;
       register char *         q;
       register struct macro * mp;
       register char *         add_slash;

       rp = from;
       p = *to;
       while (*rp)
       {
               if (*rp != '$')
               {
                       *p++ = *rp++;
                       (*len)--;
               }
               else
               {
                       q = buf;
                       if (*++rp == '{')
                               while (*++rp && *rp != '}')
                                       *q++ = *rp;
                       else if (*rp == '(')
                               while (*++rp && *rp != ')')
                                       *q++ = *rp;
                       else if (!*rp)
                       {
                               *p++ = '$';
                               break;
                       }
                       else
                               *q++ = *rp;
                       *q = '\0';
                       if (*--q == '/') {   /* If last char is a slash, */
                           add_slash = p ; /* .. set flag */
                           *q = '\0';    /* .. erase slash */
                       } else
                           add_slash = 0;
                       if (*rp)
                               rp++;
#ifdef DEBUG
                       if (debug)
                               printf("Expanding \"%s\".\n",buf);
#endif
                       if (!(mp = getmp(buf)))
                               mp = setmacro(buf, "");
                       if (mp->m_flag)
                               fatal("Infinitely recursive macro %s", mp->m_name);
                       mp->m_flag = TRUE;
                       *to = p;
                       doexp(to, mp->m_val, len, buf);
                       p = *to;
                       if (add_slash && (p != add_slash)) *p++ = '/';
                       mp->m_flag = FALSE;
               }
               if (*len <= 0)
                       error("Expanded line too long");
       }
       *p = '\0';
       *to = p;
}


/*
 *     Expand any macros in str.
 */
void
expand(str)
char *         str;
{
       static char             a[LZ];
       static char             b[LZ];
       char *                  p = str;
       int                     len = LZ-1;

       strcpy(a, str);
       doexp(&p, a, &len, b);
}