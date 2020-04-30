/*
** function to convert a string into soundex string
*/

soundex(is, os)
char  *is, *os;
   {
   char  c;

   *os = *is;                           /* copy first char unchanged */

   while (c = sindex(*++is))
      if ((c != '0') && (c != *os))
            *++os = c;             /* copy if not vowel or duplicate */

   *++os = '\0';                             /* terminate the string */
   }


/*
** translates a char to the soundex value.
**  vowels and non alpha return '0', else
**   1 <- BFPV
**   2 <- CGJKQSXZ
**   3 <- DT
**   4 <- L
**   5 <- MN
**   6 <- R
*/
static char stable[] = "01230120022455012623010202";

sindex(c)
char  c;
   {
   c = toupper(c) - 'A';                  /* convert to index 0 - 25 */
 
   if ((c < 0) || (c > 25))
         return (0);                                 /* not a letter */
      else
         return (stable[c]);             /* return the soundex value */
   }
