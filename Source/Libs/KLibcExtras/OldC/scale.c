/*static*/ double atoftbl[] = {
   0.5,
   1e1,
   1e2,
   1e3,
   1e4,
   1e5,
   1e6,
   1e7,
   1e8,
   1e9,
   1e10,
   1e20,
   1e30 };

static double
scalit(d, cnt, sign)
double   d;
int      cnt, sign;
   {
   if (cnt)
      return (sign ? d * atoftbl[cnt] : d / atoftbl[cnt]);
   return (d);
   }


double
scale(d, cnt, sign)
double   d;
int      cnt, sign;
   {
   if (cnt > 9)
      d = scalit(d, cnt / 10 + 9, sign);
   return (scalit(d, cnt % 10, sign));
   }
