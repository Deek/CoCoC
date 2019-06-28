#ifndef FORTHDEF
#define   FORTHDEF       0    /* garbage value */
#define   ERROR          0    /* value returned on error */
#define   SUCCESS        -1   /* "      "       "  success */
#define   TRUE           -1   /* a true value */
#define   FALSE          0    /* a false value */
#define   BLOCK_NULL     'B'  /* Block null input on get_word */
#define   DONT_BLOCK_NULL 'D' /* Dont block null input on get_word */

/* error codes */
#define   CLEAR_ERROR         0    /* clear the error_type ; */
#define   STACK_OVERFLOW      1    /* unable to allocate for stack */
#define   STACK_UNDERFLOW     2    /* Unable to deallocate a pointer */
#define   MEMORY_UNAVAILABLE  3    /* a malloc, calloc failed */
#define   FREE_FAILURE        4    /* a free call failed */
#define   END_OF_INPUT        5    /* this indicates the end of input */
#define   END_OF_FILE         6    /* this indicates end of file, exit */
#define   NULL_INPUT          7    /* no token given */
#define   TOKEN_NOT_FOUND     8    /* It couldnt find the token */
#define   DEFINED_WORD        9    /* Indicates defined word, loop again */
#define   ABORT_CALL          10   /* Indicates a call to abort" */
#define   UNCLOSED_IF         11   /* Unclosed if statement */
#define   THEN_MISPLACED      12   /* Then misplaced */
#define   ERROR_IN_IF         13   /* Error in if then statement */
#define   WHILE_ERROR         14   /* While error */
#define   QUIT_CALL           15   /* Indicates a call to quit */

#ifndef SKIP_LINK   /* if not skipping link step, compile this code */

int error_type ;

error(error_code)
int error_code ;
{
     char *errmsg = "" ;

     switch(error_code) {

          case STACK_OVERFLOW:
               errmsg = "Stack overflow\n" ;
               break ;
          case STACK_UNDERFLOW:
               errmsg = "Stack underflow\n" ;
               break ;
          case MEMORY_UNAVAILABLE:
               errmsg = "Memory unavailable\n" ;
               break ;
          case FREE_FAILURE:
               errmsg = "Unable to return memory\n" ;
               break ;
/*        case END_OF_INPUT:
*              errmsg = "End of input reached\n" ;
*              break ;
*         case END_OF_FILE:
*              errmsg = "End of file reached\n" ;
*              break ;
*         case NULL_INPUT:
*              errmsg = "Null input\n" ;
*/             break ;
          case TOKEN_NOT_FOUND:
               errmsg = "Token not defined\n" ;
               break ;
          case DEFINED_WORD:
               errmsg = "End of word defintion\n" ;
               break ;
/*        case ABORT_CALL:
*              errmsg = "Abort\" called\n" ;
*/             break ;
          case UNCLOSED_IF:
               errmsg = "Unclosed if statement\n" ;
               break ;
          case THEN_MISPLACED:
               errmsg = "Misplaced then\n" ;
               break ;
          case WHILE_ERROR:
               errmsg = "While loop error\n" ;
               break ;
/*        case QUIT_CALL:
*              errmsg = "Quit called\n" ;
*/             break ;
    }

     if(strlen(errmsg) != NULL)
          fprintf(stderr,errmsg) ;
     error_type = error_code ;
}

#else

extern error_type ;
extern error() ;

#endif
#endif
