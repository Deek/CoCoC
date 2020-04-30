



/* strncpy
** copies at most n chars from s2 to s1
**  if s2 < n, s1 is padded with nulls
**  returns s1
*/
strncpy(s1, s2, n)
char *s1;
register char *s2;
int n;
   {
   char *p = s1;
   while ((n-- > 0) && (*p++ = *s2++))
      ;
   while (n-- > 0)
      *p++ = 0;
   return (s1);
   }



/* strcmp
**  compares s2 to s1 thru null on s2 or at most n chars
**  returns 0 if equalthru null, or s1 > or < s2 at the nth char
*/
strncmp(s1, s2, n)
register char *s1;
char *s2;
int n;
   {
   while ((n-- > 0) && (*s1 == *s2)) {
      if (*s2++ == 0)
         return (0);
      s1++;
      }
   return ((n < 0) ? 0 : (*s1 - *s2));
   }



/* strncat
**  copies s2 thru null to end of s1, over null on s1
**  but at most n chars
**  s1 terminated if n > 0
**  returns s1
*/
strncat(s1, s2, n)
char *s1;
register char *s2;
int n;
   {
   char *p = s1;
   while (*p++)
      ;
   p--;
   while ((n-- > 0) && (*p++ = *s2++))
      ;
   if (n < 0)
      *p = 0;
   return (s1);
   }



/* strhcpy
**  copies thru char with b7 set
**  returns s1
*/
strhcpy(s1, s2)
char *s1;
char *s2;
   {
   register char *p = s1;
   while ((*p++ = *s2++) > 0)
      ;
   *(p-1) &= 0x7f;
   *p = 0;
   return (s1);
   }
