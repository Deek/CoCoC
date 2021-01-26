/*
 * Help utility for OS9
 *    This program is in the Public Domain.
 */

#define VERSION "2.1"

#include "help.h"
#include <signal.h>
#include <dir.h>
#include <stdio.h>

char inbuff[BUF_SIZE];   /* Input buffer for input commands */
char scratch[BUF_SIZE];  /* Scratch buffer for temporary storage */
char request[BUF_SIZE];
direct int interrupted;     /* Set to TRUE when signal received */
direct int clicked;         /* Set to TRUE when MOUSE signal received */
direct int keyed;           /* Set to TRUE when key pressed */
direct int noask = FALSE;

main(argc,argv)
  int argc;
  char *argv[];
{
    struct stacknode *display;  /* Current position in help tree */
    struct stacknode *tmp;
    int sig_handler();

    pagerinit();
    newstack(&display);
    chdir (HELP_DIR);
    intercept(sig_handler);
    *inbuff = '\0';

    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n') {
        noask = TRUE;
        argc--; argv++;
    }

    if(argc<2) {   /* If no arguments, show top-level topics */
        if (!noask) {
            sprintf(scratch,"NitrOS9 Help, ver. %s", VERSION);
            outline(scratch);
            outline("For help using Help, enter \"?\".");
        }
        outline();
        topics();
    } else {
       newstack(&tmp);  /* Otherwise, build list of topic desired */
       while(--argc > 0)
          push(&tmp,*(++argv));
       dostack(&tmp,&display);  /* Process it */
    }

    while (TRUE) {
       interrupted=FALSE;
       clicked=FALSE;
       if (*inbuff == '\0') { /* If don't already have input, prompt for it*/
          prompt(display);
          gets(inbuff);
       }
       /* At any level where a prompt is issued, unrecognized input can */
       /* be stored in 'inbuff'.  At this level, it will be treated as if */
       /* typed at a "topic" prompt */
       strcpy(request,inbuff);   /* Copy input to request, clear inbuff */
       *inbuff = '\0';
       if(interrupted) exit(0); /* Exit on BREAK */
       switch (*request) {
          case '?':  topichelp(); break;
          case '.':  topics(); break;
          case '$':  fork(SHELL_COM,request+1); break;
          case '\0': if (display == NULL) exit(0);  /* Exit from Topic: pmpt */
                     else {
                        chdir("..");   /* Otherwise, back up a level */
                        pop(&display);
                     }
                     break;
          default:   dostring(request,&display);
       }
   }
}

/* Given a string with a topic identification, process that topic */
dostring(input,display)
char *input;
struct stacknode **display;
{
   struct stacknode *topic;
   char *p;

   newstack(&topic);
   if(p=strtok(input," \t")) {  /* Make string into stack */
      push(&topic,p);
      while(p=strtok(NULL," \t")) {
         push(&topic,p);
      }
      dostack(&topic,display);   /* Now, process it as such */
      dump(&topic);  /* Free the stack */
   }
}

dostack(topic,display)
struct stacknode **topic,**display;
{
   struct stacknode *matches;
   int nummatches;
   int quit=0;

   newstack(&matches);
   reverse(topic);  /* Need to work in opposite order */
   while( (*topic) && ((*topic)->next) && !quit) { /* More than one item */
      nummatches=findmatches((*topic)->word,&matches);
      switch(nummatches) {  /* Want exactly one match */
         case 0: sprintf(scratch,"No match found for \"%s\".",(*topic)->word);
                 outline(scratch);
                 quit=1;break;
         case 1: push(display,matches->word); /* Add to display list */
                 formname((*display)->word);
                 if (chdir((*display)->word)){ /* If there's no subdir */
                    sprintf(scratch,"\"%s\" has no subtopics.",(*display)->word);
                    outline(scratch);
                    pop(display);  /* back up to where we were */
                    quit=1;
                 }
                 break;
         default:sprintf(scratch,"\"%s\" is ambiguous.",(*topic)->word);
                 outline(scratch);
                 quit=1;break;
      }
      pop(topic);
      dump(&matches);
   }
   if(*topic && !quit) handleword((*topic)->word,display);
   pop(topic);
}

/* Process an input topic name */
handleword(input,display)
char *input;
struct stacknode **display;
{
   struct stacknode *matches;
   int nummatches;
   char *p;

   newstack(&matches);
   nummatches = findmatches(input,&matches); /* Get list of matches */
   switch (nummatches) {
      case 0: sprintf(scratch,"No match found for \"%s\".",input);
              outline(scratch);
              break;
      case 1: showmatches(*display,matches); /* Exactly one match found */
              push(display,matches->word); /* Add to display list */
              formname((*display)->word);
              if (chdir((*display)->word) == 0) { /* If theres a subdir */
                 topics();   /* show the topics */
              } else {
                 pop(display);  /* Otherwise, back up to where we were */
              }
              break;
      default:sprintf(scratch,"There is more than one topic matching \"%s\".",
			input);
              outline(scratch);
              outline("Please choose one of the following:");
              liststack(matches); /* Show the ones that did match */
              /* Another possibility is to list all matches */
              /* showmatches(*display,matches); */
              break;
   }
   dump(&matches);
}


/* Help at topic selection level */
topichelp()
{
  outline("");
  outline("At any topic or subtopic prompt:");
  outline(" ?     will print this help message");
  outline(" .     will list available topics");
  outline(" ENTER will return to previous topic level");
  outline(" BREAK will exit help");
  outline(" A topic name will display help on that topic");
  outline("");
}

/* Capitalize name and remove extension */
formname(q)
char *q;
{
   char *p;
   for (p=q;*p!='\0';p++) /* Capitalize it */
      *p=toupper(*p);
   if (p=rindex(q,'.'))  /* Remove extension */
      *p = '\0';
}

/* List all files in "list" */
showmatches(sofar,list)
struct stacknode *sofar,*list;
/* char *inbuff; */
/* int interrupted; */
{
   struct stacknode *q;

   q=list;
   while( (q != NULL) && !(*inbuff) && !interrupted ){
     push(&sofar,q->word); /* Push word onto sofar */
     formname(sofar->word);
     sprintstack(scratch,sofar);  /* Display header */
     outline(scratch);
     outline("");  /* blank line after header */
     listfile(q->word);
     q=q->next;  /* to next entry */
     pop(&sofar);
   }
}

prompt(stack)
struct stacknode *stack;
{
  if (stack == NULL) {
     if (noask) exit(0);
     sprintf(scratch,"Topic:");
  } else {
     sprintstack(scratch,stack);
     strcat(scratch,"subtopic:");
  }
  outprompt(scratch);
}

fork(cmd,parm)
char *cmd, *parm;
{
   char newparm[256];
   int  status;

   strcpy(newparm,parm);
   strcat(newparm,"\n");  /* Terminate parms with newline */
   os9fork(cmd,strlen(newparm),newparm,0,0,0);
   wait(&status);
   if(status)
      printf("%s: Error #%d\n",cmd,status); /* Does NOT go through pager */
   return(status);
}

sig_handler(sig)
   int sig;
{
   if (sig == SIGQUIT || sig == SIGINT)
      interrupted = TRUE;
   else if (sig == 'K')
      keyed = TRUE;
   else if (sig == 'M')
      clicked = TRUE;
   else
      exit(sig);
   intercept(sig_handler);  /* Re-set intercept */
}

/*
 *   Format elements of stack into str.
 */
sprintstack(str,stack)
char *str;
struct stacknode *stack;
{
   if (stack != NULL) {
      sprintstack(str,stack->next);
      strcat(str,stack->word);
      strcat(str," ");
   } else
      *str = '\0';
}

/*
 * List elements of stack through pager (elements are assumed to be filenames)
 */
liststack(stack)
struct stacknode *stack;
{
   char *p;

   if ( (stack != NULL) && !liststack(stack->next) ) {
      sprintf(scratch,"    %s",stack->word);
      if (p=rindex(scratch,'.'))  /* If there is an extension, */
          *p = '\0';   /* remove it */
      return( outline(scratch) );
   }
}

/*
 * Stack operations
 */

newstack(stack)
struct stacknode **stack;
{
   *stack = (struct stacknode *)0;
}

push(stack,str)
struct stacknode **stack;
char *str;
{
   struct stacknode *tmp,*p;

   if(!(tmp = (struct stacknode *)malloc(sizeof(struct stacknode))))
       exit(207);
   tmp->next = *stack;
   if(!(p=malloc(strlen(str)+1))) exit(207);
   tmp->word = strcpy(p,str);
   *stack = tmp;
}

pop(stack)
struct stacknode **stack;
{
   struct stacknode *tmp;

   if ((tmp=*stack) != NULL) {
      *stack = tmp->next;
      free(tmp->word);
      free(tmp);
   }
}

dump(stack)
struct stacknode **stack;
{
   while(*stack != NULL) {
      pop(stack);
   }
}

reverse(stack)
struct stacknode **stack;
{
   struct stacknode *tmp,
                    *old, /* Walks through old stack */
                    *rev; /* Points to top of reversed stack */

   rev=NULL;  /* Reversed stack starts off empty */
   old=*stack; /* Point to top of old stack */
   while(old!=NULL) { /* Continue until end */
      tmp=old->next;  /* Remember where rest of old stack is */
      old->next=rev;  /* Add this item to top of reversed stack */
      rev=old;
      old=tmp;  /* Now move down to next item in old stack */
   }
   *stack=rev; /* Now we're done, return reversed stack */
}

/*
 * Find all names in current directory that match "request", and
 *  return linked list of them in "list", return count of matches.
 */
int
findmatches(request,list)
char *request;
struct stacknode **list;
{
   int num_found=0;
   DIR *dirp;
   struct direct *direntry;
   char exact[40];
   char close[40];

   sprintf(exact,"%s.*",request); /* Set up patmatch string for exact match */
   sprintf(close,"%s*.*",request);  /* Set up patmatch string for close enough */

   dirp=opendir(".");

   while((direntry=readdir(dirp)) != NULL) {
      if ( direntry->d_name[0] != '.' ) {  /* Ignore . and .. */
         if ( patmatch(close,direntry->d_name,1) ) {  /* Does it match? */
	     if ( patmatch(exact,direntry->d_name,1) ) {  /* Exactly? */
	         dump(list);
	         push(list,direntry->d_name);  /* Yes, return 1-element list */
                 closedir(dirp);
	         return(1);  /* 1 exact match */
             } else {
                 push(list,direntry->d_name); /* Otherwise, add to list */
	         num_found++;
             }
         }
      }
   }
   closedir(dirp);
   return(num_found);
}
