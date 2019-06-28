#include <lowio.h>
#include <sgstat.h>

main()
{
  char key;
  
  printf("KeyCode by TRIXsoft\n");
  printf("Press [ENTER] to end.\n");
  while((key = getk()) != '\r') {
    printf("Key code = %d\n",key);
  };
  
  
}

getk()
{
  char inkey();
  int key;
  char test;
  while((key = inkey(STDIN)) == '\0') ;
  return(key);
}

/****************************************************************************
 * inkey(pn) - Checks for key pressed on pn.  Returns key or 0 if none.     *
 * Entry   : int pn - Path Number to poll                                   *
 * Exit    : character recieved or 0 if none                                *
 ****************************************************************************/
char inkey(pn)
int pn;
{
  struct sgbuf pd;
  char  echo,ch;
  int toreturn;

  _gs_opt(pn,&pd);
  echo = pd.sg_echo;
  pd.sg_echo = 0;
  _ss_opt(pn,&pd);
  
  if(_gs_rdy(pn) > 0)  {
    read(pn,&ch,1);
  } else {
    ch = '\0';
  }

  pd.sg_echo = echo;
  _ss_opt(pn,&pd);

  return(ch);
} 
