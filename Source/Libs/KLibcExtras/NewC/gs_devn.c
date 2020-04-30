_gs_devn(fd, buf)
int   fd;
char  *buf;
   {
   register char *p = buf;

   getstat(0x0e, fd, p);
   strhcpy(p, p);
   }
