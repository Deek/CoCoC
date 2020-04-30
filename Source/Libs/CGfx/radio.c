/*
 "radio.c" - some quick functions to put radio buttons on the screen.
 by Mike Sweet 6/18/90
*/


RBUp(path,column,row,fg,bg) /* Radio Button (up) */
register int path;
int column,row,fg,bg;
{
 FColor(path,bg);
 BColor(path,bg);
 SetDPtr(path,column*8,row*8);
 RBar(path,7,7);
 FColor(path,fg);
 RSetDPtr(path,3,2);
 Circle(path,3);
 FFill(path);
 RSetDPtr(path,2,2);
 Circle(path,3);
 _Flush();
}


RBDown(path,column,row,fg,bg) /* Radio Button (down) */
int path,column,row,fg,bg;
{
 FColor(path,bg);
 BColor(path,bg);
 SetDPtr(path,column*8,row*8);
 RBar(path,7,7);
 FColor(path,fg);
 RSetDPtr(path,5,4);
 Circle(path,3);
 FFill(path);
 _Flush();
}


