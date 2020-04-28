
#include <password.h>

static int     pwpn = 0;
char           _pwdelim = 'a';
static char    tmpbuf[PWSIZ+1], *index();
static PWENT   pwent, *parse();


/*
** get the next password entry
*/

PWENT *getpwent()
   {
   char  *p;

   if (pwpn == 0)
      if ((pwpn = open(PASSWORD, 1)) <= 0)
         return ((PWENT *) 0);
   if (readln(pwpn, tmpbuf, PWSIZ) > 0)
      {
      for (p = tmpbuf; (*p != '\n') && (_pwdelim != ',' && _pwdelim != ':'); )
         _pwdelim = *p++;
      if (*p != '\n')
         return (parse(tmpbuf, &pwent));
      }
   return ((PWENT *) 0);
   }


/*
** rewind the password file
*/

/*void*/ setpwent()
   {
   if (pwpn != 0)
      lseek(pwpn, 0L, 0);
   }


/*
** terminate password file access
*/

/*void*/ endpwent()
   {
   if (pwpn != 0)
		{
      close(pwpn);
		pwpn = 0;
		}
   }


/*
** return the password delimiter
*/

int getpwdlm()
   {
   return ((int) _pwdelim);
   }
/*page*/
/*
** routine to parse a password file entry
*/

static PWENT *parse(p, pwp)
char  *p;
PWENT *pwp;
   {
   pwp->ugcos = (char *) 0;                         /* just in case */
   pwp->unam = p;
   *(p = index(p, _pwdelim)) = '\0';
   pwp->upw = ++p;
   *(p = index(p, _pwdelim)) = '\0';
   pwp->uid = ++p;
   *(p = index(p, _pwdelim)) = '\0';
   pwp->upri = ++p;
   *(p = index(p, _pwdelim)) = '\0';
   if (_pwdelim == UNXDLM)
      {
      pwp->ugcos = ++p;
      *(p = index(p, _pwdelim)) = '\0';
      }
   pwp->ucmd = ++p;
   *(p = index(p, _pwdelim)) = '\0';
   pwp->udat = ++p;
   *(p = index(p, _pwdelim)) = '\0';
   pwp->ujob = ++p;
   *(index(p, '\n')) = '\0';
   return (pwp);
   }


/*
** find a user by ID
*/

PWENT *getpwuid(uid)
int   uid;
   {
   PWENT *pwp;

   while (pwp = getpwent())
      if (uid == atoi(pwp->uid))
         return (pwp);
   return ((PWENT *) 0);
   }


/*
** find a user by name
*/

PWENT *getpwnam(name)
char  *name;
   {
   PWENT *pwp;

   while (pwp = getpwent())
      if (strucmp(name, pwp->unam) == 0)
         return (pwp);
   return ((PWENT *) 0);
   }
