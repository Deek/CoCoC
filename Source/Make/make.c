/*
 *     Do the actual making for make
 */

#include <stdio.h>

/* I suspect these are no longer needed, since the file routines were moved */
/* out to "files.c", but since I'm not sure, I'll leave them here. -TBK */
#ifdef unix
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#endif
#ifdef eon
#include <sys/stat.h>
#include <sys/err.h>
#endif

#include "h.h"



/*
 *     Exec a shell that returns exit status correctly (/bin/esh).
 *     The standard EON shell returns the process number of the last
 *     async command, used by the debugger (ugg).
 *     [exec on eon is like a fork+exec on unix]
 */
int
dosh(string, shell)
char *                 string;
char *                 shell;
{
       int     number;

#ifdef unix
       return system(string);
#endif
#ifdef eon
       return ((number = execl(shell, shell,"-c", string, 0)) == -1) ?
               -1:     /* couldn't start the shell */
               wait(number);   /* return its exit status */
#endif
#ifdef OS9
       int     status, pid;

       strcat(string, "\n");
       if ((number = os9fork(shell, strlen(string), string, 0, 0, 0)) == -1)
               return -1;              /* Couldn't start a shell */
       do {
               if ((pid = wait(&status)) == -1)
                       return -1;      /* child already died!?!? */
       } while (pid != number);

       return status;
#endif
}


/*
 *     Do commands to make a target
 */
void
docmds1(np, lp)
struct name *          np;
struct line *          lp;
{
       bool                    ssilent;
       bool                    signore;
       int                     estat;
       register char *         q;
       register char *         p;
       char *                  shell;
       register struct cmd *   cp;


       if (*(shell = getmacro("SHELL")) == '\0')
               shell = DEFSHELL;

       for (cp = lp->l_cmd; cp; cp = cp->c_next)
       {
               strcpy(str1, cp->c_cmd);
               expand(str1);
               q = str1;
               ssilent = silent;
               signore = ignore;
               while ((*q == '@') || (*q == '-'))
               {
                       if (*q == '@')     /*  Specific silent  */
                               ssilent = TRUE;
                       else               /*  Specific ignore  */
                               signore = TRUE;
                       q++;               /*  Not part of the command  */
               }

               if (!domake)
                       ssilent = 0;

               for (p=q; *p; p++)
               {
                       if (*p == '\n' && p[1] != '\0')
                       {
                               *p = ' ';
                               if (!ssilent)
                                       fputs("\\\n", stdout);
                       }
                       else if (!ssilent)
                               putchar(*p);
               }
               if (!ssilent)
                       putchar('\n');

               if (domake)
               {                       /*  Get the shell to execute it  */
                       if ((estat = dosh(q, shell)) != 0)
                       {
                               if (estat == -1)
                                       fatal("Couldn't execute %s", shell);
                               else
                               {
                                       printf("%s: Error code %d", myname, estat);
                                       if (signore)
                                               fputs(" (Ignored)\n", stdout);
                                       else
                                       {
                                               np->n_flag |= N_ERROR;
                                               putchar('\n');
                                               if (!(np->n_flag & N_PREC))
                                                       if (unlink(np->n_name) == 0)
                                                               printf("%s: '%s' removed.\n", myname, np->n_name);
                                               exit(estat);
                                       }
                               }
                       }
               }
       }
}


docmds(np)
struct name *          np;
{
       register struct line *  lp;


       for (lp = np->n_line; lp; lp = lp->l_next)
               docmds1(np, lp);
}


/*
 *     Recursive routine to make a target.
 */
int
make(np, level)
struct name *          np;
int                    level;
{
       register struct depend *        dp;
       register struct line *          lp;
       register struct depend *        qdp;
       time_t                          dtime = 1;
       bool                            didsomething = 0;


       if (np->n_flag & N_DONE)
       {
               if (level == 0)
                      printf("%s: '%s' is up to date.\n", myname, np->n_name);
               return 0;
       }

       if (np->n_flag & N_MARK)
            fatal("Rules created circular dependency at '%s'.\n",np->n_name);
       else
               np->n_flag |= N_MARK;

       if (!np->n_time)
               modtime(np);            /*  Gets modtime of this file  */

       if (rules)
       {
               for (lp = np->n_line; lp; lp = lp->l_next)
                       if (lp->l_cmd)
                               break;
               if (!lp)
                       dyndep(np);
       }

       if (!(np->n_flag & N_TARG) && np->n_time == 0L)
               fatal("Don't know how to make %s", np->n_name);

       for (qdp = (struct depend *)0, lp = np->n_line; lp; lp = lp->l_next)
       {
               for (dp = lp->l_dep; dp; dp = dp->d_next)
               {
                       make(dp->d_name, level+1);
                       if ((np->n_time <= dp->d_name->n_time) || force)
                               qdp = newdep(dp->d_name, qdp);
                       dtime = max(dtime, dp->d_name->n_time);
               }
               if ( (np->n_flag & N_DOUBLE) && !quest
                    && ((force) || (np->n_time <= dtime)) )
               {
                       if (debug)
                            printf("\"%s\" needs to be made.\n",np->n_name);
                       make1(np, lp, qdp);     /* free()'s qdp */
                       dtime = 1;
                       qdp = (struct depend *)0;
                       didsomething++;
               }
       }

       pflinit();
       
       np->n_flag |= N_DONE;

       if (quest)
       {
               long            t;

               t = np->n_time;
               if ( t <= dtime )  time(&np->n_time);

               if (debug) {
                       printf("\"%s\" ",np->n_name);
                       if ( t<= dtime) printf("needs");
                       else            printf("does not need");
                       printf(" to be made.\n");
               }
               return (t <= dtime);
       }
       else if (((np->n_time <= dtime) || (force)) && !(np->n_flag & N_DOUBLE))
       {
               if (debug)
                     printf("\"%s\" needs to be made.\n",np->n_name);
               make1(np, (struct line *)0, qdp);       /* free()'s qdp */
               time(&np->n_time);
               didsomething++;
       }
       else if (level == 0 && !didsomething)
               printf("%s: '%s' is up to date.\n", myname, np->n_name);
       if (debug && !didsomething) {
               printf("\"%s\" does not need to be made.\n",np->n_name);
       }
       return 0;
}


/*
 * Set up internal macros and call docmds.
 *
 * @  - full target name
 * <  - Implicitly found prerequisite
 * ?  - list of out-of-date prerequisites
 * *  - target name with suffix deleted
 */
make1(np, lp, qdp)
register struct depend *       qdp;
struct line *                  lp;
struct name *                  np;
{
       register struct depend *        dp;
       register struct line *          lp1;
       register char *                 p;

       if (dotouch)
               touch(np);
       else
       {       /* Set up ? macro -- list of out-of-date prerequisites */
               strcpy(str1, "");
               for (dp = qdp; dp; dp = qdp)
               {
                       if (strlen(str1))
                               strcat(str1, " ");
                       strcat(str1, dp->d_name->n_name);
                       qdp = dp->d_next;
                       free(dp);
               }
               setmacro("?", str1);

               /* Set up < macro -- implicit prerequisite */
               setmacro("<", np->n_impl );

               /* Set up @ macro -- current target name */
               setmacro("@", np->n_name);

               /* Set up * macro -- target without suffix */
               strcpy(str1, np->n_name);
               if (p=suffix(str1))      /* If suffix, remove it */
                       *p='\0';
               setmacro("*", str1);

               /* Now go execute the commands */
               if (lp)         /* lp set if doing a :: rule */
                       docmds1(np, lp);  /* Do just these cmds */
               else
                       docmds(np);       /* Do all cmds for target */
       }
}