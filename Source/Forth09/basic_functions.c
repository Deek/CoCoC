#define   SKIP_LINK      0    /* null value, indicating that 
                                 the file will not link     */
#include "../defs/forth.h"
#include "../defs/stack.h"
#include "../defs/token.h"
#include <stdio.h>

extern int push(), pop() ;

add()
{
     int t1,t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2+t1)) ;
}

subtract()
{
     int t1, t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2-t1)) ;
}

multiply()
{
     int t1, t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2*t1)) ;
}

divide()
{
     int t1, t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2/t1)) ;
}

modulus()
{
     int t1, t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2%t1)) ;
}

dot()
{
     int t1 ;

     if(pop(&t1) == ERROR) 
          return(ERROR) ;
     printf("%d ",t1) ;
     return(SUCCESS) ;
}

divmod()
{
     int t1, t2 ,t3;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     t3 = t2 % t1 ;
     push(t3) ;
     return(push(t2/t1)) ;
}

swap()
{
     int t1, t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     push(t1);
     push(t2);
     return(SUCCESS);
}

duplicate()
{
     int t1, t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     push(t1) ;
     push(t1) ;
     return(SUCCESS) ;
}

over()
{
     int t1,t2 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     push(t2);
     push(t1);
     push(t2);
     return(SUCCESS) ;
}

rot()
{
     int t1, t2, t3 ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     if (pop(&t2) == ERROR)
          return(ERROR) ;
     if (pop(&t3) == ERROR)
          return(ERROR) ;
     push(t2) ;
     push(t1) ;
     push(t3) ;
     return(SUCCESS) ;
}

drop()
{
     int t1 ;
     return(pop(&t1)) ;
}

cr()
{
     putchar('\n') ;
     return(SUCCESS) ;
}

spaces() 
{
     int t1 ;
     unsigned i ;

     if (pop(&t1) == ERROR)
          return(ERROR) ;
     for(i=0 ; i<(unsigned)(t1) ; i++) 
          putchar(' ') ;
     return(SUCCESS) ;
}

space() 
{
     push(1) ;
     return(spaces());
}

emit() 
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     putchar((char)(t1)) ;
     return(SUCCESS) ;
}

dswap()
{
     int t1,t2,t3,t4 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     if(pop(&t3) == ERROR)
          return(ERROR) ;
     if(pop(&t4) == ERROR)
          return(ERROR) ;
     push(t2) ;
     push(t1) ;
     push(t4) ;
     push(t3) ;
     return(SUCCESS) ;
}

ddup()
{
     int t1,t2,t3,t4 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     push(t1) ;
     push(t2) ;
     push(t1) ;
     push(t2) ;
     return(SUCCESS) ;
}

dover()
{
     int t1,t2,t3,t4 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     if(pop(&t3) == ERROR)
          return(ERROR) ;
     if(pop(&t4) == ERROR)
          return(ERROR) ;
     push(t3) ;
     push(t4) ;
     push(t1) ;
     push(t2) ;
     push(t3) ;
     push(t4) ;
     return(SUCCESS) ;
}

ddrop()
{
     int t1,t2,t3,t4 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     return(SUCCESS) ;
}

dot_quote(temp)
token_p *temp ;
{
     *temp = (*temp)->next_token ;
     printf("%s",(*temp)->token) ;
     return(SUCCESS) ;
}

equal() 
{
     int t1,t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2 == t1)) ;
}

less_than() 
{
     int t1,t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2 < t1)) ;
}

greater_than()
{
     int t1,t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2 > t1)) ;
}

zero_equal()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     return(push(t1 == 0)) ;
}

zero_less()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     return(push(t1 < 0)) ;
}

zero_greater()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     return(push(t1 > 0)) ;
}

not()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     return(push( ! t1)) ;
}

and()
{
     int t1, t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2 && t1)) ;
}

or()
{
     int t1, t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     return(push(t2 || t1)) ;
}

quest_dup()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if( t1 != 0 )
          if( push(t1) == ERROR) 
               return(ERROR) ;
     return(push(t1)) ;
}

abort_quote(temp)
token_p *temp ;
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     *temp = (*temp)->next_token ;
     if(t1) {
          fprintf(stderr,"%s",(*temp)->token) ;
          error(ABORT_CALL) ;
          return(ERROR);
     }
}

quest_stack()
{
     int t1 ;

     if(pop(&t1) == ERROR) 
          if(error_type == STACK_UNDERFLOW) 
               return(push(TRUE)) ;
          else
               return(ERROR) ;
     else {
          push(t1) ;
          push(0) ;
          return(SUCCESS) ;
     }
}

iffunct(temp)
token_p *temp ;
{
     int t1 ,ifcount ;

     ifcount = 1 ;
     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else 
          if(ifpush(t1) == ERROR)
               return(ERROR) ;
          else { 
               ifstruct.ifthen++ ;
               ifsp->elseflag = FALSE ;
               if(t1)
                    return(SUCCESS) ;
               else {
                    while((*temp)->next_token != 0 && ifcount > 0)
                         {
                              if ( ! strcmp((*temp)->next_token->token, "IF"))
                                   ifcount++ ;
                              else if ( ! strcmp((*temp)->next_token->token, "THEN"))
                                        ifcount-- ;
                                   else if( ! strcmp((*temp)->next_token->token,"ELSE")
                                             && ifcount == 1)
                                             return(SUCCESS) ;
                              *temp = (*temp)->next_token ;
                         }
                         if(ifcount != 0 || (*temp)->next_token == 0 &&
                              (strcmp((*temp)->token,"ELSE") && 
                              strcmp((*temp)->token,"THEN"))) {
                              error(ERROR_IN_IF) ;
                              return(ERROR) ;
                         }
                         else
                              return(SUCCESS) ;
                    }
               }
}

elsefunct(temp)
token_p *temp ;
{
     int t1 , ifcount ;

     ifcount = 1 ;
     if(!ifsp->elseflag) {
          ifsp->elseflag = TRUE ;
          if(ifpop(&t1) == ERROR)
               return(ERROR) ;
          else {
               if(ifpush(t1) == ERROR)
                    return(ERROR) ;
               else 
                    if(t1) {
                         while((*temp)->next_token != 0 && ifcount > 0)
                              {
                                   if ( ! strcmp((*temp)->next_token->token, "IF"))
                                        ifcount++ ;
                                   else if ( ! strcmp((*temp)->next_token->token, "THEN"))
                                             ifcount-- ;
                                   *temp = (*temp)->next_token ;
                              }
                         if(ifcount != 0 || ((*temp)->next_token == 0 
                              && strcmp((*temp)->token, "THEN"))) {
                              error(ERROR_IN_IF) ;
                              return(ERROR) ;
                         }
                         else
                              return(SUCCESS) ;
                    }
                    else
                         return(SUCCESS) ;
          }
     }
     else {
          error(UNCLOSED_IF) ;
          return(ERROR) ;
     }
}

thenfunct()
{
     int t1 ;

     if(ifstruct.ifthen = 0) {
          error(THEN_MISPLACED) ;
          return(ERROR);
     }
     else 
          if(ifpop(&t1) == ERROR)
               return(ERROR) ;
          else {
               ifstruct.ifthen-- ;
               }
}

inc()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else
          return(push((++t1))) ;
}

dec()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else 
          return(push((--t1))) ;
}

inc2()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else 
          return(push(t1+2)) ;
}

dec2()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else
          return(push(t1-2)) ;
}

left_shift()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else
          return(push((t1 << 1))) ;
}

right_shift()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else
          return(push((t1 >> 1))) ;
}

absfunct()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else 
               return(push(abs(t1))) ;
}

negate()
{
     int t1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     else
          return(push(-t1)) ;
}

min()
{
     int t1, t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;

     return(push((t2 < t1) ? t2 : t1)) ;
}

max()
{
     int t1, t2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;

     return(push((t2 > t1) ? t2 : t1)) ;
}

to_return()
{
     rstack_structure temp ;
     int i ;

     if(pop(&i) == ERROR)
          return(ERROR) ;
     temp.rvalue = i ;
     temp.looptype = VALUE ;
     return(rpush(&temp)) ;
}

from_return()
{
     rstack_structure temp ;

     if(rpop(&temp) == ERROR)
          return(ERROR) ;
     return(push(temp.rvalue)) ;
}

copy1_return()
{
     rstack_structure temp1 ;

     if(rpop(&temp1) == ERROR)
          return(ERROR) ;
     if(rpush(&temp1) == ERROR)
          return(ERROR) ;
     return(push(temp1.rvalue)) ;
}

copy2_return()
{
     rstack_structure temp1, temp2 ;

     if(rpop(&temp1) == ERROR)
          return(ERROR) ;
     if(rpop(&temp2) == ERROR)
          return(ERROR) ;
     if(rpush(&temp2) == ERROR)
          return(ERROR) ;
     if(rpush(&temp1) == ERROR)
          return(ERROR) ;
     return(push(temp2.rvalue)) ;
}

copy3_return()
{
     rstack_structure temp1, temp2, temp3 ;

     if(rpop(&temp1) == ERROR)
          return(ERROR) ;
     if(rpop(&temp2) == ERROR)
          return(ERROR) ;
     if(rpop(&temp3) == ERROR)
          return(ERROR) ;
     if(rpush(&temp3)== ERROR)
          return(ERROR) ;
     if(rpush(&temp2) == ERROR)
          return(ERROR) ;
     if(rpush(&temp1) == ERROR)  
          return(ERROR) ;
     return(push(temp3.rvalue)) ;
}

starslash()
{
     int t1, t2, t3 ;
     long temp1 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     if(pop(&t3) == ERROR)
          return(ERROR) ;

     temp1 = ((long) t3) * ((long) t2) / ((long) t1) ;
     return(push((int) temp1)) ;

}

starslmod()
{
     int t1, t2, t3 ;
     long temp1, temp2 ;

     if(pop(&t1) == ERROR)
          return(ERROR) ;
     if(pop(&t2) == ERROR)
          return(ERROR) ;
     if(pop(&t3) == ERROR)
          return(ERROR) ;

     temp2 = ((long) t3) * ((long) t2) % ((long) t1) ;
     if(push((int) temp2) == ERROR) 
          return(ERROR) ;
     temp1 = ((long) t3) * ((long) t2) / ((long) t1) ;
     return(push((int) temp1)) ;

}

do_funct(temp)
token_p *temp ;
{
     int index, limit ;
     rstack_structure a ;

     if(pop(&index) == ERROR)
          return(ERROR) ;
     if(pop(&limit) == ERROR)
          return(ERROR) ;
     if( limit < index)
          --limit ;

     a.rvalue = limit ;
     a.looptype = LIMIT_VALUE ;
     a.rtoken_adr = *temp ;
     if(rpush(&a) == ERROR)
          return(ERROR) ;
     a.rvalue = index ;
     a.looptype = INDEX_VALUE ;
     a.rtoken_adr = *temp ;
     return(rpush(&a)) ;
}

loop_funct(temp)
token_p *temp ;
{
     rstack_structure index, limit ;

     if(rpop(&index) == ERROR) 
          return(ERROR) ;
     if(rpop(&limit) == ERROR)
          return(ERROR) ;
     if( (++index.rvalue) < limit.rvalue) {
          *temp = limit.rtoken_adr ;
          if(rpush(&limit) == ERROR)
               return(ERROR) ;
          return(rpush(&index)) ;
     }
     return(SUCCESS) ;
}

loopinc_funct(temp)
token_p *temp ;
{
     rstack_structure index, limit ;
     int increment ;

     if(pop(&increment) == ERROR)
          return(ERROR) ;
     if(rpop(&index) == ERROR) 
          return(ERROR) ;
     if(rpop(&limit) == ERROR)
          return(ERROR) ;
     index.rvalue += increment ;
     if( abs(index.rvalue) < abs(limit.rvalue)) {
          *temp = limit.rtoken_adr ;
          if(rpush(&limit) == ERROR)
               return(ERROR) ;
          return(rpush(&index)) ;
     }
     return(SUCCESS) ;
}

leave_funct()
{
     rstack_structure index, limit ;
     
     if(rpop(&index) == ERROR)
          return(ERROR) ;
     if(rpop(&limit) ==  ERROR) 
          return(ERROR) ;
     limit.rvalue = index.rvalue ;
     if(rpush(&limit) == ERROR)
          return(ERROR) ;
     return(rpush(&index)) ;
}

begin_funct(temp)
token_p *temp ;
{
     rstack_structure a ;

     a.rvalue = 0 ;
     a.looptype = BEGIN_VALUE ;
     a.rtoken_adr = *temp ;
     return(rpush(&a)) ;
}

until_funct(temp) 
token_p *temp ;
{
     int i ;
     rstack_structure a ;

     if(rpop(&a) == ERROR)
          return(ERROR) ;
     if(a.looptype != BEGIN_VALUE)
          return(ERROR) ;
     if( pop(&i) == ERROR) 
          return(ERROR) ;
     if( i) 
          return(SUCCESS) ;
     else {
          *temp = a.rtoken_adr ;     
          return(rpush(&a)) ;
     }
}

while_funct(temp) 
token_p *temp ;
{
     int i , while_count = 1 ;

     if( pop(&i) == ERROR) 
          return(ERROR) ;
     if( i)  
          return(SUCCESS) ;
     else {
               while((*temp)->next_token != 0 && while_count > 0) {
                    if ( ! strcmp((*temp)->next_token->token, "WHILE"))
                         while_count++ ;
                    else if ( ! strcmp((*temp)->next_token->token, "REPEAT"))
                              while_count-- ;
                    *temp = (*temp)->next_token ;
               }
               if(while_count != 0 || (*temp)->next_token == 0 &&
                    (strcmp((*temp)->token,"REPEAT"))) { 
                         error(WHILE_ERROR) ;
                         return(ERROR) ;
               }
                    else
                         return(SUCCESS) ;
     }

}

repeat_funct(temp)
token_p *temp ;
{
     rstack_structure a ;

     if(rpop(&a) == ERROR) 
          return(ERROR) ;
     if(a.looptype != BEGIN_VALUE)
          return(ERROR) ;
     *temp = a.rtoken_adr ;
     return(rpush(&a)) ;
}

ur()
{
     char temp[12] ;
     int field,value ;

     if(pop(&field) == ERROR)
          return(ERROR) ;
     if(pop(&value) == ERROR)
          return(ERROR) ;
     sprintf(temp,"%%%uu ",field) ;
     printf(temp,value) ;
     return(SUCCESS) ;
}

page()
{
     putchar('\f') ;
}

quit()
{
     error(QUIT_CALL) ;
     return(ERROR) ;
}
