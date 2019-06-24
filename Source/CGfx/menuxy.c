/*
 "menuxy.c" - high level menu function for MW C

by Mike Sweet 11/27/89,7/19/90
*/

#include <lowio.h>
#include <sgstat.h>
#include <menu.h>

static int levels=0;
static struct sgbuf oldopts,newopts;

/*
int MenuXY(path,title,itemptr,column,row,fg,bg)
int path,fg,bg;
char *title;
ITEM itemptr[];

    'Menu()' will display the menu item array pointed to by 'itemptr' and
allow the user to select from the enabled ones.  A shadowed overlay window
is created on 'path' with a foreground color of 'fg' and a background color
of 'bg'.  The user can use the up and down arrow keys to highlight the item
he/she wants.  If the item is disabled, then the item will be skipped, other-
wise the function pointed to by 'func' will be executed.  'title' is the title
that is displayed at the top of the overlay window.  If the user presses the 
BREAK key during the selection proceess, the function will return after
closing  the overlay without calling any of the item functions.
Note that because of the limitations of screen size, do not have more than 19
items  for any given menu (or 20 if you have patched GRFDRV for 25 lines.)
The array of items should end with an element with the name field NULL.
'Menu' also returns the item # selected or 0 (if BREAK was pressed.)
*/
 
int MenuXY(path,title,itemptr,column,row,fg,bg)
register int path;
int fg,bg,column,row;
char *title;
ITEM itemptr[];
{
 int index,numitems;
 char ch; /* for keypresses */
 int width,offset;
 
 numitems=0;
 if (levels==0)
  {
   _gs_opt(path,&oldopts);
   _gs_opt(path,&newopts);
   newopts.sg_echo=newopts.sg_kbich=newopts.sg_kbach=newopts.sg_psch=0;
   _ss_opt(path,&newopts);
  };

 width=strlen(title);
 
 while (itemptr[numitems].i_name)
  {
   if (width<(index=strlen(itemptr[numitems].i_name)))
    width=index;
   numitems++;
  };
 width+=2;
 if (title)
  {
   offset=3;
   OWSet(path,1,column,row,width,numitems+4,fg,bg);
   CurXY(path,(width-strlen(title))/2,1);
   cwrite(path,title,80);
  }
 else
  {
   offset=1;
   OWSet(path,1,column,row,width,numitems+2,fg,bg);
  };

 levels++;

 CurOff(path);

 for (index=0;index<numitems;index++) /* now print the choices (items) */
  {
   CurXY(path,1,index+offset);
   cwrite(path,itemptr[index].i_name,80);
  };

 for (index=0;!itemptr[index].i_enabled;index++);
   
 while (TRUE) /* OK, let's just stick around 'till we do something */
  {
   CurXY(path,1,index+offset);
   RevOn(path);
   cwrite(path,itemptr[index].i_name,80);

   read(path,&ch,1); /* get a keypress */

   CurXY(path,1,index+offset);
   RevOff(path);
   cwrite(path,itemptr[index].i_name,80);
   _Flush();
   
   if (ch==0x0a) /* down arrow ? */
    do
     {
      index++; /* please make sure you don't disable ALL the items! */
      if (index==numitems)
       index=0;
     }
    while (itemptr[index].i_enabled==FALSE);
    
   else if (ch==0x0c) /* up arrow ? */
    do
     {
      index--;
      if (index<0)
       index+=numitems;
     }
    while (itemptr[index].i_enabled==FALSE);
 
   else if (ch==0x0d)
    {
     if (itemptr[index].i_enabled==MN_SUBMN)
      {
       index+=1+16*MenuXY(path,0,itemptr[index].i_func,column+width-1,row+offset,bg,fg);
       return (index);
      };    
     remove(path); /* close the overlay */
     _ss_opt(path,&oldopts);
     if (itemptr[index].i_func)
      (*itemptr[index].i_func)(); /* call the selected function */
     return(index+1);
    }
   else if (ch==0x05) /* BREAK pressed ? */
    {
     remove(path);
     _ss_opt(path,&oldopts);
     return(0);
    }
   else
    Bell(path); /* beep at unknowing user.... */
  };
}


remove(path)
int path;
{
 while (levels>0)
  {
   levels--;
   OWEnd(path);
  };
}

