/*
More multi-vue functions for the CGFX library
by Mike Sweet 10/10/89
*/

#include <lowio.h>
#include <mouse.h>
#include <sgstat.h>

static MSRET mp; /* mouse data packet */
static struct sgbuf oldopts,newopts;

/*
int MouseKey(path)
int path;

  Returns a keypress from path if one is available, or returns -1 or -2 if
one of the mouse buttons has been pressed (-1=left button, -2=right button.)
If neither a keypress is ready nor a button is pressed, then it returns a 0.
*/

int MouseKey(path)
register int path;
{
 char ch; /* character variable used to read a character from the window */

 _gs_opt(path,&oldopts);
 _gs_opt(path,&newopts);
 newopts.sg_echo=newopts.sg_kbich=newopts.sg_kbach=0;
 _ss_opt(path,&newopts);

 while (TRUE)
  {
   if (_gs_rdy(path)>0) /* chars ready? */
    {
     read(path,&ch,1); /* get the waiting character */
     _ss_opt(path,&oldopts);
     return(ch); /* and give it to the caller */
    }
   else
    {
     _gs_mous(path,&mp);
     if (!mp.pt_valid)
      continue;

     if (mp.pt_cbsa) /* if the left button has been pressed...*/
      {
       _ss_opt(path,&oldopts);
       tsleep(15);
       return(-1); /* return a -1 for left button */
      }
     else if (mp.pt_cctb) /* else if the right button has been pressed...*/
      {
       _ss_opt(path,&oldopts);
       tsleep(15);
       return(-2); /* return -2 for the right button */
      };
    };
  };
}
 


