/*
 'settype.c' - a function to set-up a window only if necessary.
 
 by Mike Sweet 6/2/90
*/

SetType(path,stype,fg,bg)
register int path;
int stype,fg,bg;
{
 int oldtype;

 _gs_styp(path,&oldtype);

 if (oldtype!=stype)
  {
   DWEnd(path);
   if ((stype==1) | (stype==6) | (stype==8))
    DWSet(path,stype,0,0,40,24,fg,bg,bg);
   else
    DWSet(path,stype,0,0,80,24,fg,bg,bg);
   Select(path);
  };
 FColor(path,fg);
 BColor(path,bg);
 Border(path,bg);
 Clear(path);
 _Flush();
}
