#include "cuts.h"

char header_packet[85];
extern char saved_ipacket[85];

encode(filename,ftype,ofptr,first_ipacket)
   char *filename;
   int ftype;
   FILE *ofptr;
   int *first_ipacket;
{
   extern char checksum();
   char buff[BUFLEN];
   static int line_number = 0;
   int fileend, asciiflag;
   FILE *infp;

   buff[79] = '\0';

   asciiflag = (ftype == ASCIIFILE);

   if (strcmp(filename,"-"))  /* Not stdin */
   {
#ifdef IBMPC
      if (asciiflag) infp = fopen(filename,"r");
      else           infp = fopen(filename,"rb");
#else
      infp = fopen(filename,"r");
#endif
      if (infp == NULL)
      {
         fprintf(stderr, "Error opening '%s'\n", filename);
         return;
      }
      make_ipacket(filename, ftype, first_ipacket, line_number++);
   }
   else
   {
      infp = stdin;
      make_ipacket("stdin", ftype, first_ipacket, line_number++);
   }
   fprintf(ofptr, "%s\n", header_packet);
   do
   {
       sprintf(buff, ".%04d.D", line_number++);
       fileend = line_encode(infp, buff, asciiflag);
       buff[78] = checksum(buff);
       fprintf(ofptr, "%s\n", buff);
   }
   while (fileend);

   CLOSEIN(infp);
}


/*
 * NOTE: these routines are only for use in the function
 *       line_encode.  These routines were written to help
 *       Eddie Kuns port cuts to VMS.  Apparently VMS
 *       has problems ungetc'ing characters >= $80.
 * Srt
 *
 * Later modified to allow ungetc'ing of 2 consective characters
 * for translation of local EOL marker --> CR while CUTS encoding
 * ASCII files.
 *      EWK
 */

static int _save_ch[5],
           _save_flag = FALSE;

int sgetc(fp)
   FILE *fp;
{
   if (_save_flag)
      return(_save_ch[_save_flag--]);
   else
      return(fgetc(fp));
}

int sungetc(c)
   int c;
{
   _save_ch[++_save_flag] = c;
}

/*
 * Encode a single line.
 */
line_encode(infp, line, asciiflag)
   FILE *infp;
   char line[];
   int  asciiflag;
{
   register int i;
   int nch, ch;

   for (i=7; (i<78) && ((ch=sgetc(infp)) != EOF); )
      if (SAME(ch) || (ch == ' '))
         line[i++] = ch;
      else
      {
         if (asciiflag && (ch == CR || ch == LF))
         {
            if (((nch = sgetc(infp)) != CR && nch != LF) || ch == nch)
               sungetc(nch);
            ch = CR;
         }
         line[i++] = ch / 0x20 + 0x21;
         line[i++] = ch % 0x20 + 0x30;
      }

   if (i > 78 || i == 77)
   {
      line[77] = '#';
      sungetc(ch,infp);
   }
   if (i > 76)  return(TRUE);

   line[i++] = '#';
   for (; i<78; line[i++] = '.');
   return(FALSE);
}

/*
 * Create an I packet
 */
make_ipacket(filename, type, first_ipacket, line_number)
   char *filename;
   int type, line_number, *first_ipacket;
{
   extern char *fstr(), *base(), checksum(), *trunc();
   struct tm *t, *localtime();
   long lt;
   int i;
  
   time(&lt);
   t = localtime(&lt);

   filename = trunc(base(filename), MAXNAMELEN);
   sprintf(header_packet, ".%04d.I.A.%02d%02d%02d.%s.\"%s\"", line_number,
           t->tm_year, t->tm_mon+1, t->tm_mday, fstr(type), filename);

   for (i=strlen(header_packet); i<78; header_packet[i++] = '.');

   header_packet[78] = checksum(header_packet);
   if (*first_ipacket)
   {
      strncpy(saved_ipacket, header_packet, 79);
      *first_ipacket = FALSE;
   }
}

ending_ipacket(ofptr)
   FILE *ofptr;
{
   saved_ipacket[79] = '\0';
   fprintf(ofptr, "%s\n", saved_ipacket);
}