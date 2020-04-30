/*
 "Tandy.c" - tandy menu item handler for MW C
*/


 static struct {
  char *name;
  char xsize,ysize;
  } items[]={{"gcalc",30,12},
            {"gclock",30,12},
            {"gcal",40,24},
            {"control",40,21},
            {"gprint",40,21},
            {"gport",40,21},
            {"help",40,12},
            {"shell",40,12}}; /* program names... */




/*
 int TandyMN(path,inum,fg,bg)
 int path;
 int inum;
 int fg,bg;
 
 'TandyMN()' will run the appropriate program from the default Tandy menu,
 where 'inum' contains the item # that was selected from the menu.  The
 function sets up an overlay window on the first line of the current
 window with the foreground and background colors set to fg and bg 
 respectively.  The forked program will have 'path' as its standard in, out,
 and error.  'TandyMN()' returns 0 on success and -1 on error.  If an error
 occurred then the global variable 'errno' contains the error code returned.
*/

int TandyMN(path,inum,fg,bg)
register int path;
int inum,fg,bg;
{
 int error; /* error code */
 int oldpath; /* saved stdin */

 if (path<3)
  {
   OWSet(path,1,0,0,items[inum-1].xsize,items[inum-1].ysize,fg,bg);
   os9fork(items[inum-1].name,1,"\n",0,0); /* run the program */
   wait(&error);
   OWEnd(path);
  }
 else
  { /* not current window */
   oldpath=dup(1); /* save old stdout */
   close(0);
   close(1);
   close(2);
   dup(path);
   dup(path);
   dup(path);
   os9fork(items[inum-1].name,1,"\n",0,0); /* run it- assume window all set... */
   wait(&error);
   close(0);
   close(1);
   close(2);
   dup(oldpath);
   dup(oldpath);
   dup(oldpath);
   close(oldpath);
  };
  
 if (error==0)
  return(0);
 errno=error;
 return(-1);
}

