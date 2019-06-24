/*
 "mvmenu.c" - high level menu function for MW C

by Mike Sweet 1/17/90
*/

#include <menu.h>


int MVMenu(path,title,itemptr,fg,bg)
int path,fg,bg;
char *title;
ITEM itemptr[];
{
 int length,width,temp;
 int maxwidth;
 register int index;
 
 _gs_scsz(path,&width,&length);
 
 index=0;
 maxwidth=strlen(title);
 
 while (itemptr[index].i_name)
  if ((temp=strlen(itemptr[index++].i_name))>maxwidth)
   maxwidth=temp;
   
 return(MVMenuXY(path,title,itemptr,(width-maxwidth-2)/2,(length-index-4)/2,fg,bg));
}

