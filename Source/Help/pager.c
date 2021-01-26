/*
 *
 * Handle screen output.
 *      This program is in the Public Domain.
 *
 * All screen output should go through "outline" or "outprompt".
 *
 * "outline" counts lines of output, and stops at the bottom of
 *   screen, etc.  It handles all of the paging work.
 *   All output should go through "outline".
 *
 * "outprompt" outputs a line with no trailing newline, and resets
 *   the pager for the next full screen.
 *
 * "listfile" lists the specified filename to the screen.  It can
 *   be modified to behave differently depending on the extension,
 *   if desired.
 */

#include "help.h"
#include <stdio.h>

int width,height; /* width/height of screen */
int linesleft; /* Number of lines to fulfill request */
int pagequit;  /* TRUE means don't display lines until we get to a prompt */
char pagebuff[1000]; /* Buffer for line breaking */

/*
 * Initialize variables used by pager
 */
pagerinit()
{
  _gs_scsz(1,&width,&height);
  if( (width<28) || (height<3) )
     printf("Screen size must be at least 28 x 3.\n");
  linesleft = height-1;
  width--;     /* Use one less than width as our reference */
  pagequit=0;
}

/*
 * Process line before handing it to the pager code
 *
 * Specifically, this expands TABs, breaks lines at LFs, and, if any line
 *   is too long after this, will break the line to fit the screen so that
 *   the pager can keep an accurate count of screen lines filled.
 */
#define LF '\x0A'
#define TAB '\x09'
outline(str)
char *str;
{
   char c;    /* Current character being inspected */
   int space=0;      /* Location of last space in output */
   char *strspace; /* Location of last space in input */
   int col=0;  /* Current output column */
   int rtn=0;  /* Value to return */
   int i;      /* Used to count when expanding tabs */

   if(!*str) return(outpage(str)); /* special handling for blank lines */

   while(*str && !rtn) {
      col=0;space=0;
      while(col<width && (c=*str)!=LF && c) { /* copy one line to pagebuff */
         if(c==TAB) {
            for(i=(col/4+1)*4-col;i>0;i--)  /* expand tabs */
               pagebuff[col++]=' ';
         } else {
            if (c==' ') { strspace=str;space = col;} /* remember last space */
            pagebuff[col++]=c;
         }
         str++;
      }
      pagebuff[col]='\0';  /* Mark end of line */
      if(col>=width && space) {   /* If line too long, break at last space */
         pagebuff[space]='\0';    /* Mark end */
         str=strspace;            /* Restore read position */
      }
      rtn=outpage(pagebuff);
      if(col>=width)              /* If we broke the line.. */
         while((c=*str)==' ' || c==TAB) str++;   /* ..skip whitespace */
      if(c==LF) str++;  /* If we broke at a LF, skip it */
   }
   return rtn;
}

/*
 * sleep and wait for input ready
 */
getinput()
{
    _ss_ssig(STDIN_FILENO, 'K');    /* set up key check */
    _ss_msig(STDIN_FILENO, 'M');    /* and mouse check */
    pause();
    _ss_rel(STDIN_FILENO);
}

/*
 * put line on screen, handle end-of-screen paging duties
 */
outpage(str)
char *str;
/* int interrupted; /* Set to TRUE by sighandler if we should return */
{
  char ch;
  int rtn=pagequit;  /* return value: note 'quit' state is persistent */

  if(interrupted) rtn=pagequit=1;  /* Break quits immediately */

  if (linesleft==0 && !rtn) {  /* If we've hit the bottom of the screen, */
     fflush(stdout);
     do {
        write(1,"--More--",8); /* then put up a prompt */

        getinput();
        if (keyed) read(0,&ch,1);         /* Get a character */
        else if (clicked) ch = ' ';
        else if (interrupted) ch = 'q';

        write(1,"\r         \r",11); /* erase the prompt */

        keyed = FALSE;
        clicked = FALSE;

        switch(ch) {
           case ' ': linesleft=height-2;break; /* ahead one screen */
           case '\r':linesleft=1;break; /* ahead one line */
           case 'q':
           case 'Q': rtn=1; pagequit=1; break;
           case 'h':
           case 'H':
           case '?': pagerhelp();  /* Fall through to default case */
           default:  ch='\0';  /* Otherwise, treat as null */
        }
     } while (ch=='\0');
  }
  if(!rtn) { 
     puts(str);  /* Output with trailing newline */
     linesleft--;
  }
  return rtn;
}

/*
 * output prompt message, reset paging variables
 */
outprompt(str)
char *str;
{
  fflush(stdout);
  write(1,str,strlen(str)); /* Output without trailing newline */
  linesleft = height-1;
  pagequit=0;
}

/*
 * Display help for pager.
 * NOTE: This MUST NOT go through outline.  If it did, we could end
 * with a nasty recursive situation.
 */
pagerhelp()
{
  puts("");
  puts("At any --More-- prompt, you may press:");
  puts(" Space to see next screen");
  puts(" ENTER to see next line");
  puts(" q     to return to (sub)topic prompt");
}

/*
 * List a file to the screen.
 * Check against recognized extensions, if none match, use internal lister
 */
listfile(name)
char *name;
{
    int_list(name);
    outline("");  /* Put blank line afterward */
}

/*
 * List a file through the built-in pager
 */
int_list(name)
char *name;  /* Name of file to list */
{
   FILE *infile;

   if(infile=fopen(name,"r")) {
      while(myfgets(scratch,BUF_SIZE,infile)  /* Get a line */
              && !outline(scratch) ) ;         /* Output it */
      fclose(infile);
   } else {
      sprintf(scratch,"Couldn't open file \"%s\".  Error #%02d.",name,errno);
      outline(scratch);
   }
}

myfgets(buff,n,fp)  /* Like fgets, except does not return newline */
char *buff;
int n;
FILE *fp;
{
    int rtn;
    char *p;

    rtn = fgets(buff,n,fp); /* do fgets */
    if (p=index(buff,'\n')) /* If there's a newline char */
       *p='\0';   /* remove it */
    return rtn;
}
