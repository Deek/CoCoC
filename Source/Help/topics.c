/*   This program is in the Public Domain. */

#include "help.h"
#include <dir.h>
#include <stdio.h>

/* define TAB width */
#define TAB 11

topics()
{
   DIR *dirp;
   struct direct *direntry;
   char name[32];  /* Long enough for a file name */
   char *p;
   int height, width;
   int column=0;
   int linelen=0;
   int nextpos=0;
   int i;
   int quit=0;

   _gs_scsz(1,&width,&height); /* Get screen size */

   quit=outline("Further information available for:");
   dirp = opendir(".");
   *scratch = '\0';

   while (  (direntry=readdir(dirp)) && !quit) {
      strcpy(name,direntry->d_name);
      if ( ((p=rindex(name,'.')) != NULL) && (name[0] != '.') ) {
         *p='\0';
         if ( (nextpos+strlen(name)+1 >= width)  /* If this will fit */
            && nextpos ) {  /* Must already have one word on this line */
            quit= outline(scratch);
            *scratch = '\0';
            linelen = nextpos = 0; /* Start with a clean line */
         }
         for(;linelen<nextpos;scratch[linelen++]=' ') /* do Tab */
             ;
         scratch[linelen]='\0';
         strcat(scratch,name);
         linelen = linelen+strlen(name);
         nextpos = (linelen/TAB+1)*TAB;
      }
   }
   outline(scratch);
   outline("");  /* Add a blank line for aesthetics */
   closedir(dirp);
}
