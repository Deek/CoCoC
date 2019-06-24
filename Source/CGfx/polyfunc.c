/*
 "polyfunc.c" - polygon manipulation functions.

 by Mike Sweet 10/1/90
*/

#include <polygon.h>

static int polysize(polygon)
VERTEX *polygon;
{
 int n;
 register VERTEX *temp;

 for (n=2,temp=polygon+1;temp->p_xcor!=polygon->p_xcor || temp->p_ycor!=polygon->p_ycor;temp++,n++);

 return(n);
}


/* PolyBox()- return the limits (coords) of a polygon. */
PolyBox(polygon,left,right,top,bottom)
VERTEX *polygon;
int *left,*right,*top,*bottom;
{
 register VERTEX *temp;

 temp=polygon+1;

 *left = *right = polygon->p_xcor;
 *top = *bottom = polygon->p_ycor;

 while (temp->p_xcor!=polygon->p_xcor || temp->p_ycor!=polygon->p_ycor)
  {
   if (temp->p_xcor < *left)
    *left = temp->p_xcor;
   else if (temp->p_xcor > *right)
    *right = temp->p_xcor;

   if (temp->p_ycor < *top)
    *top = temp->p_ycor;
   else if (temp->p_ycor > *bottom)
    *bottom = temp->p_ycor;

   temp++;
  };
}


/* PolyRot()- rotate a polygon 'angle' degrees around (cx,cy) */
VERTEX *PolyRot(polygon,cx,cy,angle)
VERTEX *polygon;
int cx,cy,angle;
{
 register VERTEX *temp,*temp2;
 int n;

 n=polysize(polygon);

 if (temp2=malloc(sizeof(VERTEX)*n))
  {
   temp=temp2;
   while (n>0)
    {
     temp->p_xcor = cx + Cosine(polygon->p_xcor - cx,angle) - Sine(polygon->p_ycor - cy,angle);
     temp->p_ycor = cy + Cosine(polygon->p_ycor - cy,angle) + Sine(polygon->p_xcor - cx,angle);
     temp++;
     polygon++;
     n--;
    };
  };

 return(temp2);
}


/* PolyScal() - scale a polygon about (cx,cy) */
VERTEX *PolyScal(polygon,cx,cy,xmult,ymult,div)
VERTEX *polygon;
int cx,cy,xmult,ymult,div;
{
 int n;
 register VERTEX *temp,*temp2;

 n=polysize(polygon);

 if (temp2=malloc(sizeof(VERTEX)*n))
  {
   temp=temp2;
   while (n>0)
    {
     temp->p_xcor = cx + (polygon->p_xcor - cx) * xmult / div;
     temp->p_ycor = cy + (polygon->p_ycor - cy) * ymult / div;
     temp++;
     polygon++;
     n--;
    };
  };

 return(temp2);
}


/* PolyTran() - translate (move) a polygon relative to (xoff,yoff) */
VERTEX *PolyTran(polygon,xoff,yoff)
VERTEX *polygon;
int xoff,yoff;
{
 register VERTEX *temp,*temp2;
 int n;

 n=polysize(polygon);

 if (temp2=malloc(sizeof(VERTEX) * n))
  {
   temp=temp2;
   while (n>0)
    {
     temp->p_xcor = polygon->p_xcor + xoff;
     temp->p_ycor = polygon->p_ycor + yoff;
     temp++;
     polygon++;
     n--;
    };
  };

 return(temp2);
}
