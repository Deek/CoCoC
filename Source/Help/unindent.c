/*
 * For each file specified on the command line, this program will
 * read the file, determine the left margin (minimum number of spaces
 * appearing on each non-trivial line), and then remove that number
 * of spaces from each line.  Does not handle tabs.
 *
 * Used to compress long, formatted text files.  Released into Public
 * Domain.
 *			- Tim Kientzle
 */
#include <stdio.h>

char inbuff[1000];
char tmpname[50];
char fname[50];

main()
{
   FILE *fp,*tmp;
   int mintab;
   int i;
   long total;

   pflinit();total=0L;

   strcpy(tmpname,"UnTab_tmp_XXXXX");
   mktemp(tmpname);
   unlink(tmpname);

   while (gets(fname)) {
      if(fp=fopen(fname,"r+")) {
         /* Copy from fp to tmp, and find out how much to trim */
         mintab=1000;
         tmp=fopen(tmpname,"w");
         while(fgets(inbuff,1000,fp)) {
            fputs(inbuff,tmp);
            for(i=0;inbuff[i]==' ';i++) ;
            if(i<mintab && inbuff[i]!=0 && inbuff[i]!='\n') mintab=i;
         }
         fclose(tmp);
         fclose(fp);

         printf("%s: Minimum tab = %d\n",fname,mintab);

         if(mintab!=0) {
            /* Now, copy from tmp to fp, deleting extra spaces */
            fp=fopen(fname,"w");
            tmp=fopen(tmpname,"r");
            while(fgets(inbuff,1000,tmp)) {
               i=strlen(inbuff)-1;
               while(i>=0 && (inbuff[i]==' ' || inbuff[i]=='\n') )
                     i--;
               inbuff[++i]='\n'; inbuff[++i]='\0';
               for(i=0;i<mintab && inbuff[i]!='\n' && inbuff[i]; i++) ;
               fputs(inbuff+i,fp);
               total +=i;
            }
            fclose(fp);
            fclose(tmp);
         }
      } else
         printf("%s: error #%d\n",fname,errno);
  
   }
   unlink(tmpname);
   printf("Total # leading spaces stripped: %ld\n",total);
}