extern  char    _chcodes2[];      /* in chcodes.r */

#define _DOLLAR     0x01
#define _UPPER      0x02
#define _LOWER      0x04
#define _DIGIT      0x08
#define _BINDIG     0x10
#define _HEXDIG     0x20

#define isascii(c)  ((unsigned)(c)<=0x7f)
#define isalpha(c)  (_chcodes2[c]&(_UPPER|_LOWER))
#define isupper(c)  (_chcodes2[c]&_UPPER)
#define islower(c)  (_chcodes2[c]&_LOWER)
#define isdigit(c)  (_chcodes2[c]&_DIGIT)
#define isxdigit(c) (_chcodes2[c]&_HEXDIG)
#define isalnum(c)  (_chcodes2[c] > 0)
#define iscntrl(c)  (_chcodes2[c]&_DOLLAR)
#define isgraph(c)  (isprint(c)&&c!=' ')
#define _toupper(c) ((c)&0xdf)
#define _tolower(c) ((c)|0x20)
#define toascii(c)  ((c)&0x7f)
