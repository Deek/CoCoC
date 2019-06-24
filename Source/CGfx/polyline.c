/*
 "polyline.c" - polygon outline drawing function.

 by Mike Sweet 9/20/90
*/

#include <polygon.h>

int PolyLine(path,polygon)
int path;
VERTEX *polygon;
{
 int status;
 register VERTEX *temp;

 temp=polygon;

 status=SetDPtr(path,temp->p_xcor,temp->p_ycor);

 do
  {
   temp++;
   status |= LineM(path,temp->p_xcor,temp->p_ycor);
  }
 while (temp->p_xcor != polygon->p_xcor || temp->p_ycor != polygon->p_ycor);

 return (status);
}
