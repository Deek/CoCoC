/*
"mvfname.c" - an on-screen file picking function
by Mike Sweet 12/7/89,2/2/90
*/

#include <lowio.h>
#include <sgstat.h>
#include <buffs.h>
#include <wind.h>
#include <mouse.h>


long _gs_pos();

static char _FName[30];
static int files[256]; /* offsets into directory- convert to long! */
static char dbuf[32];
static MSRET mp;
static struct sgbuf oldopts,newopts;

/*
 'MVFName()' will set up an overlay in the center of the window you specify
 and allow the user to pick a file.  If the user clicks on the close box,
 a NULL pointer is returned.  Otherwise, a pointer to the filename is returned.
 Note that the mouse must be set up prior to a call to this function (using
 the _ss_mous() function- the SetGC() is not required...)
*/

char *MVFName(path,title,column,row,fg,bg)
register int path;
int fg,bg,column,row;
char *title;
{
 int dpath,temp;
 int line,startnum,refresh,index;
 int numfiles;
 int width,length;
 char ch;
 char scrlsize,ypos;
 char xoff,yoff;
 char bflag; /* button flag */

 _gs_opt(path,&oldopts);
 _gs_opt(path,&newopts);
 newopts.sg_echo=newopts.sg_kbich=newopts.sg_kbach=0;
 _ss_opt(path,&newopts);


 OWSet(path,1,column,row,22,10,fg,bg); /* create an overlay window */
 
 write(path,"\x1b\x3a\xc8\x01\x05 \x1b\x35\x00\x1bH\x00\xaf\x00\x4f\x1b@\x00\xa8\x00\x00\x1bD\x00\xa8\x00\x4f",27);
 OWSet(path,0,column,row,22,11,bg,fg);
 write(path,"\x03\x1b\x3a\xc8\x03\x06\xc7\x02\x35!\xc4\x02\x35)\xc3\x02\# \x1b\x3a\xc8\x01",22);
 cwrite(path,title,19);
 OWEnd(path);
 column++;
 row++;
 bflag=0;

 SetGC(path,GRP_PTR,PTR_ARR);
 _Flush();
 
 while (TRUE)
  {
   if ((dpath=open(".",READ | DIR))==-1)
    {
     OWEnd(path);
     _ss_opt(path,&oldopts);
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
 
   refresh=TRUE;
   scrlsize=53/(numfiles/8+1);
   if (scrlsize<4)
    scrlsize=4;
   
   startnum=-1;
   line=0;

   while(line<256)
    {
     if (refresh)
      {
       OWSet(path,0,column,row,20,8,fg,bg);
       Clear(path);
       OWEnd(path);
       
       for (index=startnum;index<startnum+8;index++)
        if (index<=numfiles)
         {
          if (index==-1)
           strcpy(_FName,"[new file]");
          else
           {
            lseek(dpath,(long)files[index],0);
            read(dpath,dbuf,32);
            strhcpy(_FName,dbuf);
           };
          CurXY(path,1,index-startnum+1);
          if (index==(startnum+line))
           RevOn(path);
          else
           RevOff(path);
          cwrite(path,_FName,19);
         };
        refresh=0;
       };
       
      FColor(path,bg);
      SetDPtr(path,170,17);
      Bar(path,173,71);
      temp=17+(startnum+line)*(53-scrlsize)/numfiles;
      if (temp<17)
       temp=17;
      SetDPtr(path,170,temp);
      FColor(path,fg);
      RBar(path,3,scrlsize);
       
      if ((line+startnum)==-1)
       strcpy(_FName,"[new file]");
      else
       {
        lseek(dpath,(long)files[startnum+line],0);
        read(dpath,dbuf,32);
        strhcpy(_FName,dbuf);
       };
      CurXY(path,1,line+1);
      RevOn(path);
      cwrite(path,_FName,19);
      RevOff(path);
      _Flush();

      do
       {
        _gs_mous(path,&mp);
        if ((_gs_rdy(path)==-1) && !mp.pt_cbsa)
         bflag=0;
       }
      while ((_gs_rdy(path)==-1) && !(mp.pt_cbsa ^ bflag));
      
      CurXY(path,1,line+1);
      cwrite(path,_FName,19);
      _Flush();
      
      if (_gs_rdy(path)==-1)
       ch=0;
      else
       read(path,&ch,1);
      mp.pt_wrx/=8;
      ypos=mp.pt_wry/8;
      
      if (!mp.pt_valid && (ch==0))
       continue;
      if (ch)
       mp.pt_wrx=ypos=-1;
     
      if ((((mp.pt_wrx==21) && (ypos==9)) || (ch==10)) && ((line+startnum)<numfiles)) /* down arrow ? */
       {
        line++;
        if (line>7)
         {
          OWSet(path,0,column,row,20,8,fg,bg);
          DelLine(path);
          OWEnd(path);
          line=7;
          startnum++;
         };
       }
      else if ((((mp.pt_wrx==21) && (ypos==1)) || (ch==12)) && ((line+startnum) > -1) ) /* up arrow ? */
       {
        line--;
        if (line<0)
         {
          OWSet(path,0,column,row,20,8,fg,bg);
          InsLin(path);
          OWEnd(path);
          startnum--;
          line=0;
         };
       }
      else if (((ypos>0) && (ypos<9) && (mp.pt_wrx<21)) || (ch==13)) /* enter ? */
      {
       if ((ypos==(line+1)) || (ch==13))
        {
         close(dpath);
         if ((startnum+line)==-1)
          getstr(path,"Filename? ",_FName,16,column-2,row+4,fg,bg);
         if (chdir(_FName)==-1)
          {
           OWEnd(path);
           _ss_opt(path,&oldopts);
           return(_FName);
          };
         line=256;
        }
       else
        if ((startnum+ypos-1)<=numfiles)
         {
          line=ypos-1;
          bflag=1;
         }; 
      }
     else if (((mp.pt_wrx==1) && (ypos==0)) || ch==5) /* break ? */
      {
       OWEnd(path);
       _ss_opt(path,&oldopts);
       close(dpath);
       return(0);
      }
     else if ((mp.pt_wrx==21) && (ypos>1) && (ypos<9))
      {
       temp=startnum;
       startnum=(mp.pt_wry-16)*numfiles/56;
       if (temp!=startnum)
        refresh=TRUE;
      };
    };
  };
}
