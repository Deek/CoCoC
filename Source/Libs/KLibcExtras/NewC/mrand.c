static long next = 1;

int rand()
   {
   next = next*1103515245l + 12345l;
   return ((unsigned int )(next / 65536l) % 32768l);
   }

srand(seed)
unsigned int seed;
   {
   next = seed;
   }
