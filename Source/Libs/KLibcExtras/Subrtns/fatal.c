/*
** print a fatal error message and exit
*/

fatal(msg, arg1, arg2, arg3)
char  *msg;
int   arg1, arg2, arg3;
   {
   fprintf(stderr, "%s: ", mod);
   fprintf(stderr, msg, arg1, arg2);
   exit (1);
   }


