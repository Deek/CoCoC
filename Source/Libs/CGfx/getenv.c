/*
 "getenv.c" - environment variable reading/setting functions.
 updated 8/6/90 by Mike Sweet
*/

char *_ENVFILE=0; /* this global pointer holds the current ENV.FILE contents. */
static char envstr[81];

static char *readenv()
{
 register int path,len;

 if (!(_ENVFILE=malloc(1024)))
  return(0);

 if ((path=open("/dd/sys/env.file",1))==-1)
  {
   free(_ENVFILE);
   _ENVFILE=0;
   return(0);
  };

 if ((len=read(path,_ENVFILE,1023))<0)
  {
   close(path);
   free(_ENVFILE);
   _ENVFILE=0;
   return(0);
  };

 _ENVFILE[len]=0;
 close(path);
 return(_ENVFILE);
}



static char *findenv(s)
char *s;
{
 char *temp;
 register int len, slen=strlen(s);

 if (!_ENVFILE)
  readenv();

 temp=_ENVFILE;

 while (temp)
  {
   if (temp[0]!='*') /* ignore commented lines */
    {
     len=0;
     while ((temp[len]!=' ') && (temp[len]!='='))
      len++;
     if (strnucmp(temp,s,slen)==0)
      {
       temp+=len;
       while (*temp && *temp!='=')
        temp++;
       temp++;
       while (*temp && *temp==' ')
        temp++;
       return(temp);
      };
    };
   if (temp=strchr(temp,'\n'))
    temp++;
  };
 return(0);
}


char *getenv(s)
char *s;
{
 register char *temp;
 int len;

 temp=findenv(s);
 len=((char *)strchr(temp,'\n')-temp); /* We need to use this more than once */

 strncpy(envstr,temp,len);
 envstr[len] = 0;                      /* Add a terminating NUL */
 return(envstr);
} 



putenv(v,s)
char *v,*s;
{
 register char *temp;
 int len,slen,path;

 slen=strlen(s);

 if (temp=findenv(v))
  {
   len=(char *)strchr(temp,'\n')-temp;
   movemem(temp+slen,temp+len,strlen(temp+len)+1);
   movemem(temp,s,slen);
  }
 else if (_ENVFILE)
  {
   if ((strlen(v)+strlen(_ENVFILE)+slen+2)>1023)
    return;
   strcat(_ENVFILE,v);
   strcat(_ENVFILE,"=");
   strcat(_ENVFILE,s);
   strcat(_ENVFILE,"\n");
  }
 else
  return;

 if ((path=open("/dd/sys/env.file",2))==-1)
  return;

 write(path,_ENVFILE,strlen(_ENVFILE));
 close(path);
 return;
}
