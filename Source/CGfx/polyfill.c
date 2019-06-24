/*
 "polyfill.c" - polygon filling routine.
 
 by Mike Sweet 9/20/90
*/


#include <polygon.h>

#define NULL 0

#define MAXLINES 192 /* 192 scan lines */

typedef struct {   /* polygon edge structure */
 short p_numedges; /* number of edges for this scan line... */
 short *p_edges;   /* pointer to list of edges... */
 } EDGE;

static EDGE edges[MAXLINES];


static int addpt(x,y)
int x,y;
{
 int n,n2;
 register short *temp;

 if (x<0 || x>639 || y<0 || y>191)
  return(0);

 if (edges[y].p_numedges==0)
  if (edges[y].p_edges=malloc(sizeof(short)))
   {
    *(edges[y].p_edges)=x;
    edges[y].p_numedges=1;
    return(0);
   }
  else
   return(-1);

 if (temp=realloc(edges[y].p_edges,sizeof(short)*(edges[y].p_numedges+1)))
  {
   edges[y].p_edges=temp;
   for (n=0;n<edges[y].p_numedges;n++,temp++)
    if (x < *temp)
     break;
/*   for (n2=edges[y].p_numedges-1;n2>n;n2--)
    edges[y].p_edges[n2]=edges[y].p_edges[n2-1];
*/
   movemem(edges[y].p_edges+n+1,edges[y].p_edges+n,(edges[y].p_numedges-n)*2);
   edges[y].p_numedges++;
   *temp=x;
   return(0);
  }
 else
  return(-1);
}



int PolyFill(path,polygon)
int path;
VERTEX *polygon;
{
 register VERTEX *temp;
 int xcor,ycor;
 int dx,dy;
 int incx,incy,lastincy;
 int halfx,halfy;
 int errterm;
 int status;
 int n,n2;

 for (n=0;n<MAXLINES;n++)
  edges[n].p_numedges=0;

 temp=polygon+1;

 while (temp->p_xcor != polygon->p_xcor || temp->p_ycor != polygon->p_ycor)
  temp++;

 if ((dy=(temp-1)->p_ycor - temp->p_ycor)<0)
  lastincy = 1;
 else if (dy>0)
  lastincy = -1;
 else
  lastincy = 0;

 temp=polygon;

 do
  {
   xcor=temp->p_xcor;
   ycor=temp->p_ycor;
   temp++;
 
   if ((dx = xcor - temp->p_xcor) < 0)
    {
     incx = 1;
     dx = - dx;
    }
   else
    incx = -1;

   if ((dy = ycor - temp->p_ycor) < 0)
    {
     incy = 1;
     dy = -dy;
    }
   else if (dy > 0)
    incy = -1;
   else
    incy = 0;

   halfx = dx/2;
   halfy = dy/2;
   errterm=0;

   if (incy*lastincy<0)
    addpt(xcor,ycor);

   if (dy==0)
    {
     if (lastincy*((temp+1)->p_ycor - temp->p_ycor)<0)
      addpt(xcor,ycor);
    }
   else if (dx>dy)
    {
     dx=dx*incx;
     while (xcor != temp->p_xcor)
      {
       if (status=addpt(xcor,ycor))
        break;
       xcor+=(dx+errterm)/dy;
       errterm=(dx+errterm) % dy;
       ycor+=incy;
      };
    }
   else
    {
     n=dy;
     while (n>0)
      {
       if (status=addpt(xcor,ycor)) /* add a point to the lists... */
        break;
       errterm+=dx;
       if (errterm>=halfy)
        {
         errterm-=dy;
         xcor+=incx;
        };
       ycor+=incy;
       n--;
      };
    }; 
   lastincy=incy;
  }
 while (temp->p_xcor != polygon->p_xcor || temp->p_ycor != polygon->p_ycor);

 if (status)
  {
   for (n=0;n<MAXLINES;n++)
    if (edges[n].p_numedges)
     free(edges[n].p_edges);
   return(-1);
  };

 for (n=0;n<MAXLINES;n++)
  {
   register short *temp;
   if (edges[n].p_numedges)
    {
     for (n2=edges[n].p_numedges-1,temp=edges[n].p_edges;n2>0;n2-=2,temp+=2)   
      {
       status |= SetDPtr(path,*temp,n);
       status |= Line(path,*(temp+1),n);
      };
     free(edges[n].p_edges);
    };
  };
 
 return(status);
}

