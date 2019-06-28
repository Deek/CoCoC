#ifndef COLON_DEFINITION
typedef struct token_struct {
               char *token ;
               struct token_struct *next_token ;
          } token_record, *token_p ;

#define COLON_DENINITION      ':' 
#define NOT_COLON_DEFINITION  ' '  
#define END_COLON_DEFINITION  ';'
#endif
