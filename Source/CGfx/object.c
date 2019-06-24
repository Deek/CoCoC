/*
 "object.c" - sprite handling functions for Level II windows....
 by Mike Sweet 1/30/90
*/

#define NULL 0


/*
 Each object (or sprite) has certain information associated with it.
 The 'OBJECT' type holds this info.  On special part of the type is the
 border field- this is a pointer to a function to call to move-and/or check
 boundaries for the object.  This function must return a 0 if the object
 is to remain on the screen and a 1 if it should be deleted.  The function
 recieves one pointer to an OBJECT.  If this is a NULL pointer then a simple 
 window boundary check is done and the object 'bounces' off the sides.
 Also, all the coordinates and offsets are fixed-point numbers.  That is,
 each is an integer representing the actual position/offset * 32.
 This allows for fractional movement resulting in smoother movement.
*/

typedef struct OBJSTR {
 char group;          /* G/P group for this object */
 char buffer;         /* G/P buffer for this object */
 int xcor;            /* xcor * 32 */
 int ycor;            /* ycor * 32 */
 int deltax;          /* +/- xcor */
 int deltay;          /* +/- ycor */
 int xaccel;          /* xcor acceleration */
 int yaccel;          /* ycor acceleration */
 int (*border)();     /* function to call to check boundaries */
 struct OBJSTR *next; /* pointer to next object */
 struct OBJSTR *prev; /* pointer to previous object */
 } OBJECT;            /* call this type OBJECT */
 
OBJECT *Objects=NULL; /* pointer to OBJECT list */







OBJECT *AddObj(path,group,buffer,xcor,ycor,border)
int path,group,buffer,xcor,ycor,(*border)();
{
 register OBJECT *temp;
 
 if ((temp=malloc(sizeof(OBJECT)))==NULL)
  return(NULL);
  
 temp->group=group;
 temp->buffer=buffer;
 temp->xcor=xcor;
 temp->ycor=ycor;
 temp->border=border;
 temp->deltax=
 temp->deltay=
 temp->xaccel=
 temp->yaccel=
 temp->prev=0;
 temp->next=Objects;
 if (Objects)
  Objects->prev=temp;

 Objects=temp;
 if (group)
  PutBlk(path,group,buffer,xcor>>5,ycor>>5);
 else
  {
   FColor(path,buffer);
   Point(path,xcor>>5,ycor>>5);
  };
 _Flush();
 return(temp);
}


MoveObj(path)
int path;
{
 register OBJECT *temp;
 
 temp=Objects;
 
 while (temp)
  {
   if (temp->group)
    PutBlk(path,temp->group,temp->buffer,temp->xcor>>5,temp->ycor>>5);
   else
    {
     FColor(path,temp->buffer);
     Point(path,temp->xcor>>5,temp->ycor>>5);
    };
   if (temp->border)
    {
     if ((*temp->border)(temp))
      DelObj(-1,temp);
    }
   else
    {
     temp->xcor+=temp->deltax;
     temp->ycor+=temp->deltay;
     temp->deltay+=temp->yaccel;
     temp->deltax+=temp->xaccel;
     if ((temp->xcor<0) || (temp->xcor>20479))
      {
       temp->xcor-=temp->deltax;
       temp->deltax= -temp->deltax;
      };
     if ((temp->ycor<0) || (temp->ycor>6143))
      {
       temp->ycor-=temp->deltay;
       temp->deltay= -temp->deltay;
      };
    };
   if (temp->group)
    PutBlk(path,temp->group,temp->buffer,temp->xcor>>5,temp->ycor>>5);
   else
    {
     FColor(path,temp->buffer);
     Point(path,temp->xcor>>5,temp->ycor>>5);
    };
    
   temp=temp->next;
  };
 _Flush();
}


DelObj(path,objptr)
int path;
register OBJECT *objptr;
{
 if (objptr->next)
  (objptr->next)->prev=objptr->prev;
 if (objptr->prev)
  (objptr->prev)->next=objptr->next;
 else
  Objects=objptr->next;

 if (path!=-1) /* only erase if we have a valid path */
  {
   if (objptr->group)
    PutBlk(path,objptr->group,objptr->buffer,objptr->xcor>>5,objptr->ycor>>5);
   else
    {
     FColor(path,objptr->buffer);
     Point(path,objptr->xcor>>5,objptr->ycor>>5);
    };
   _Flush();
  };
  
 free(objptr);
}
