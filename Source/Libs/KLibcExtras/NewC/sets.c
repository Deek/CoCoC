/*
**   S E T S  routines to manage sets in C
*/

/*
** makeset allocates memory for a set of 'size' characters.
**  success returns a pointer to the set, NULL on error.
*/

char *allocset()
   {
   return (malloc(32));                                  /* 256 bits */
   }


/*
** addc2set adds the character 'c' to the set 's', no error possible.
*/

char *addc2set(s, c)
char  *s, c;
   {
   s[(c >> 3) & 0x1f] |= (1 << (c & 7));
   return(s);
   }


/*
** adds2set adds string 'p' to the set 's', no error possible.
*/

char *adds2set(s, p)
char  *s, *p;
   {
   for (; *p; p++)
      s[(*p >> 3) & 0x1f] |= (1 << (*p & 7));
   return(s);
   }


/*
** rmfmset removes the char 'c' from the set 's', no error possible.
*/

char *rmfmset(s, c)
char  *s, c;
   {
   s[(c >> 3) & 0x1f] &= ~(1 << (c & 7));
   return (s);
   }


/*
** member returns TRUE if 'c' is a member of set 's', else FALSE.
*/

smember(s, c)
char  *s, c;
   {
   return (s[(c >> 3) & 0x1f] & (1 << (c & 7)));
   }
/*page*/
/*
** union - merge second set into first
*/

char *sunion(s1, s2)
char  s1[], s2[];
   {
   int   j;

   for (j = 31; j >= 0; j--)
      s1[j] |= s2[j];
   return (s1);
   }


/*
**  intersect - set1 returns elements only existing in both
*/

char *sintersect(s1, s2)
char  s1[], s2[];
   {
   int   j;

   for (j = 31; j >= 0; j--)
      s1[j] &= s2[j];
   return (s1);
   }


/*
** difference - return elts only in first
*/

char *sdifference(s1, s2)
char  s1[], s2[];
   {
   int   j;

   for (j = 31; j >= 0; j--)
      s1[j] ^= s2[j];
   return (s1);
   }


/*
** copyset - duplicates set2 into set1
*/

char *copyset(s1, s2)
char  s1[], s2[];
   {
   int   j;

   for (j = 31; j >= 0; j--)
      s1[j] = s2[j];
   return (s1);
   }


/*
** dupset - allocates a new set and copies 'set' to it.
*/

char *dupset(s)
char  s[];
   {
   return (copyset(allocset(), s);
   }
