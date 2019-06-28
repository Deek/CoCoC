#define   SKIP_LINK      1
#include <stdio.h>
#include <ctype.h>
#include "../DEFS/forth.h"
#include "../DEFS/dictionary.h"

execute(token_list,start_search)
token_p token_list ;
dict_p start_search ;
{
     token_p token ;

     token = token_list ;
     do {
          if (search_dictionary(&token,start_search) == ERROR) 
               if(error_type == ABORT_CALL || error_type == QUIT_CALL)
                    return(ERROR) ;
               else if(numbers_runner(token) == ERROR) {
                         error(TOKEN_NOT_FOUND) ;
                         fprintf(stderr,"%s not defined\n",token->token) ;
                         return(ERROR) ;
                    }
          token = token->next_token ;
     } while (token != 0) ;
     return(SUCCESS);
}

init_dict()
{
     int i, add(), subtract(), multiply(), divide(), modulus(), dot() ;
     int cr(), spaces(), space(), emit(), dswap(), ddup(), dover(), ddrop() ;
     int divmod(), swap(), duplicate(), over(), rot(), drop(), dot_quote();
     int equal(), less_than(), greater_than(), zero_equal(), zero_less_than() ;
     int zero_greater_than(), not(), and(), or(), quest_dup(), abort_quote() ;
     int quest_stack(), iffunct(), elsefunct(), thenfunct() ;
     int inc(), dec(), inc2(), dec2(), left_shift(), right_shift(), absfunct() ;
     int negate(), min(), max(), to_return(), from_return(), copy1_return() ;
     int copy2_return(), copy3_return(), starslash(), starslmod() ;
     int do_funct(), loop_funct(), loopinc_funct(), leave_funct() ;
     int begin_funct(), until_funct(), while_funct(), repeat_funct() ;
     int ur(), page(), quit() ;
     dict_p temp ;

     temp = 0 ;
     for(i=0 ; i < DICT_ENTRIES ; i++) {
          if((dict_head = malloc(sizeof(dict_record))) == ERROR) {
               error(MEMORY_UNAVAILABLE) ;
               return(ERROR) ;
          }
          dict_head->dict_next = temp ;
          dict_head->code_type = BASIC ;
          temp = dict_head ;
     }

     temp->word = "+" ;
     temp->code_adr.adr = add ;
     temp = temp->dict_next ;

     temp->word = "-" ;
     temp->code_adr.adr = subtract ;
     temp = temp->dict_next ;

     temp->word = "*" ;
     temp->code_adr.adr = multiply ;
     temp = temp->dict_next ;

     temp->word = "/" ;
     temp->code_adr.adr = divide ;
     temp = temp->dict_next ;

     temp->word = "MOD" ;
     temp->code_adr.adr = modulus ;
     temp = temp->dict_next ;

     temp->word = "." ;
     temp->code_adr.adr = dot ;
     temp = temp->dict_next ;

     temp->word = "/MOD" ;
     temp->code_adr.adr = divmod ;
     temp = temp->dict_next ;

     temp->word = "SWAP" ;
     temp->code_adr.adr = swap ;
     temp = temp->dict_next ;

     temp->word = "DUP" ;
     temp->code_adr.adr = duplicate ;
     temp = temp->dict_next ;

     temp->word = "OVER" ;
     temp->code_adr.adr = over ;
     temp = temp->dict_next ;

     temp->word = "ROT" ;
     temp->code_adr.adr = rot ;
     temp = temp->dict_next ;

     temp->word = "DROP" ;
     temp->code_adr.adr = drop ;
     temp = temp->dict_next ;

     temp->word = "CR" ;
     temp->code_adr.adr = cr ;
     temp = temp->dict_next ;

     temp->word = "SPACES" ;
     temp->code_adr.adr = spaces ;
     temp = temp->dict_next ;

     temp->word = "SPACE" ;
     temp->code_adr.adr = space ;
     temp = temp->dict_next ;

     temp->word = "EMIT" ;
     temp->code_adr.adr = emit ;
     temp = temp->dict_next ;

     temp->word = "2SWAP" ;
     temp->code_adr.adr = dswap ;
     temp = temp->dict_next ;

     temp->word = "2DUP" ;
     temp->code_adr.adr = ddup ;
     temp = temp->dict_next ;

     temp->word = "2OVER" ;
     temp->code_adr.adr = dover ;
     temp = temp->dict_next ;

     temp->word = "2DROP" ;
     temp->code_adr.adr = ddrop ;
     temp = temp->dict_next ;

     temp->word = ".\"" ;
     temp->code_adr.adr = dot_quote ;
     temp = temp->dict_next ;

     temp->word = "=" ;
     temp->code_adr.adr = equal ;
     temp = temp->dict_next ;

     temp->word = "<" ;
     temp->code_adr.adr = less_than ;
     temp = temp->dict_next ;

     temp->word = ">" ;
     temp->code_adr.adr = greater_than ;
     temp = temp->dict_next ;

     temp->word = "0=" ;
     temp->code_adr.adr = zero_equal ;
     temp = temp->dict_next ;

     temp->word = "0<" ;
     temp->code_adr.adr = zero_less ;
     temp = temp->dict_next ;

     temp->word = "0>" ;
     temp->code_adr.adr = zero_greater ;
     temp = temp->dict_next ;

     temp->word = "NOT" ;
     temp->code_adr.adr = not ;
     temp = temp->dict_next ;

     temp->word = "AND" ;
     temp->code_adr.adr = and ;
     temp = temp->dict_next ;

     temp->word = "OR" ;
     temp->code_adr.adr = or ;
     temp = temp->dict_next ;

     temp->word = "?DUP" ;
     temp->code_adr.adr = quest_dup ;
     temp = temp->dict_next ;

     temp->word = "ABORT\"" ;
     temp->code_adr.adr = abort_quote ;
     temp = temp->dict_next ;

     temp->word = "?STACK" ;
     temp->code_adr.adr = quest_stack ; 
     temp = temp->dict_next ;

     temp->word = "IF" ;
     temp->code_adr.adr = iffunct;
     temp = temp->dict_next ;

     temp->word = "ELSE" ;
     temp->code_adr.adr = elsefunct ;
     temp = temp->dict_next ;

     temp->word = "THEN" ;
     temp->code_adr.adr = thenfunct ;
     temp = temp->dict_next ;

     temp->word = "1+" ;
     temp->code_adr.adr = inc ;
     temp = temp->dict_next ;

     temp->word = "1-" ;
     temp->code_adr.adr = dec ;
     temp = temp->dict_next ;

     temp->word = "2+" ;
     temp->code_adr.adr = inc2 ;
     temp = temp->dict_next ;

     temp->word = "2-" ;
     temp->code_adr.adr = dec2 ;
     temp = temp->dict_next ;

     temp->word = "2*" ;
     temp->code_adr.adr = left_shift ;
     temp = temp->dict_next ;

     temp->word = "2/" ;
     temp->code_adr.adr = right_shift ;
     temp = temp->dict_next ;

     temp->word = "ABS" ;
     temp->code_adr.adr = absfunct ;
     temp = temp->dict_next ;

     temp->word = "NEGATE" ;
     temp->code_adr.adr = negate ;
     temp = temp->dict_next ;

     temp->word = "MIN" ;
     temp->code_adr.adr = min ;
     temp = temp->dict_next ;

     temp->word = "MAX" ;
     temp->code_adr.adr = max ;
     temp = temp->dict_next ;

     temp->word = ">R" ;
     temp->code_adr.adr = to_return ;
     temp = temp->dict_next ;

     temp->word = "R>" ;
     temp->code_adr.adr = from_return ;
     temp = temp->dict_next ;

     temp->word = "I" ;
     temp->code_adr.adr = copy1_return ;
     temp = temp->dict_next ;

     temp->word = "I'" ;
     temp->code_adr.adr = copy2_return ;
     temp = temp->dict_next ;

     temp->word = "J" ;
     temp->code_adr.adr = copy3_return ;
     temp = temp->dict_next ;

     temp->word = "*/" ;
     temp->code_adr.adr = starslash ;
     temp = temp->dict_next ;

     temp->word = "*/MOD" ;
     temp->code_adr.adr = starslmod ;
     temp = temp->dict_next ;

     temp->word = "DO" ;
     temp->code_adr.adr = do_funct ;
     temp = temp->dict_next ;

     temp->word = "LOOP" ;
     temp->code_adr.adr = loop_funct ;
     temp = temp->dict_next ;

     temp->word = "+LOOP" ;
     temp->code_adr.adr = loopinc_funct ;
     temp = temp->dict_next ;

     temp->word = "LEAVE" ;
     temp->code_adr.adr = leave_funct ;
     temp = temp->dict_next ;

     temp->word = "BEGIN" ;
     temp->code_adr.adr = begin_funct ;
     temp = temp->dict_next ;

     temp->word = "UNTIL" ;
     temp->code_adr.adr = until_funct ;
     temp = temp->dict_next ;

     temp->word = "WHILE" ;
     temp->code_adr.adr = while_funct ;
     temp = temp->dict_next ;

     temp->word = "REPEAT" ;
     temp->code_adr.adr = repeat_funct ;
     temp = temp->dict_next ;

     temp->word = "U.R" ;
     temp->code_adr.adr = ur ;
     temp = temp->dict_next ;

     temp->word = "PAGE" ;
     temp->code_adr.adr = page ;
     temp = temp->dict_next ;

     temp->word = "QUIT" ;
     temp->code_adr.adr = quit ;
     temp = temp->dict_next ;

     return(SUCCESS) ;
}

search_dictionary(token, start_search)
token_p *token ;
dict_p start_search ;
{
     dict_p temp ;

     temp = start_search ;
     do
          if(strcmp((*token)->token, temp->word) == 0) {
               if(temp->code_type == BASIC) {
                    if ((*(temp->code_adr.adr))(token) == SUCCESS)
                         return(SUCCESS) ; /* Execution successful */
                    else
                         if (error_type == ABORT_CALL 
                              || error_type == QUIT_CALL) 
                                   return(ERROR) ;
                         else
                              return(SUCCESS) ; /* It found it, but bombed */
               }
               else 
                    if (execute(temp->code_adr.token_list,temp->dict_next))
                         return(SUCCESS) ;
                    else 
                         return(ERROR) ;                    
          }
          else
               temp = temp->dict_next ;
     while(temp != 0) ;
     return(ERROR) ;
}

numbers_runner(token)
token_p token ;
{
     int i = 0 , sign = 1, numstart = 0;

     if(token->token[i] == '-') {
          sign = -1 ;
          i++ ;
          numstart++ ;
     }
     
     for(; isdigit(token->token[i]) ; i++) ;

     if((token->token[i] == '\0')
          || (sign == -1 && i>1 && token->token[i] == '\0'))
          return(push(sign * atoi((token->token)+numstart)));
     else
          return(ERROR) ;
}
