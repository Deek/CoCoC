/*
 "dialog.c" - a simple dialog management function for Level II OS-9 and
 Multi-Vue.

 by Mike Sweet 6/4/90,6/21/90,8/12/90
*/


#include <buffs.h>
#include <wind.h>
#include <mouse.h>
#include <dialog.h>
#include <lowio.h>


int Dialog(path,dlgptr,column,row,width,length,fg,bg)
int path;
int column,row,width,length,fg,bg;
DIALOG *dlgptr;
{
 register DIALOG *temp;
 DIALOG *temp2,*textptr;
 char ch;
 MSRET mp;
 int textpos,textnum,xcor,ycor,event;
 register int n;

 OWSet(path,1,column,row,width,length,fg,bg);
 CurOff(path);
 SetGC(path,GRP_PTR,PTR_ARR);
 ScaleSw(path,0);
 TCharSw(path,0);
 Font(path,GRP_FONT,FNT_S8X8);
 _ss_wset(path,WT_DBOX);

 textptr=textpos=0;

 for (n=0,temp=dlgptr; temp->d_type!=D_END ;temp++,n++)
  switch (temp->d_type)
   {
    case D_TEXT  :
    case D_STRING: CurXY(path,temp->d_column,temp->d_row);
                   cwrite(path,temp->d_string,80);
                   if (temp->d_type==D_STRING)
                    {
                     SetDPtr(path,temp->d_column*8-2,temp->d_row*8-2);
                     RBox(path,strlen(temp->d_string)*8+3,11);
                     if (!textptr)
                      {
                       textptr=temp;
                       textnum=n;
                      };
                    };
                   break;
    case D_BUTTON: BUp(path,temp->d_column,temp->d_row,temp->d_string,fg,bg);
                   break;
    case D_RADIO : if (temp->d_val)
                    RBDown(path,temp->d_column,temp->d_row,fg,bg);
                   else
                    RBUp(path,temp->d_column,temp->d_row,fg,bg);
   };

 if (textptr)
  {
   CurOn(path);
   CurXY(path,textptr->d_column,textptr->d_row);
  };

 while (TRUE)
  {
   _Flush();
   ch=MouseKey(path); /* get a keypress or button click */
   event=-1;

   if (ch>0 && textptr) /* >0 means a key was pressed */
    {
     if (ch==13)
      {
       CurOff(path);
       if (textptr->d_val)
        {
         MVOWEnd(path);
         return(textptr->d_val);
        };
       textptr=0;
      }        
     else if (ch==10) /* down arrow ? */
      {
       for (n=textnum+1,temp=textptr+1;temp->d_type!=D_END;n++,temp++)
        if (temp->d_type==D_STRING)
         break;
       if (temp->d_type==D_STRING)
        event=n;
      }
     else if (ch==12) /* up arrow ? */
      {
       for (temp=textptr-1,n=textnum;n>=0;temp--,n--)
        if (temp->d_type==D_STRING)
         break;
       if (n>=0)
        event=n;
      }
     else
      {
       estr(textptr->d_string,&textpos,ch); /* call the local editing function */
       CurXY(path,textptr->d_column,textptr->d_row);
       cwrite(path,textptr->d_string,80);
       CurXY(path,textptr->d_column+textpos,textptr->d_row);
      };
    }
   else
    {
     if (ch<0)
      {
       _gs_mous(path,&mp);
       if (mp.pt_stat!=WR_CNTNT)
        break;
       xcor=mp.pt_wrx/8;
       ycor=mp.pt_wry/8;
      }
     else if (ch>0x5f)
      ch&=0x5f; /* fold to uppercase... */

     for (temp=dlgptr,n=0;temp->d_type!=D_END;temp++,n++)
      switch (temp->d_type)
       {
        case D_STRING :
        case D_BUTTON : if (((ch<0) & (ycor==temp->d_row) & (xcor>=temp->d_column) & (xcor<(temp->d_column+strlen(temp->d_string)))) | (ch==temp->d_key))
                         event=n;
                        break;
        case D_RADIO  : if (((ch<0) * (ycor==temp->d_row) & (xcor==temp->d_column)) | (ch==temp->d_key))
                         event=n;
       };
    };

   if (event<0)
    continue;

   temp=&dlgptr[event];
   
   if (temp->d_type==D_BUTTON)
    {
     BDown(path,temp->d_column,temp->d_row,temp->d_string);
     _Flush();
     tsleep(10);
     MVOWEnd(path); /* erase overlay */
     return(temp->d_val); /* return */
    }
   else if (temp->d_type==D_RADIO)
    {
     if (textptr)
      {
       CurOff(path);
       textptr=0;
      };
     if (temp->d_string)
      {
       if (!temp->d_val)
        {
         temp->d_val=1;
         for (n=0;temp->d_string[n];n++)
          if ((temp2=&dlgptr[temp->d_string[n]-1])->d_val)
           {
            temp2->d_val=0;
            RBUp(path,temp2->d_column,temp2->d_row,fg,bg);
           };
         RBDown(path,temp->d_column,temp->d_row,fg,bg);
        };
      }
     else
      {
       if (temp->d_val)
        RBUp(path,temp->d_column,temp->d_row);
       else
        RBDown(path,temp->d_column,temp->d_row);
       temp->d_val = !temp->d_val;
      };
    }
   else if (temp->d_type==D_STRING)
    {
     CurOn(path);
     textptr=temp;
     textpos=0;
     CurXY(path,temp->d_column,temp->d_row);
    }
   else if (temp->d_val) /* else it must be a key.... */
    {
     MVOWEnd(path);
     return(temp->d_val);
    };
  };
}



/*
The following is an assembly language line-editing subroutine which looks
like a function within this ROF.  Because the name lacks a colon (:) at
the end, there will never be a conflict with another function name or var.
(i.e. this is a STATIC function.... )
*/

#asm

* /local function/
* estr(s,pos,ch)
* char *s;
* int *pos,ch;
*

estr pshs u
 ldd 4,s
 pshs d
 lbsr strlen
 puls x
 leax d,x
 leax -1,x
 ldu 4,s
 ldd [6,s]
 leau d,u
 lda 9,s
 cmpa #8
 bne e0
 ldd [6,s]
 beq return
 decb
 std [6,s]
return puls u,pc

e0 cmpa #9
 bne e1
 pshs x
 cmpu ,s++
 bhs return
 ldd [6,s]
 incb
 std [6,s]
 puls u,pc

e1 cmpa #$18
 bne e2
 clra
 clrb
 std [6,s]
 puls u,pc

e2 cmpa #$19
 bne e3
 tfr x,d
 subd 4,s
 std [6,s]
 puls u,pc

e3 cmpa #$10
 bne e4
 pshs x
eloop0 cmpu ,s
 bhs enext0
 lda 1,u
 sta ,u+
 bra eloop0
enext0 puls u
 lda #$20
 sta ,x
 puls u,pc

e4 cmpa #$11
 bne e5
 pshs u
 leau ,x
eloop1 cmpu ,s
 bls enext1
 lda ,-u
 sta 1,u
 bra eloop1
enext1 puls u
 lda #$20
 sta ,u
 puls u,pc

e5 cmpa #$20
 blo return
 cmpa #$7f
 bhi return
 sta ,u
 pshs x
 cmpu ,s++
 bhs return
 ldd [6,s]
 incb
 std [6,s]
 puls u,pc
#endasm

