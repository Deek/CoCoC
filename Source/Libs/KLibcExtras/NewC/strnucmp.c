/* strnucmp
**  compares s2 to s1 thru null on s2 or at most n chars
**  returns 0 if equalthru null, or s1 > or < s2 at the nth char
*/
strnucmp(s1, s2, n)
register char *s1;
char *s2;
int n;
   {
   while ((n-- > 0) && (toupper(*s1) == toupper(*s2))) {
      if (*s2++ == 0)
         return (0);
      s1++;
      }
   return ((n < 0) ? 0 : (toupper(*s1) - toupper(*s2)));
   }
