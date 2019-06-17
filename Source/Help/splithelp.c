/*
 * Split /dd/sys/helpmsg into separate files suitable for use with
 *   my help utility.
 *  Released into Public Domain, November, 1990 -- Tim Kientzle
 */

#include <stdio.h>

#define HELP_DIR  "./"
#define HELP_FILE "helpmsg"

main()
{
   FILE *help_file, *out_file;
   char line[200],filename[200], *j;

   line[0]='\0';
   printf("Opening Tandy help file \"%s\".\n",HELP_FILE);
   help_file = fopen(HELP_FILE,"r");

   if (help_file == NULL) {
      printf("Couldn't open Tandy help file \"%s\".\n",HELP_FILE);
      exit(1);
   }

   while (line[0] != '@')
      if (fgets(line,200,help_file) == NULL) exit(2); 

   do {
      for( j=line ; *j != '\0' ; j++ )
             if (*j== '\n' || *j== ' ' )  *j = '\0' ;
      strcpy(filename,HELP_DIR);
      strcat(filename,line+1);
      strcat(filename,".hlp");
      maklower(filename);
      printf("Creating file \"%s\".\n",filename);
      out_file = fopen(filename,"w");
      if (out_file == NULL) exit(3);

      do {
          if (fgets(line,200,help_file)==NULL && !feof(help_file)) {
              printf("Error reading Tandy help file.\n");
              exit(4);
          }
          else if (line[0] !='@')
              fputs(line,out_file);
      } while (line[0] !='@' && !feof(help_file) ) ;

      fclose(out_file);
   } while(!feof(help_file));
}

maklower(str)
  char *str;
{
   while( *str != '\0' ) {
      *str = tolower(*str);
      str++;
   }
}
