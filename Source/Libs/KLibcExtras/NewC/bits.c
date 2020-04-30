

bset(flag, n)
int  flag, n;
   {
   return (flag |= (0x01 << n));
   }

bclr(flag, n)
int  flag, n;
   {
   return (flag &= ~(0x01 << n));
   }

btst(flag, n)
int  flag, n;
   {
   return (flag & (0x01 << n));
   }

