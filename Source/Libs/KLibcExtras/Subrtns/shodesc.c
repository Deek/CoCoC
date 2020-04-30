shodesc(desc)
char *desc;
   {
   int i;
   for (i=0; i < 32; i++)
       {
       printf("%02x ",*desc++);
       if (i == 15) putchar('\n');
       }
   putchar('\n');
   putchar('\l');
   }
