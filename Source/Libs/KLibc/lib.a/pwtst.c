#include <stdio.h>

main(argc, argv)
int   argc;
char  **argv;
   {
   char  buf[64];

   strcpy(buf, argv[1]);
   pwcryp(buf);
   puts(buf);
   }
