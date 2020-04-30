/*
 "getstr.c" - string getting function for multi-vue.
*/

#include <wind.h>
#include <buffs.h>
#include <sgstat.h>

static struct sgbuf opts;

/*
 'getstr()' will get a string from the user.
*/

int getstr(path,title,s,n,column,row,fg,bg)
char *title,*s;
register int path;
int n,column,row,fg,bg;
{
 int oldecho;
 int len;
 
 _gs_opt(path,&opts);
 oldecho=opts.sg_echo;
 opts.sg_echo=1;
 _ss_opt(path,&opts);
 OWSet(path,1,column,row,strlen(title)+n+3,3,fg,bg);
 CurOff(path);
 if (_ss_wset(path,WT_DBOX,0))
  CurXY(path,1,1); /* for text windows */
 CurOn(path);
 Font(path,GRP_FONT,FNT_S8X8);
 cwrite(path,title,80);
 _Flush();
 while (_gs_rdy(path)==-1)
  tsleep(6);

 len=creadln(path,s,n);
 
 opts.sg_echo=oldecho;
 _ss_opt(path,&opts);
 MVOWEnd(path);
 return(len);
}
