/* strlen
**  returns length of s
*/
strlen(s)
char *s; {
   register char *c;
   c = s;
   while (*c++)
     ;
   return (c - s -1);
   }


/* strcpy
**  copies s2 thru null to s1
**  returns s1
*/
strcpy(s1, s2)
char *s1;
register char *s2;
   {
   char *p = s1;
   while (*p++ = *s2++)
        ;
   return(s1);
   }


/* strcat
**  copies s2 thru null to end of s1, over null on s1
**  returns s1
*/
strcat(s1, s2)
char *s1;
register char *s2;
   {
   char *p = s1;
   while (*p++)
      ;
   p--;
   while (*p++ = *s2++)
      ;
   return (s1);
   }


/* strcmp
**  compares s2 to s1 thru null on s2
**  returns 0 if equal, or s1 > or < s2
*/
strcmp(s1, s2)
register char *s1;
char *s2;
   {
   while (*s1 == *s2) {
      if (*s2++ == 0)
         return (0);
      s1++;
      }
   return (*s1 - *s2);
   }
