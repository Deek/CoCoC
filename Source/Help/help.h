/*
 * Help utility for OS9
 *    This program is in the Public Domain.
 */

#define TRUE   1     /* Changed to +1 for patmatch to work */
#define FALSE  0

/*
 *  Directory where "help" starts.
 */
#define HELP_DIR  "/DD/Sys/Help"

/*
 *  Name of shell command forked by "$" escape
 */
#define SHELL_COM "shell"

/*
 *  Used for building linked lists of strings.
 */
struct stacknode {
     char *word;
     struct stacknode *next;
};
#define NULL (struct stacknode *)0


#define BUF_SIZE 1000
extern char scratch[BUF_SIZE];  /* Available for general use */

extern int interrupted;  /*  Set to TRUE by sighandler if an interrupt occurs */
extern int clicked;  /*  Set to TRUE by sighandler if mouse pressed */
extern int keyed;  /*  Set to TRUE by sighandler if key pressed */
extern int noask;	/* Set if we are to exit when done displaying */

extern char inbuff[BUF_SIZE];

extern getinput();
