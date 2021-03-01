/*	os9tovax.c -- 	
**	    This program takes a OS-9 format text file and writes it out
**      as a VAX/VMS text file.  The input and output files are command line
**      arguments.  Thanks to VAX/VMS, if the output file name is not
**      specified, the output goes to a new verion of the input file.
**
**	Modifications:
**	  11.July.90	G.Walker	Original version from "vaxtoos9".
*/

#include <stdio>
#include <ctype.h>

#define BUFF_SIZE    256

char buffr[BUFF_SIZE];

main( argc, argv)
    int argc; char *argv[];
{
    FILE *inpfile, *outfile;
    int ch;
    char *outfilename, *chptr;

    if( argc != 2 && argc != 3) {
	printf("Usage: vaxtoos9 <input_file> [<output_file>]\n");
	exit(1);
	}
    if( (inpfile = fopen( argv[1], "r")) == NULL) {
	printf("vaxtoos9: Cannot open input file %s.\n",argv[1]);
	exit(1);
	}
    if( argc == 2) outfilename = argv[1];
    else outfilename = argv[2];
    if( (outfile = fopen( outfilename, "w")) == NULL) {
	printf("vaxtoos9: Cannot open output file %s.\n",outfilename);
	exit(1);
	}

    chptr = buffr;
    while( (ch = fgetc( inpfile)) != EOF )
    {
	if( ch == '\r' )
	{
	    *chptr++ = '\n';
	    *chptr = '\0';
	    fputs(buffr, outfile);
	    chptr = buffr;
	}
	else  *chptr++ = ch;
    }
}
