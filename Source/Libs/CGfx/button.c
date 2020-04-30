/*
 "button.c" - some quick functions to put buttons on the screen.
 by Mike Sweet 3/31/90
*/

#include <buffs.h>

/*
 'BUp()' will place a button (all the way up) on the screen at the
 specified coordinates with the string 's' enclosed in it.  Make sure
 you turn off scaling first!
*/

BUp(path,column,row,s,fg,bg)
register int path;
int column,row,fg,bg;
char *s;
{
 int x,y;
 int len;

 x=column*8-1;
 y=row*8-2;
 FColor(path,bg);
 SetDPtr(path,x-1,y-1);
 len=strlen(s);
 RBar(path,len*8+4,15);
 FColor(path,fg);
 BColor(path,bg);
 
 CurXY(path,column,row);
 cwrite(path,s,80);

 SetDPtr(path,x,y);
 RBox(path,strlen(s)*8+1,10);
 SetDPtr(path,x+strlen(s)*8+1,y);
 RLineM(path,2,2);
 RLineM(path,0,10);
 RLine(path,-2,-2);
 LineM(path,x+3,y+12);
 RLineM(path,-2,-2);
 _Flush();
}



/*
 'BDown()' - basically the same as BUp(), but the button is pushed down...
*/

BDown(path,column,row,s)
register int path;
int column,row;
char *s;
{
 int x,y;
 int pid;
 
 x=column*8-2;
 y=row*8-3;
 pid=getpid();
 
 GetBlk(path,pid,255,x,y,strlen(s)*8+3,12);
 PutBlk(path,pid,255,x+1,y+1);
 KilBuf(path,pid,255);
 _Flush();
}
