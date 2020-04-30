#include <ctype.h>
#define  FALSE  0
#define  TRUE   (FALSE == 0)

double
atof(p)
register char  *p;
   {
   double   d, _dnorm(), scale();
   double   *_dneg();
   int      exp;
   int      negmant;
   int      posexp;
   int      fracdig;
   char     c;
   char     *dp;

   while (*p == ' ' || *p == '\t')
      ++p;

   negmant = (*p == '-');

   if (*p == '-' || *p == '+')
      ++p;

   d = 0;

   while (isdigit(c = *p))
      {
      dX10(&d, c);
      ++p;
      }

   fracdig = 0;

   if (*p == '.')
      {
      ++p;
      while (isdigit(c = *p))
         {
         dX10(&d, c);
         ++p;
         ++fracdig;
         }
      }

   dp = &d;
   dp[7] = 128 + 56;

   d = _dnorm(&d);

   if ((c = *p) == 'e' || c == 'E')
      {
      posexp = 1;
      if (*++p == '+')
         ++p;
      else
         if (*p == '-')
            {
            ++p;
            posexp = 0;
            }

      for (exp = 0; isdigit(c = *p++); )
         exp = exp * 10 + c - '0';

      fracdig += (posexp ? -exp : exp);
      }

   if (fracdig < 0)
      {
      fracdig = -fracdig;
      posexp = TRUE;
      }
   else
      posexp = FALSE;

   d = scale(d, fracdig, posexp);

   return (negmant ? -d : d);
   }


#asm
dX10 ldx 2,S
 leas -8,S
 ldd 5,X
 aslb 
 rola 
 std 5,X
 std 5,S
 ldd 3,X
 rolb 
 rola 
 std 3,X
 std 3,S
 ldd 1,X
 rolb 
 rola 
 std 1,X
 std 1,S
 lda ,X
 rola 
 sta ,X
 sta ,S
 asl 6,X
 rol 5,X
 rol 4,X
 rol 3,X
 rol 2,X
 rol 1,X
 rol ,X
 lbcs z03a5
 asl 6,X
 rol 5,X
 rol 4,X
 rol 3,X
 rol 2,X
 rol 1,X
 rol ,X
 lbcs z03a5
 ldd 5,X
 addd 5,S
 std 5,X
 ldd 3,X
 adcb 4,S
 adca 3,S
 std 3,X
 ldd 1,X
 adcb 2,S
 adca 1,S
 std 1,X
 ldb ,X
 adcb ,S
 stb ,X
 bcs z03a5
 ldb 13,S
 andb #$0f
 clra 
 addd 5,X
 std 5,X
 ldd #0
 adcb 4,X
 adca 3,X
 std 3,X
 ldd #0
 adcb 2,X
 adca 1,X
 std 1,X
 lda #0
 adca ,X
 sta ,X
 bcs z03a5
 leas 8,S
 clra 
 clrb 
 rts 


z03a5 ldd #1
 leas 8,S
 rts 

#endasm
