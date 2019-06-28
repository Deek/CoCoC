#include <stdio.h>
#include <ctype.h>
#include "../defs/forth.h"
#include "../defs/dictionary.h"
#include "stack.c"
#include "token.c"

main()
{
     token_p head ;

     setbuf(stdout,NULL) ;

     init_stack() ; ifinit_stack() ;  rinit_stack() ;
     init_dict() ;

     printf("FORTH09 v 01.00.00 1987\n") ;
     printf("by Dennis M. Weldy (OS9ER)\n") ;
     printf(" <PRELIMINARY VERSION!> \n") ;
     printf("\n") ;

     while(TRUE) {

          if(error_type != QUIT_CALL)
               printf("ok\n") ;
          else
               putchar('\n') ;

          error(CLEAR_ERROR) ;
          while ((get_tokens(&head,NOT_COLON_DEFINITION)) == ERROR)
               if(error_type == END_OF_FILE)
                    exit(0);
               else 
                    if (error_type != DEFINED_WORD) {
                         fprintf(stderr,"get tokens bombed\n");
                         break ;
                    }

          if ((execute(head,dict_head)) == ERROR)
               if(error_type != ABORT_CALL && error_type != QUIT_CALL) {
                    fprintf(stderr,"execute bombed\n");
                    deallocate(head) ;
               }
               else
                    deallocate(head) ;
          else
               deallocate(head) ;
     
     }
}
