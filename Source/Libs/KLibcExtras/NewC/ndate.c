#include <stdio.h>
#include <utime.h>

long  t;

main(argc, argv)
int   argc;
char  **argv;
   {
   t = time(0);
   printf("%s", ctime(&t));
   }
