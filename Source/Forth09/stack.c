
#include "../DEFS/stack.h"
#include "../DEFS/forth.h"

push(d)
int d ;
{
     char *temp;
     
     if ((temp=malloc(sizeof(stack_structure))) == 0) {
          error(STACK_OVERFLOW) ;
          return(ERROR) ;
     }

     sp->next_stack = temp ;
     temp->prev_stack = sp ;
     sp = temp ;
     sp->value = d ;
     return(SUCCESS) ;
}

pop(d)
int *d ;
{
     if (sp->prev_stack == 0) {
          error(STACK_UNDERFLOW) ;
          return(ERROR);
     }

     *d = sp->value ;
     sp = sp->prev_stack ;
     free(sp->next_stack) ;
     sp->next_stack = 0 ;
     return(SUCCESS) ;
}

init_stack() 

{
     sp = malloc(sizeof(stack_structure)) ;
     sp->prev_stack = 0 ;
     sp->next_stack = 0 ;
     sp->value = 0 ;
}

ifpush(d)
int d ;
{
     char *temp;
     
     if ((temp=malloc(sizeof(stack_structure))) == 0) {
          error(STACK_OVERFLOW) ;
          return(ERROR) ;
     }

     ifsp->next_stack = temp ;
     temp->prev_stack = ifsp ;
     ifsp = temp ;
     ifsp->value = d ;
     return(SUCCESS) ;
}

ifpop(d)
int *d ;
{
     if (ifsp->prev_stack == 0) {
          error(STACK_UNDERFLOW) ;
          return(ERROR);
     }

     *d = ifsp->value ;
     ifsp = ifsp->prev_stack ;
     free(ifsp->next_stack) ;
     ifsp->next_stack = 0 ;
     return(SUCCESS) ;
}

ifinit_stack() 

{
     ifsp = malloc(sizeof(stack_structure)) ;
     ifsp->prev_stack = 0 ;
     ifsp->next_stack = 0 ;
     ifsp->value = 0 ;
}

rpush(a)
rstack_structure *a ;
{
     char *temp;
     
     if ((temp=malloc(sizeof(rstack_structure))) == 0) {
          error(STACK_OVERFLOW) ;
          return(ERROR) ;
     }

     _strass(temp, a, sizeof(rstack_structure)) ;
     rsp->rnext_stack = temp ;
     temp->rprev_stack = rsp ;
     rsp = temp ;
     return(SUCCESS) ;
}

rpop(a)
rstack_structure *a ;
{
     if (rsp->rprev_stack == 0) {
          error(STACK_UNDERFLOW) ;
          return(ERROR);
     }

     _strass(a, rsp, sizeof(rstack_structure)) ;
     rsp = rsp->rprev_stack ;
     free(rsp->rnext_stack) ;
     rsp->rnext_stack = 0 ;
     return(SUCCESS) ;
}

rinit_stack() 
{
     rsp = malloc(sizeof(rstack_structure)) ;
     rsp->rprev_stack = 0 ;
     rsp->rnext_stack = 0 ;
     rsp->rvalue = 0 ;
     rsp->looptype = VALUE ;
     rsp->rtoken_adr = 0 ;
}
