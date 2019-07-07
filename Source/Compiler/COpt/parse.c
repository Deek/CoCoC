#include "op.h"
#include "../rma/ctype.h"

parse(type,s,p,globflag)
int type,*globflag;
register char *s,*p;
{
    int size;
#ifdef DEBUG
    char *save = s;
#endif

    switch(type) {
        case LABEL:
            size = LABELSIZE;
            if (*p == ' ' || *p == '\t') break;
            *s++ = *p++;
            while (--size && isalnum(*p)) *s++ = *p++;
            while (isalnum(*p)) ++p;
            if (*globflag = *p == ':') ++p;
            break;
        case MNEM:
            size = MNEMSIZE;
            while (*p == ' ' || *p == '\t') ++p;
            while (size-- && isalnum(*p)) *s++ = *p++;
            while (isalnum(*p)) ++p;
            break;
        case ARGS:
            size = ARGSIZE;
            while (*p == ' ' || *p == '\t') ++p;
            while (size-- && *p) *s++ = *p++;
            break;
        default:
            error("parse called with bad type : %d",type);
    }

    *s = '\0';
#ifdef DEBUG
    switch(type) {
        case LABEL:
            debug("parsed label : \"%s\"\n", save);
            break;
        case MNEM:
            debug("parsed mnemonic : \"%s\"\n",save);
            break;
        case ARGS:
            debug("parsed args : \"%s\"\n",save);
    }
#endif
    return p;
}
