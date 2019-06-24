/*
 "mvmenuxy.c" - high level menu function for MW C

by Mike Sweet 1/17/90
*/

#include <sgstat.h>
#include <mouse.h>
#include <wind.h>
#include <menu.h>
#include <buffs.h>

static int levels=0; /* # of levels of overlays... */
static MSRET mp;
static struct sgbuf oldopts,newopts;

/*
int MVMenuXY(path,title,itemptr,column,row,fg,bg)
int path,fg,bg;
char *title;
ITEM itemptr[];

    'MVMenuXY()' will display the menu item array pointed to by 'itemptr' and
allow the user to select from the enabled ones.  A shadowed overlay window
is created on 'path' with a foreground color of 'fg' and a background color
of 'bg'.  The user can use the mouse to highlight the item
he/she wants.  If the item is disabled, then the item will be skipped, other-
wise the function pointed to by 'func' will be executed.  'title' is the title
that is displayed at the top of the overlay window. 
Note that because of the limitations of screen size, do not have more than 19
items  for any given menu (or 20 if you have patched GRFDRV for 25 lines.)
The array of items should end with an element with the name field NULL.
'MVMenuXY()' also returns the item # selected or 0 (if no selection was made.)
*/
 
int MVMenuXY(path,title,itemptr,column,row,fg,bg)
register int path;
int fg,bg,column,row;
char *title;
ITEM itemptr[];
{
 int index,numitems;
 int width,newindex,offset;
 char ch,onwindow;

 if (levels==0)
  { 
   _gs_opt(path,&oldopts);
   _gs_opt(path,&newopts);
   newopts.sg_echo=newopts.sg_kbach=newopts.sg_kbich=newopts.sg_psch=0;
   _ss_opt(path,&newopts);
  };

 numitems=0;

 width=strlen(title);
 
 while (itemptr[numitems].i_name)
  {
   if (width < (index=strlen(itemptr[numitems].i_name)))
    width=index;
   numitems++;
  };
  
 if (title)
  {
   offset=2;
   OWSet(path,1,column,row,width+3,numitems+4,fg,bg);
   CurOff(path);
   _ss_wset(path,WT_SBOX,0);
   Font(path,GRP_FONT,FNT_S8X8);
   CurXY(path,(width-strlen(title))/2,0);
   cwrite(path,title,80);
  }
 else
  {
   offset=0;
   OWSet(path,1,column,row,width+3,numitems+2,fg,bg);
   CurOff(path);
   _ss_wset(path,WT_SBOX,0);
   Font(path,GRP_FONT,FNT_S8X8);
  };

 levels++; 

 for (index=0;index<numitems;index++) /* now print the choices (items) */
  {
   CurXY(path,0,index+offset);
   BoldSw(path,itemptr[index].i_enabled);
   cwrite(path,itemptr[index].i_name,80);
  };

 for (index=0;!itemptr[index].i_enabled;index++);

 BoldSw(path,1);
 CurXY(path,0,index+offset);
 RevOn(path);
 cwrite(path,itemptr[index].i_name,80);
 _Flush();

 onwindow=0;
   
 while (1) /* OK, let's just stick around 'till we do something */
  {
   _gs_mous(path,&mp);

   if (_gs_rdy(path)!=-1)
    read(path,&ch,1);
   else
    ch=0;

   if ((mp.pt_stat==WR_OFWIN) && (ch==0))
    {
     if (!onwindow)
      continue;
     else
      {
       remove(path); /* remove all overlays */
       _ss_opt(path,&oldopts);
       return(0);
      };
    };

   if (onwindow=(mp.pt_stat != WR_OFWIN))
    newindex=mp.pt_wry/8-offset;
   else
    newindex=index;

   if (newindex<0 || !itemptr[newindex].i_enabled)
    newindex=index;

   if (ch==10)
    {
     newindex=index;
     do
      {
       newindex++;
       if (newindex>=numitems)
        newindex=0;
      }
     while (!itemptr[newindex].i_enabled);
    }
   else if (ch==12)
    {
     newindex=index;
     do
      {
       newindex--;
       if (newindex<0)
        newindex=numitems-1;
      }
     while (!itemptr[newindex].i_enabled);
    };

   if (index!=newindex)
    {
     CurXY(path,0,index+offset);
     RevOff(path);
     cwrite(path,itemptr[index].i_name,80);
     index=newindex;
     CurXY(path,0,index+offset);
     RevOn(path);
     cwrite(path,itemptr[index].i_name,80);
     _Flush();
    };
   if (mp.pt_cbsa || (ch==13))
    {
     if (itemptr[index].i_enabled==MN_SUBMN)
      {
       index+=1+16*MVMenuXY(path,0,itemptr[index].i_func,column+width-1,row+index+offset+1,fg,bg);
       return (index);
      };
     remove(path); /* close the overlay */
     _ss_opt(path,&oldopts);
     if (itemptr[index].i_func)
      (*itemptr[index].i_func)(); /* call the selected function */
     return(index+1);
    };
   if (ch==5)
    {
     remove(path);
     _ss_opt(path,&oldopts);
     return(0);
    };
   tsleep(1);
  };
}


remove(path)
int path;
{
 while (levels>0)
  {
   MVOWEnd(path);
   levels--;
  };
}
