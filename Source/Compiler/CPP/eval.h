/*
	@(#)eval.h	2.2.1	6/15/87
*/
/* expression priority levels */
#define LEV_15		15
#define LEV_14		14
#define LEV_13		13
#define LEV_12		12
#define LEV_11		11
#define LEV_10		10
#define LEV_9		9
#define LEV_8		8
#define LEV_7		7
#define LEV_6		6
#define LEV_5		5
#define LEV_4		4
#define LEV_3		3
#define LEV_2		2
#define LEV_1		1
#define LEV_0		0

/* operator tokens */
#define	INT		1
#define UNSIGN	2

#define    SEMICOL 10
#define    LBRACE  11
#define    RBRACE  12
#define    LBRACK  13
#define    RBRACK  14
#define    LPAREN  15
#define    RPAREN  16
#define    COLON   17
#define    COMMA   18
#define    KEYWORD 19
#define    NAME    20
#define    CONST   21
#define    UCONST  22
#define    STRING  23

#define    INCBEF  25
#define    DECBEF  26
#define    INCAFT  27
#define    DECAFT  28
#define    NOT     29
#define    AMPER   30
#define    STAR    31
#define    NEG     32
#define    COMPL   33
#define    DOT     34
#define    ARROW   35
#define    DBLAND  36
#define    DBLOR   37
#define    PLUS    40
#define    MINUS   41
#define    TIMES   42
#define    DIV     43
#define    MOD     44
#define    SHR     45
#define    SHL     46
#define    AND     47
#define    OR      48
#define    XOR     49

/* relational operators */
#define    EQ      50
#define    NEQ     51
#define    LEQ     52
#define    LT      53
#define    GEQ     54
#define    GT      55
#define    ULEQ    56   /* unsigned */
#define    ULT     57   /* unsigned */
#define    UGEQ    58   /* unsigned */
#define    UGT     59   /* unsigned */

#define    ASSIGN  60  /* assignment (=) */

#define    QUERY   61
#define    BSLASH  62
#define    PRIME   63
#define    QUOTE   64
#define    LETTER  65
#define    DIGIT   66
#define    SPACE   67

/* assignment operators */
#define   ASSPLUS   80
#define   ASSMIN    81
#define   ASSMUL    82
#define   ASSDIV    83
#define   ASSMOD    84
#define   ASSSHR    85
#define   ASSSHL    86
#define   ASSAND    87
#define   ASSOR     88
#define   ASSXOR    89

/* useful values */
#define			YES				1
#define			NO				0
#define         ERR             (~0)
#define         BACKSPACE       8
#define         TAB_CHAR        9
#define         LINEFEED        10
#define         VTAB_CHAR       11
#define         FORMFEED        12
#define         RET_CHAR        13
#define			NEWLINE			13

