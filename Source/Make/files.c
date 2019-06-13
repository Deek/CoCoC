/*
 *     Find and update file modification times
 */

#include <stdio.h>
#ifdef unix
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#endif
#ifdef eon
#include <sys/stat.h>
#include <sys/err.h>
#endif
#ifdef OS9
#include <time.h>
#include <os9.h>
#include <modes.h>
#include <direct.h>
#include <errno.h>
#endif
#include "h.h"



#ifdef OS9
/*
 *     Some stuffing around to get the modified time of a file
 *     in an OS9 file system
 */
getmdate(fd, tbp)
struct sgtbuf *                tbp;
{
       struct registers        regs;
       static struct fildes    fdbuf;


       regs.rg_a = fd;
       regs.rg_b = SS_FD;
       regs.rg_x = &fdbuf;
       regs.rg_y = sizeof (fdbuf);

       if (_os9(I_GETSTT, &regs) == -1)
       {
               errno = regs.rg_b & 0xff;
               return -1;
       }
       if (tbp)
       {
               _strass(tbp, fdbuf.fd_date, sizeof (fdbuf.fd_date));
               tbp->t_second = 0;      /* Files are only acurate to mins */
       }
       return 0;
}


/*
 *     Kludge routine to return an aproximation of how many
 *     seconds since 1980.  Dates will be in order, but will not
 *     be linear
 */
time_t
cnvtime(tbp)
struct sgtbuf          *tbp;
{
       long                    acc;

       if (tbp->t_year > 80)
           acc = tbp->t_year - 80;         /* Baseyear is 1980 */
       else if (tbp->t_year!= 0)
           acc = tbp->t_year + 100 - 80;  /* Years <80 are in 21st cent. */
       acc = acc * 12 + tbp->t_month;
       acc = acc * 31 + tbp->t_day;
       acc = acc * 24 + tbp->t_hour;
       acc = acc * 60 + tbp->t_minute;
       acc = acc * 60 + tbp->t_second;

       return acc;
}


/*
 *     Get the current time in the internal format
 */
time(tp)
time_t *               tp;
{
       struct sgtbuf           tbuf;


       if (getime(&tbuf) < 0)
               return -1;

       if (tp)
               *tp = cnvtime(&tbuf);

       return 0;
}
#endif


/*
 *     Get the modification time of a file.  If the first
 *     doesn't exist, it's modtime is set to 0.
 */
void
modtime(np)
struct name *          np;
{
#ifdef unix
       struct stat             info;
       int                     fd;


       if (stat(np->n_name, &info) < 0)
       {
               if (errno != ENOENT)
                       fatal("Can't open %s; error %d", np->n_name, errno);

               np->n_time = 0L;
       }
       else
               np->n_time = info.st_mtime;
#endif
#ifdef eon
       struct stat             info;
       int                     fd;


       if ((fd = open(np->n_name, 0)) < 0)
       {
               if (errno != ER_NOTF)
                       fatal("Can't open %s; error %d", np->n_name, errno);

               np->n_time = 0L;
       }
       else if (getstat(fd, &info) < 0)
               fatal("Can't getstat %s; error %d", np->n_name, errno);
       else
               np->n_time = info.st_mod;

       close(fd);
#endif
#ifdef OS9
       struct sgtbuf           info;
       int                     fd;
       int                     mode=0;
       char *                  suff;
       char                    fullname[256];
       
       suff=suffix(np->n_name);
       fullname[0]='\0';
       
       if ((suff == (char *)0 ) && (rules)) { /* Is this an executable? */
               mode = 4;
               strcpy(fullname,object_dir);
               if (strlen(object_dir) != 0)
                       strcat(fullname,"/");
       } else if ((strcmp(suff,".r")==0) && (rules)) {  /* Is it .r ?? */
               strcpy(fullname,rel_dir);
               if (strlen(rel_dir) != 0)
                       strcat(fullname,"/");
       }
       strcat(fullname,np->n_name);

       if (((fd = open(fullname, mode)) < 0) &&   /* Check mode first */
           ((fd = open(fullname,  0  )) < 0))    /* Then data dir */
       {
               if (errno != E_PNNF)
                       fatal("Can't open %s with mode %d; error %d",
                             fullname, mode, errno);
               if (debug)
                  printf("\"%s\" does not exist, timestamp = 0.\n",fullname);

               np->n_time = 0L;
       }
       else if (getmdate(fd, &info) < 0)
               fatal("Can't getstat %s; error %d", np->n_name, errno);
       else {
               np->n_time = cnvtime(&info);
       }
       if (debug && (np->n_time != 0L)) {
               printf("\"%s\" was last modified",fullname);
               printf(" %3d/%02d/%02d  %2d:%02d; timestamp %ld.\n",
                     info.t_year,info.t_month,info.t_day,info.t_hour,
                     info.t_minute,np->n_time);
       }   

       close(fd);
#endif
}


/*
 *     Update the mod time of a file to now.
 */
void
touch(np)
struct name *          np;
{
       char                    c;
       int                     fd;


       if (!domake || !silent)
               printf("    touch(%s)\n", np->n_name);

       if (domake)
       {
#ifdef unix
               long            a[2];

               a[0] = a[1] = time(0);
               if (utime(np->n_name, &a[0]) < 0)
                       printf("%s: '%s' not touched - non-existent\n",
                                       myname, np->n_name);
#endif
#ifdef eon
               if ((fd = open(np->n_name, 0)) < 0)
                       printf("%s: '%s' not touched - non-existent\n",
                                       myname, np->n_name);
               else
               {
                       uread(fd, &c, 1, 0);
                       uwrite(fd, &c, 1);
               }
               close(fd);
#endif
#ifdef OS9
               /*
                *      Strange that something almost as totally useless
                *      as this is easy to do in OS9!
                */
               int                     mode=0;
               char *                  suff;
               char                    fullname[256];
       
               suff=suffix(np->n_name);
               fullname[0]='\0';

               if ((suff == (char *)0 ) && (rules)) { /* Is this an exec? */
                       mode = 4;
                       strcpy(fullname,object_dir);
                       if (strlen(object_dir) != 0)
                               strcat(fullname,"/");
               } else if ((strcmp(suff,".r")==0) && (rules)) { /*Is it .r ?*/
                       strcpy(fullname,rel_dir);
                       if (strlen(rel_dir) != 0)
                               strcat(fullname,"/");
               }
               strcat(fullname,np->n_name);

               if (debug)
                   printf("Touching %s with mode %d.\n",fullname,mode);

               if ((fd = open(fullname, S_IWRITE | mode )) < 0) {
                       printf("%s: '%s' not touched - non-existent.\n",
                                       myname, fullname);
               }
               close(fd);
#endif
       }
}

