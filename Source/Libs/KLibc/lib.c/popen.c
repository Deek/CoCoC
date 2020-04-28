/*#define MAIN   /* Only use when testing routines */

#ifdef MAIN
#define DEBUG
#endif

/*
 * Popen, and Pclose, for OS-9.
 *
 * Simmule Turner - simmy@nu.cs.fsu.edu - 70651,67
 *
 * V 1.3  10/22/88 - Forgot to close pipe on an error.
 * V 1.2  06/28/88 - Removed shell as parent process.
 *                 - It forks command directly now.
 * V 1.1  06/28/88 - Uses a shell to run child process SrT
 *                 - Fixed bug found by PWL, SrT
 *                 - Improved error checking, cleaned up code.
 * V 1.0  06/25/88 - Initial coding SrT
 *
 */

#include <stdio.h>

#define ERR      (-1)
#define PIPEMAX  _NFILE
#define READ     1 /* For OS-9 */
#define WRITE    2 /* For OS-9 */
#define STDIN    0 /* For OS-9 */
#define STDOUT   1 /* For OS-9 */

#define RESTORE  free(parameter);close(path);dup(save);close(save);

static int   _pid[PIPEMAX];

FILE *popen(command, type)
    char *command, *type;
{
    register char *p = command;
    char *parameter;
    FILE *_pfp;
    int l, path, pipe, pcnt, save;

    path = (*type == 'w') ? STDIN : STDOUT;

    if ((pipe = open("/pipe",READ+WRITE)) == ERR)
        return(NULL);
    pcnt = pipe;

    if ((save = dup(path)) == ERR) {
        close(pipe);
        return(NULL);
    }
    close(path);

    if (dup(pipe) == ERR) {
        dup(save);
        close(save);
        close(pipe);   /* BUG fix  10/22/88 SrT */ 
        return(NULL);
    }

    while (*p != ' ' && *p)
        p++;
    if (*p == ' ')
        p++;
    l = strlen(p);
    parameter = (char *) malloc(l+2);
    strcpy(parameter,p);
    strcat(parameter,"\n");

#ifdef DEBUG
_dump("Parameter: ",parameter,l+1);
#endif

    if ((_pid[pcnt] = os9fork(command,l+1,parameter,1,1,0)) == ERR) {
        { RESTORE }
        close(pipe);
        _pid[pcnt] = 0;
        return(NULL);
    }

    { RESTORE }

    if ((_pfp = fdopen(pipe,type)) == NULL) {
        close(pipe);
        while (((l=wait(0)) != _pid[pcnt]) && l != ERR)
            ;
        _pid[pcnt] = 0;
        return(NULL);
    }

    return(_pfp);
}

int pclose(stream)
FILE *stream;
{
    register int i;
    int f,status;

    f = fileno(stream);
    fclose(stream);
#ifdef DEBUG
    fprintf(stderr,"Pclose:  Fileno=%d  PID=%d\n",f,_pid[f]);
#endif
    while (((i=wait(&status)) != _pid[f]) && i != ERR)
        ;
    _pid[f]= 0;
    return((i == ERR) ? ERR : status);
}

#ifdef MAIN

#define LINSIZ 200
main() {
    char line[LINSIZ];
    FILE *popen(), *fp;
    int status;

    /* Test the read side of popen
     * SrT 06/25/88 */

    if (( fp = popen("procs e","r")) != NULL) {
        while (fgets(line,LINSIZ,fp) != NULL)
            printf("%s",line);
        if ((status = pclose(fp)) == ERR) {
            fprintf(stderr,"***ERR: closing read pipe ERR #%03d\n",
                            status&0xff);
            exit(1);
        }
        printf("Read status =%d\n",status&0xff);
    }
    else {
        fprintf(stderr,"***ERR: opening read pipe\n");
        exit(1);
    }

    /* Test the write side of popen
     * SrT 06/25/88 */
    if (( fp = popen("woof one two three four","w")) != NULL) {
        while (fgets(line,LINSIZ,stdin) != NULL)
            fprintf(fp,"%s",line);
        if ((status = pclose(fp)) == ERR) {
            fprintf(stderr,"***ERR: closing write pipe ERR #%03d\n",
                            status&0xff);
            exit(1);
        }
       printf("Write status =%d\n",status&0xff);
    }
    else {
        fprintf(stderr,"***ERR: opening write pipe\n");
        exit(1);
    }
}
#endif
