
char  *dm;
unsigned space;

main()
   {
   datlink("common.dat", &dm, &space);
   lockdata(dm);
   unlkdata(dm);
   dunlink(dm);
   }
