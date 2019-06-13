char scid[]= 
   "$Header: cuts, v1.6  89/08/08 00:23:17  Copyright Simmule Turner $";
/*
 * $Source:  /pub/src/cuts.c.Z                        $
 * $Author:  Simmule R. Turner  (simmy@nu.cs.fsu.edu) $
 * $Header:  cuts.c,v 1.6 89/08/08  00:23:17  SrT     $
 * $Log:     cuts.c,v                                 $
 *
 * Revision 1.0 88/05/03  21:20:00  SrT
 *   Initial coding.
 *
 * Revision 1.1 88/07/15  17:45:00  SrT
 *   Added in VMS support via Eddie Kuns (EKUNS@PISCES.RUTGERS.EDU)
 *   PD getopt code added to distribution, for VMS.
 *
 * Revision 1.2 88/08/09  15:35:10  SrT
 *   Added Sun/BSD support(the c-library is braindead) via 
 *   Claude Morin (klode%deepthot.uwo.ca)
 *
 * Revision 1.3 88/08/13  13:31:28  SrT
 *   Source cleanup for public posting.
 *
 * Revision 1.4 88/11/23  16:37:15  TBK
 *   Removed SUN symbol by generalizing code.
 *   Generalized out some VMS switches.
 *   Split source, cleaned up for public posting.
 *   Added CR -> native EOL conversion on decode.
 *   Added -v, -h switches.
 *
 * Revision 1.5 89/03/02  00:23:17  EWK
 *   Added native EOL -> CR conversion on encode.
 *   Significant re-write of encoding/decoding logic.
 *   Allow decoding of merged CUTS files.
 *   Squashed a few hidden minor bugs dating to 1.0!
 *   Finally got VMS file attributes correct on file creation.
 *   Re-wrote use of getopt() to allow random ordering of arguments.
 *   Added filename truncation.
 *   More specific error messages when error occurs in decoding.
 *   Removed USEGETOPT symbol.  - TBK 89/03/06
 *
 * Revision 1.6 89/08/08  00:23:17
 *   Added -q option, 'quiet' variable.
 *   Added code to ignore leading garbage on decode.
 *     (Now looks for first period before attempting decode).
 *   Added IBM PC/Quick C support from Don Ingli.
 *   Tested under Turbo C by Ernest Campbell.
 *   Generalized RSDOS and OS9 file types to
 *     MAC = machine-specific binary
 */

#include "cuts.h"

int quiet;   /* When non-zero, quiet mode is on, so we don't print as much */

typedef struct
{
   int  file_type;
   char *name;
} namestype;

main(argc,argv)
   int argc;
   char *argv[];
{
   int c, i, decoder=0, encoder=0, outputfile=0, nfiles=0;
   int first_ipacket = TRUE, flagind;
   char errmsg[BUFLEN], *output_filename;
   namestype names[MAXNAMES];
   extern int optind, opterr;
   extern char *optarg;
   FILE *ofptr;  

   if (argc == 1)
      usage("");

   quiet = 0;   /* Quiet mode initially off */

   ofptr = stdout;

   while (optind < argc)
   {
      c = getopt(argc, argv, "dehvo:9:a:b:r:m:");
      switch (isupper(c) ? tolower(c) : c)
      {
         case 'q':
            quiet++;
            break;
         case 'v':
            version();
            break;
         case 'd':
            decoder++;
            break;
         case 'e':
            encoder++;
            break;
         case 'o':
            if (outputfile)
               usage("Only one output file allowed!");
            outputfile++;
            output_filename = optarg;
            break;
         case '9':  /* -9 and -r are outdated and should be removed */
         case 'r':
         case 'm':
            names[nfiles].file_type = MACHINEFILE;
            names[nfiles++].name    = optarg;
            break;
         case 'a':
            names[nfiles].file_type = ASCIIFILE;
            names[nfiles++].name    = optarg;
            break;
         case 'b':
            names[nfiles].file_type = BINARYFILE;
            names[nfiles++].name    = optarg;
            break;
         case 'h':
            longusage();
            break;
         case '?':
            usage("");
            break;
         case EOF:
            names[nfiles].file_type =
               (nfiles>0) ? names[nfiles-1].file_type : BINARYFILE;
            names[nfiles++].name    = argv[optind];
            optind = optind + 1;/*Note that this is somewhat nonstandard*/
      }                         /*use of getopt() here...*/
   }

   if (decoder*encoder)
      usage("Illegal combination of options");

   if (outputfile)
      if (strcmp(output_filename, "-"))   /* Not stdout */
      {
sprintf(errmsg,"output filename = '%s'\n",output_filename);

#ifndef VMS    /* VMS handles filename collisions */
         if ((ofptr=fopen(output_filename, "r")) != NULL)
         {
            sprintf(errmsg,"Output file '%s' already exists",
                                            output_filename);
            usage(errmsg);
         }
         if ((ofptr=fopen(output_filename, "w")) == NULL)
#else
         if ((ofptr=fopen(output_filename, "w", "rfm=var", "rat=cr"))
                                                                == NULL)
#endif
         {
           sprintf(errmsg,"Could not create output file '%s'",
                                                      output_filename);
           usage(errmsg);
         }
      }

   if (!encoder)
      for (i=0; i<nfiles; i++)
         decode(names[i].name, outputfile, ofptr);
   else
   {
      for (i=0; i<nfiles; i++)
         encode(names[i].name, names[i].file_type, ofptr, &first_ipacket);
      ending_ipacket(ofptr);
   }
}

/*
 * Computes checksum for single line
 */

char checksum(line)
   char line[BUFLEN];
{
   int i,sum=0;

   for (i=0; i<78; i++)
      sum += line[i];

   sum = sum % 32 + 48;
   return ((char)sum);
}

/*
 * Ask User if they want to continue after an error
 */
cont_yn()
{
   char str[32];

   fprintf(stderr,"    Continue: with this file y/n?  ");
   fflush(stderr);

   gets(str);
   if (str[0] == 'N' || str[0] == 'n')
      return(NO);
  
   return(YES);
}

/*
 * Print Version message
 */
version()
{
   fprintf(stderr,"%s\n\n",scid);
}

/*
 * Give long usage display
 */
longusage()
{
 fprintf(stderr,"\n          CUTS Usage Summary\n\n");
 fprintf(stderr,"To decode one or more CUTS-encoded files:\n");
 fprintf(stderr,"     cuts -d <file> . . .\n\n");
 fprintf(stderr,"To encode one or more files into a CUTS package:\n");
 fprintf(stderr,"     cuts -e [-o <output_file>]  [ [-<type>] <file> ]\n");
 fprintf(stderr,"\n   where <output_file> defaults to standard output,\n");
 fprintf(stderr,"   <type> is one of:   -a  ASCII text file\n");
 fprintf(stderr,"                       -b  non-specific Binary file");
 fprintf(stderr,"(e.g. picture files, archives)\n");
 fprintf(stderr,"                       -m  machine-specific binary\n\n");
 fprintf(stderr,"To print out information about this version of CUTS:");
 fprintf(stderr,"  cuts -v\n");
 fprintf(stderr,"To get this message:  cuts -h\n\n");
 exit();
}

/*
 * Give short usage display, and error message
 */
usage(errmsg)
   char *errmsg;
{
   if (errmsg[0] != '\0')
   fprintf(stderr, "\n%s\n",errmsg);
   fprintf(stderr, "\nUsage: cuts <-d> [-o name] <file>...\n");
   fprintf(stderr, "       cuts <-e> [-o name] [[<->abm] <file>] ...\n");
   fprintf(stderr, "'cuts -h' for more information\n");
   exit();
}

/*
 * Return filetype string
 */
char *fstr(n)
   int n;
{
   switch(n)
   {
      case MACHINEFILE:
         return("MAC");
      case ASCIIFILE:
         return("ASC");
      default:
         return("BIN");
   }
}

/*
 * Returns base filename, stripped of directory prefixes
 */
char *base(s)
   char *s;
{
#ifdef VMS  /* VMS has a different filename structure */
   char *p=strrchr(s,']');
   if (p == NULL)  p = strrchr(s,':');
#else
   char *p=strrchr(s,'/');
#endif
   return ( (p) ? p+1 : s );
}

/*
 * Return a legal filename (which may depend on the system)
 */
char *trunc(name, maxlen)
   char *name;
   int  maxlen;
{
   int  test=TRUE;
   char newname[60];

   do
   {
      if (strlen(name) > maxlen)
      {
         fprintf(stderr, "Filename '%s' is too long.\n", name);
         name[maxlen] = '\0';
      }
#ifdef VMS
      else if (!valid_VMS(&name))
         ;
#endif
      else
         test = FALSE;

      if (test)
      {
         fprintf(stderr, "Enter new filename <%s>", name);
         fflush(stderr);
         gets(newname);
         if (newname[0] != '\0')
            strcpy(name, newname);
      }
   }
   while (test);

   return(name);
}

#ifdef VMS
/*
 *   VMS only allows one '.' character in a filename.
 */
valid_VMS(name)
   char **name;
{
   char *per1, *per2;
   int  flag=TRUE;

   do
   {
      per2 = NULL;
      per1 = strchr(*name, '.');
      if (per1 != NULL)
         per2 = strchr(per1+1, '.');

      if (per2 != NULL)
      {
         if (flag)
            fprintf(stderr, "Too many '.'s in filename '%s'.\n", *name);
         flag = FALSE;
         *per1 = '_';
      }
   }
   while (per2 != NULL);

   return (flag);
}
#endif