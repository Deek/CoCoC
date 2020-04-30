/*
"fname.c" - an on-screen file picking function
by Mike Sweet 8/7/89
*/

#include <lowio.h>
#include <sgstat.h>


long _gs_pos();


static char _FName[30];
static int files[256];
static struct sgbuf oldopts,newopts;
static char dbuf[32];


char *FName(path,title,fg,bg)
register int path;
int fg,bg;
char *title;
{
 char ch;
 int dpath;
 int line,index;
 int numfiles;
 char *s;

 Shadow(path,32,14,fg,bg); /* create an overlay window */
 CurOff(path); /* turn the cursor off */
 CurXY(path,(32-strlen(title))/2,1); /* now print the title at the top */
 write(path,title,strlen(title));
 CurXY(path,4,12); /* and the instructions at the bottom */
 write(path,"UP/DOWN/ENTER to select",23);
  
 CWArea(path,1,3,30,8); /* restrict ourselves to the inside now */

 _gs_opt(path,&oldopts); /* now turn off echo, abort, and interrupt */
 _gs_opt(path,&newopts); /* now turn off echo, abort, and interrupt */
 newopts.sg_echo=
 newopts.sg_kbich=
 newopts.sg_kbach=
 newopts.sg_psch=0;

 _ss_opt(path,&newopts);

 while (TRUE)
  {
  
   if ((dpath=open(".",READ | DIR))==-1)
    {
     OWEnd(path);
     return(0); /* return nothing if we can't open the current dir... */
    };
  
 
   index=0;

   while (index<256)
    {
     if (read(dpath,dbuf,32)<32)
      {
       numfiles=index-1;
       index=256;
      }
     else
      if (dbuf[0]!=0)
       {
        files[index]=_gs_pos(dpath)-32l;
        index++;
       };
    };
 
   Clear(path);
 
   for (line=1;line<8;line++)
    if (line<=(numfiles+1))
     {
      lseek(dpath,(long)files[line-1],0);
      read(dpath,dbuf,32);
      CurXY(path,0,line);
      strhcpy(_FName,dbuf);
      write(path,_FName,strlen(_FName));
     };
  
   line=0;
   index=-1;
 
 
   while (index<256)
    {
     RevOn(path);
     CurXY(path,0,line);
     if (index<0)
      s="[new file]";
     else
      {
       lseek(dpath,(long)files[index],0);
       read(dpath,dbuf,32);
       strhcpy(_FName,dbuf);
       s=_FName;
      };
     write(path,s,strlen(s));
     read(path,&ch,1);
     RevOff(path);
     CurXY(path,0,line);
     write(path,s,strlen(s));
     _Flush();
   
     if (ch==0x0a && index<numfiles) /* down arrow ? */
      {
       index++;
       line++;
       if (line>7)
        {
         CurHome(path);
         DelLine(path);
         line=7;
        };
      }
     else if (ch==0x0c && index>-1) /* up arrow ? */
      {
       index--;
       line--;
       if (line<0)
        {
         CurHome(path);
         InsLin(path);
         line=0;
        };
      }
     else if (ch==0x0d) /* enter ? */
      {
       _ss_opt(path,&oldopts);
       close(dpath);
       if (index==-1)
        {
         Clear(path);
         CurXY(path,1,4);
         write(path,"Filename?",9);
         CurXY(path,1,5);
         _ss_opt(path,&oldopts);
         CurOn(path);
         if ((line=readln(path,_FName,30))<1)
          {
           UnShadow(path);
           return(0);
          };
         _FName[line-1]=0;
         _ss_opt(path,&newopts);
         CurOff(path);
        };
       if (chdir(_FName)==-1)
        {
         OWEnd(path);
         return(_FName);
        }
       else
        index=256;
      }
     else if (ch==0x05) /* break ? */
      {
       UnShadow(path);
       _ss_opt(path,&oldopts);
       close(dpath);
       return(0);
      }
     else
      Bell(path);
    };
  };
}
