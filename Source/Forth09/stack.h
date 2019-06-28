#include "../DEFS/token.h"

typedef   struct stacks {
               int value ;
               int elseflag ; /* true if else encountered */
               struct stacks *next_stack ;
               struct stacks *prev_stack ;
          } stack_structure ;

typedef   struct rstacks {
               int rvalue, looptype ;
               token_p rtoken_adr ; /* adr of statement to return to */
               struct rstacks *rnext_stack ;
               struct rstacks *rprev_stack ;
          } rstack_structure ;

static    struct {
               int ifthen ; /* maintains count of ifthen stmts */
          } ifstruct = { 0 } ;

#define   VALUE          0
#define   INDEX_VALUE    1 
#define   LIMIT_VALUE    2
#define   BEGIN_VALUE    3
#define   BEGIN_UNTIL    4
#define   BEGIN_WHILE    5

#ifndef SKIP_LINK   /* if not skipping link step, do this */

stack_structure stack, *sp, ifstack, *ifsp ;
rstack_structure rstack,*rsp ;

#else

extern stack_structure stack, *sp, ifstack, *ifsp ;
extern rstack_structure rstack, *rsp ;

#endif
