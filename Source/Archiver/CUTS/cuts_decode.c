#include "cuts.h"
char saved_ipacket[85];

decode(filename, output_file, ofptr)
   char *filename;
   int  output_file; /* True -> Output file specified on cmd line */
   FILE *ofptr;      /* If output file was specified, this is it */
{
   extern char checksum(), *trunc();
   char name[60], buff1[BUFLEN], date[8], type[4], version;
   char *buff;
   int line_number, ch, i, ok, first_ipacket, 
       asciiflag, done, line_no, nmat;
   FILE *infp, *outfp;

   if (strcmp(filename,"-"))    /* Not stdin */
   {
      if ((infp = fopen(filename,"r")) == NULL)
      {
         fprintf(stderr, "Error opening '%s'\n", filename);
         return;
      }
   }
   else
      infp = stdin;

   first_ipacket = TRUE;
   done = FALSE;
   /*
    * Loop through and do an entire file.
    */
   while (TRUE)
   {
      /*
       * Search for Identifier packets.
       */
      ok = TRUE;
      while (ok)
      {
         if (fgets(buff1,BUFLEN,infp) == NULL)
         {
            if (!first_ipacket && !done)
            {
               fprintf(stderr,"Final I-Packet missing in '%s'.\n",
                                                         filename);
               fprintf(stderr,"Possible loss of encoded files.\n");
            }
            return;
         }

         buff=buff1;
         while (*buff != '.' && *buff != '\0') buff++; /* Skip junk */

         if (IPACKET)
         {
            ok = FALSE;
            if (first_ipacket)
            {
               done = first_ipacket = FALSE;
               strncpy(saved_ipacket, buff, 79);
            }
            else if (strncmp(buff, saved_ipacket, 7) == 0)
               done = first_ipacket = ok = TRUE;
         }
         else
            if(quiet==0) fprintf(stderr, ">>>%s", buff1);
      }

      if (checksum(buff) != buff[78])
      {
         fprintf(stderr, "Checksum error with Identifier Packet\n");
         if (!cont_yn())
         {
            CLOSEIN(infp);
            return;
         }
      }

      sscanf(buff, ".%04d.I.%c.%6s.%3s.\"%[^\"]", &line_number, &version,
         date, type, name);
      asciiflag = (strcmp(type,"ASC") == 0);

      fprintf(stderr,"'%s' Encoded as '%3s' on '%6s' By Version '%c'\n",
         name, type, date, version);
      strcpy(name,trunc(name, NAMELEN));

      if (!output_file)
      {
         ok = TRUE;
#ifdef VMS
         if (asciiflag)
         {
            if ( (outfp = fopen(name,"w","rfm=var","rat=cr")) == NULL)
               ok = FALSE;
         }
         else
            if ( (outfp = fopen(name,"w","rfm=fix","mrs=512")) == NULL)
               ok = FALSE;
#else
         /*
          * Try opening file in header, if error try 'name.[0-9]'
          */
         if ( (outfp = fopen(name,"r")) == NULL)
         {
#ifdef IBMPC
            if (asciiflag) outfp = fopen(name,"w");
            else           outfp = fopen(name,"wb");
#else
            outfp = fopen(name,"w");
#endif
            if ( outfp == NULL)
               ok = file_open(outfp, name,asciiflag);
         }
         else
         {
            CLOSEOUT(outfp);
            ok = file_open(outfp, name,asciiflag);
         }
#endif

         if (!ok)
         {
            fprintf(stderr, "Unable to open a file for '%s'\n", name);
            CLOSEIN(infp);
            return;
         }
      }    
      else
         outfp = ofptr;

      /*
       * Start decoding
       */
      ok = TRUE;
      while (ok)
      {
         if (fgets(buff1, BUFLEN, infp) == NULL)
         {
            fprintf(stderr, 
               "Unexpected EOF in '%s'...closing files\n", name);
            CLOSEOUT(outfp);
            return;
         }

         buff=buff1;
         while (*buff != '.' && *buff != '\0') buff++;

         nmat = sscanf(buff, ".%04d.D", &line_no);
         if (nmat == 0)
            fprintf(stderr, "<<<%s", buff1);
         else
         {
            if (++line_number != line_no)
            {
               fprintf(stderr,
                  "Lines out of order or missing at %04d in '%s'\n",
                  line_number, name);
               line_number = line_no;
               ok = FALSE;
            }
            else if (strlen(buff) < 79)
            {
               fprintf(stderr,
                 "Line %04d too Short in '%s'\n",line_number,name);
               ok = FALSE;
            }
            else if (checksum(buff) != buff[78])
            {
               fprintf(stderr, "Checksum error on Line %04d in '%s'\n",
                  line_number, name);
               ok = FALSE;
            }

            if (!ok)
               if (cont_yn())
                  ok = TRUE;
               else
                  CLOSEOUT(outfp);

            if (ok)  ok = line_decode(outfp, buff, asciiflag);
         }
      }
   }
}

/*
 * Decode a single line.
 */
line_decode(outfp, line, asciiflag)
   FILE *outfp;
   char line[];
   int  asciiflag;
{
   register int i;
   char ch;
  
   for (i=7; i<77; i++)
   {
      if (line[i]=='#' && line[i+1] == '.')
         return(FALSE);

      ch = line[i];

      if (PREFIX(ch))
         ch = (ch - 0x21) * 0x20 + line[++i] - 0x30;

      if ((ch == CR) && asciiflag) /* CR is End-Of-Line marker in CUTS */
         fputs("\n",outfp);        /* ASC files -- translate to local  */
      else                         /* End-Of-Line marker               */
         fputc(ch,outfp);
  }

  if ((i==77) && !PREFIX(line[77]))
     fputc(line[77], outfp);
  
  return(TRUE);
}

#ifndef VMS        /* We don't need this if under VMS */
file_open(outfp,name,asciiflag)
   FILE *outfp;
   char name[];
   int  asciiflag;       /* added for fopen 7/1/89  DRI */
{
   char new_name[NAMELEN+1];
   int npos,pos;

   strcpy(new_name, name);
   strcat(new_name, ".");
   npos = strlen(new_name);
   new_name[npos+1] = '\0';
  
   for (pos=0; pos < 10; pos++)
   {
      new_name[npos] = pos + 0x30;
  
      /* != NULL Means it already exists */
      if ((outfp = fopen(new_name,"r")) != NULL)
         fclose(outfp);
      else
      {
#ifdef IBMPC
         if (asciiflag) outfp = fopen(new_name,"w"); /* different fopen() for */
         else           outfp = fopen(new_name,"wb");/* binary vs. ASCII */
#else
         outfp = fopen(new_name,"w");  /* Only one kind of fopen() */
#endif
         if (outfp != NULL)
         {
            fprintf(stderr, "Saving '%s' into '%s'\n", name, new_name);
            return(YES);
         }
      }
   }
   return(NO);
}
#endif