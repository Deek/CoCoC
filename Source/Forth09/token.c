
#include "../DEFS/token.h"
#include "../DEFS/dictionary.h"

get_word(token_pointer,null_block)
char **token_pointer ;
char null_block ;
{
     char temp[100] ;
     char c ;
     unsigned int i=0 ;

     *token_pointer = 0 ;
     do {                          /* skip leading whitespace */
          c = getchar() ;
          if(c == '\n' && null_block == DONT_BLOCK_NULL) {
                    error(NULL_INPUT);
                    return(ERROR) ;
               }
     } while (isspace(c)) ;

     if (c != -1) 
          do {
               temp[i++] = c ; 
               c = getchar() ;
          } while ( c != '\n' && ! isspace(c) && i<100 && c != -1) ;
     else {
          error(END_OF_FILE) ;
          return(ERROR);
     } 

     temp[i] = '\0' ;

     if ( c == -1) {
          error(END_OF_FILE) ;
          return(ERROR) ;
     }
     else if (i == 0) {
               error(END_OF_INPUT) ;
               return(ERROR);
          }
          else 
               if ((*token_pointer=calloc(i,sizeof(char))) != 0 ) {
                    strcpy(*token_pointer,temp) ;
                    if (c == '\n') {
                         error(END_OF_INPUT) ;
                         return(ERROR);
                    }
                    else
                         return(SUCCESS) ; 
               }
               else {
                    error(MEMORY_UNAVAILABLE);
                    return(ERROR);
               }
}

get_tokens(head,colon_flag) 
token_p *head ;
char colon_flag ;
{
     token_p point, temp ;
     char block_flag ;
     char *semicolon, *colon, *dotquote, *abortquote ;

     block_flag = BLOCK_NULL ;
     *head = 0 ;
     semicolon = ";" ;
     colon = ":" ;
     dotquote = ".\"" ;
     abortquote = "ABORT\"" ;
     while (TRUE) {
          if ((temp = malloc(sizeof(token_record))) == 0) {
               error(MEMORY_UNAVAILABLE) ;
               return(ERROR) ;
          }
          temp->next_token = 0 ;
          if (get_word(&(temp->token),block_flag) == SUCCESS 
               || error_type == END_OF_INPUT) {
                    if ( ! strcmp(temp->token,colon) 
                         && colon_flag != COLON_DEFINITION) {
                              free(temp->token) ;
                              free(temp) ;
                              return(define_word()) ;
                    }
                    if (colon_flag == COLON_DEFINITION)
                         if ( ! strcmp(temp->token,semicolon)) {
                              free(temp->token) ;
                              free(temp) ;
                              return(SUCCESS) ;
                         }
                    if (*head == 0) {
                         *head = temp ;
                         point = temp ;
                         if(colon_flag != COLON_DEFINITION)
                              block_flag = DONT_BLOCK_NULL ;
                    }
                    else {
                         point->next_token = temp ;
                         point = temp ;
                         point->next_token = 0 ;
                    }
                    if ( ! strcmp(point->token,dotquote) 
                         || ! strcmp(point->token,abortquote)) {
                            if (( temp = malloc(sizeof(token_record))) == 0) {
                                   error(MEMORY_UNAVAILABLE) ;
                                   return(ERROR) ;
                              }
                              if (define_string(&(temp->token)) == ERROR) {
                                   free(temp) ;
                                   return(ERROR) ;
                              }
                              else {
                                   point->next_token = temp ;
                                   point = temp ;
                                   point->next_token = 0 ;
                              }
                    }
                    if( error_type == END_OF_INPUT 
                         && colon_flag != COLON_DEFINITION)
                              return(SUCCESS) ;
          }
          else {
                    if(temp->token != 0) 
                         free(temp->token) ;
                    free(temp) ;
                    if(error_type == NULL_INPUT)
                         if(colon_flag == COLON_DEFINITION)
                              continue ;
                         else
                              return(SUCCESS) ;
                    else
                         return(ERROR) ;
          }
     }
}

deallocate(token_pointer)
token_p token_pointer ;
{
     if(token_pointer->next_token == 0) {
          free(token_pointer->token);
          free(token_pointer) ;
          return(SUCCESS) ;
     }
     else {
          deallocate(token_pointer->next_token) ;
          free(token_pointer->token) ;
          free(token_pointer) ;
          return(SUCCESS) ;
     }
}
 
define_word() 
{
     dict_p temp_dict ;

     if((temp_dict = malloc(sizeof(dict_record))) == ERROR) {
          error(MEMORY_UNAVAILABLE) ;
          return(ERROR) ;
     }
     
     temp_dict->dict_next = dict_head ;
     dict_head = temp_dict ;
     dict_head->code_type = DEFINED ;

     if(get_word(&(dict_head->word),BLOCK_NULL) == ERROR && error_type != END_OF_INPUT)
          return(ERROR) ;

     if(get_tokens(&(dict_head->code_adr.token_list),COLON_DEFINITION) == ERROR)
          if(error_type == END_OF_FILE)
               exit(0) ;
          else {
               if(dict_head->code_adr.token_list != 0) 
                    deallocate(dict_head->code_adr.token_list) ;
               else ; /* null else */
               free(dict_head->word) ;
               temp_dict = dict_head ;
               dict_head = dict_head->dict_next ;
               free(temp_dict) ;
               return(ERROR) ;
          }
     else {
          error(DEFINED_WORD) ;
          return(ERROR) ;
     }
}

define_string(token_pointer) 
char **token_pointer ;
{
     char temp[256] ;
     char c;
     unsigned int i=0 ;

     while((c=getchar()) != '"') 
          temp[i++] = c ;
     temp[i] = '\0' ;

     if((*token_pointer = calloc(i,sizeof(char))) != 0) {
          strcpy(*token_pointer,temp) ;
          return(SUCCESS) ;
     }
     else
          return(ERROR) ;          
}
