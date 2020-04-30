/*
 * strtok
 *  uses calls to strpbrk and strspn to break a string into
 *   tokens on sequentially subsequent calls.
 *  returns NULL when all tokens are consumed.
 *  first arg is NULL on 'subsequent' calls.
 */

#define  NULL  (char *)0

extern   int   strspn();
extern   char  *strpbrk();

char  *
strtok(string, sepset)
char  *string, *sepset;
   {
   register char  *p;
   static   char  *save;

   if ((p = (string == NULL) ? save : string) == NULL)
      return (NULL);                            /* no tokens remain */

   p += strspn(p, sepset);               /* skip leading seperators */

   if (*p == '\0')
      return (NULL);                            /* no tokens remain */

   if ((save = strpbrk(p, sepset)) != NULL)  /* find next seperator */
      *save++ = '\0';                       /* terminate this token */

   return (p);
   }
