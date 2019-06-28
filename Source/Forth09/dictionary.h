#ifndef DICTDEFS
#include "../DEFS/token.h"

#define DICTDEFS 1 /* dummy value indicating dictionary definitions are there */

typedef struct dict_struct {
                    char *word ;
                    struct dict_struct *dict_next ;
                    int code_type ;
                    union {
                         int (*adr)() ;
                         token_p token_list ;
                    } code_adr ;
               } dict_record, *dict_p ;

#define BASIC 0
#define DEFINED 1

#ifdef SKIP_LINK
dict_p dict_head ;
#else
extern dict_p dict_head ;
#endif

#define DICT_ENTRIES 64

#endif
