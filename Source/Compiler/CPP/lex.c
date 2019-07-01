/*
	@(#)lex.c	2.2.1	6/15/87
*/
/*
 * borrowed lex.c from c68 to help us parse constant expressions for
 * #if, #elif
 *
 * ALL of this code is appropriated for use solely with input that has
 * been fed through scan() so that it's guaranteed free of macros needing
 * expansion.
 *
 * You'll note that this has much stuff ripped out of it.  We don't need
 * no steenkin' symbol table--it takes up too much room, and all we need
 * to know is that we've found a name that made it past scan() so it's
 * not a macro.  We don't need no steenkin' string data file--strings
 * don't have any place in preprocessor constant expressions, so all we
 * need to know is that it is a string so we can complain about its being
 * there.  We don't need no steenkin' double-precision constants--we need
 * to *recognize* them, but we don't care about their value, since all
 * we're going to do is complain about them.
 */

#include "prep.h"
#include "eval.h"

#define ISDIGIT(c)  (chartab[(c)] == DIGIT)

#ifdef KANJI
extern char kanjtbl[];
#define iskanji(c)	(kanjtbl[(c)] != 0)
#endif


char chartab[] = {
	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	/* 00-07 */
	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	/* 08-0f */
	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	/* 10-17 */
	0, 	0, 	0, 	0, 	0, 	0, 	0, 	0, 	/* 18-1f */
	SPACE, 	NOT, 	QUOTE, 	0, 	0, 	MOD, 	AMPER, 	PRIME, 	/* 20-27 */
	LPAREN,	RPAREN,	STAR, 	PLUS, 	COMMA, 	NEG, 	DOT, 	DIV, 	/* 28-2f */
	DIGIT, 	DIGIT, 	DIGIT, 	DIGIT, 	DIGIT, 	DIGIT, 	DIGIT, 	DIGIT, 	/* 30-37 */
	DIGIT, 	DIGIT, 	COLON, 	SEMICOL, LT, 	ASSIGN, GT, 	QUERY, 	/* 38-3f */
	0, 	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, /* 40-47 */
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, /* 48-4f */
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, /* 50-57 */
	LETTER, LETTER, LETTER, LBRACK, BSLASH, RBRACK, XOR, 	LETTER, /* 58-5f */
	0, 	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, /* 60-67 */
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, /* 68-6f */
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, /* 70-77 */
	LETTER, LETTER, LETTER, LBRACE, OR,		RBRACE, COMPL, 	0, 	/* 78-7f */
};

char valtab[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, LEV_9, 0, 0, 0, LEV_13, LEV_14, 0, 		/* 9=NOT, 13=MOD, 14=AMPER */
	0, 0, LEV_14, LEV_12, LEV_1, 			/* 14=STAR, 12=PLUS, 1=COMMA */
	LEV_14, LEV_15, LEV_13, 				/* 14=NEG, 15=DOT, 13=DIV */
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, LEV_3, 0, LEV_10, LEV_2, LEV_10, LEV_3, /* 3=COLON, 10=LT, 2=ASSIGN, 10=GT, 3=QUERY */
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, LEV_7, 0, 				/* 7=XOR */
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, LEV_6, 0, LEV_14, 0			/* 6=OR, 14=COMPL */
};

#ifdef KANJI
char kanjtbl[] = {						/* kanji first byte table */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 00 - 0f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 10 - 1f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 20 - 2f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 30 - 3f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 40 - 4f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 50 - 5f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 60 - 6f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* 70 - 7f */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 	/* 80 - 8f */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 	/* 90 - 9f */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* a0 - af */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* b0 - bf */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* c0 - cf */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 	/* d0 - df */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 	/* e0 - ef */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 	/* f0 - ff */
};
#endif

static char namebuf[NAMESIZE + 1];

static
dobslash();

void getsym(void)
{
	register int		numtype;
	auto long			n;

	while (isspace(cch))
	{
		gch(1);
	}
	if (cch == 0)
	{
		sym = 0; 
		return; 
	}

	symptr = lptr - 1;
	while (!(sym = chartab[cch]))
	{
		error("invalid character in constant expression");
		gch(1);
		symptr = lptr;
	}
	symval = (long) valtab[cch];		/* operator character's priority */

	switch(sym)
	{
	case LETTER:
		getword(namebuf, NAMESIZE);
		sym = NAME;
		symval = 0;
		break;
	case DIGIT:
		numtype = number(&n);
donum:   
		switch(numtype)
		{
		case INT:
			symval = n;
			sym = CONST;
			break;
		case UNSIGN:
			symval = n;
			sym = UCONST;
			break;
		default:        
			error("illegal number");
			symval = 1;
			sym = CONST;
			break;
		}
		break;

	case PRIME: 
		pstr(); 
		sym = CONST; 
		break;
	case QUOTE: 
		qstr(); 
		sym = STRING; 
		break;
	default :
		gch(1);
		switch(sym)
		{
		case AMPER :
			switch(cch)
			{
			case '&': 
				sym = DBLAND; 
				gch(1); 
				symval = LEV_5; 			/* boolean "and" level */
				break;
			case '=': 
				sym = ASSAND; 
				symval = LEV_2; 			/* assignment operator level */
				gch(1);
			} 
			break;
		case ASSIGN : 
			if (cch == '=')
			{
				sym = EQ;
				symval = LEV_9;			/* boolean "equality" level */
				gch(1);
			} 
			break;
		case OR :
			switch (cch)
			{
			case '|': 
				sym = DBLOR; 
				gch(1); 
				symval = LEV_4; 			/* boolean "or" level */
				break;
			case '=': 
				sym = ASSOR;
				gch(1); 
				symval = LEV_2;			/* assignment operator level */
			} 
			break;
		case NOT : 
			if (cch == '=')
			{
				sym = NEQ;
				gch(1);
				symval = LEV_9;			/* boolean "equality" level */
			} 
			break;
		case STAR: 
			if (cch == '=')
			{
				sym = ASSMUL;
				gch(1);
				symval = LEV_2;			/* assignment operator level */
			} 
			break;
		case DIV: 
		case MOD:
		case XOR:  
			if (cch == '=')
			{
				sym = sym + (ASSPLUS - PLUS);
				gch(1);
				symval = LEV_2;			/* assignment operator level */
			} 
			break;
		case LT : 
			switch (cch)
			{
			case '<' : 
				sym = SHL;
				symval = LEV_11;			/* shift operator level */
				gch(1);
				if (cch == '=')
				{
					sym = ASSSHL; 
					symval = LEV_2; 
					gch(1);
				}
				break;
			case '=' : 
				sym = LEQ;
				symval = LEV_10;			/* boolean "relational" level */
				gch(1);
			} 
			break;
		case GT : 
			switch (cch)
			{
			case '>' : 
				sym = SHR;
				symval = LEV_11;			/* shift operator level */
				gch(1);
				if (cch == '=')
				{
					sym = ASSSHR;
					symval = LEV_2; 		/* assignment operator level */
					gch(1);
				}
				break;
			case '=' : 
				sym = GEQ;
				symval = LEV_10;			/* boolean "relational" level */
				gch(1);
			} 
			break;
		case PLUS :
			switch (cch)
			{
			case '+': 
				sym = INCBEF; 
				symval = LEV_14; 		/* high level unary operator */
				gch(1); 
				break;
			case '=': 
				sym = ASSPLUS;
				symval = LEV_2; 			/* assignment operator level */
				gch(1);
			}
			break;
		case NEG :
			switch (cch)
			{
			case '-': 
				sym = DECBEF; 
				symval = LEV_14; 		/* high level unary operator */
				gch(1); 
				break;
			case '=': 
				sym = ASSMIN; 
				symval = LEV_2; 			/* assignment operator level */
				gch(1);  
				break;
			case '>': 
				sym = ARROW; 
				symval = LEV_15; 		/* top level binding */
				gch(1);
			}
			break;
		}
	}
	return;
}

int number(long *np)
{
	auto long		n;
	register char	*cp;
	register short	nunsigned = FALSE, nlong = FALSE;
	
	n = 0;
	cp = (char*) &n;

	if (cch == '0')
	{
		gch(1);
		if (cch == 'x' || cch == 'X'){
			int c;
			gch(1);
			while (c = ishex(cch)) {
				n = (n << 4) + (c - (c >= 'A' ? 'A' - 10 : '0'));
				gch(1);
			}
		}
		else
		{
			while (isoct(cch)) {
				n = (n << 3) + (cch - '0');
				gch(1);
			}
		}
		goto intsuffix;
	}

	/* here all types still possible; collect in 4 byte integer */
	while (ISDIGIT(cch))
	{
		n = (n * 10) + (cch - '0');
		gch(1);
	}

intsuffix:
	for (; ; gch(1))
	{
		switch (cch)
		{
		case 'l':
		case 'L':
			if (nlong)
				break;
			nlong = TRUE;
			continue;
		case 'u':
		case 'U':
			if (nunsigned)
				break;
			nunsigned = TRUE;
			continue;
		case '.':
			return 0;
		}
		break;
	}

	*np = n;
	return nunsigned ? UNSIGN : INT;
}

#if 0
static direct FILE	*sfile;
#endif
direct int			stringlen;           /* for inits */

void pstr(void)
{
	gch(0);
	if (cch != '\\')
	{
		symval = cch;
		gch(0);
	}
	else
	{
		symval = dobslash();
	}

	if (cch == '\'')
	{
		gch(1);
	}
	else
	{
		error("bad character constant");
	}
}


void qstr(void)
{
#ifdef KANJI
	static int kanji_flg;
#endif

	stringlen = 0;
	gch(0);

	while (cch != '"')
	{
		if ((lptr - lbase) == 0)
		{
			error("bad string constant");
			break;
		}

#ifdef KANJI
		if (kanji_flg)
		{
			kanji_flg = 0;
			gch(0);
		}
		else if (cch != '\\')
		{
			kanji_flg = iskanji(cch & 0xff);
			gch(0);
		}
#else
		if (cch != '\\')
		{
			gch(0);
		}
#endif
	}
	gch(1);
}


static int dobslash(void)
{
	register int	c, n;

	gch(1);
	c = cch;
	gch(1);

	switch(c)
	{
	case 'n':   return NEWLINE;
	case 'l':   return LINEFEED;
	case 't':   return TAB_CHAR;
	case 'b':   return BACKSPACE;
	case 'v':   return VTAB_CHAR;
	case 'r':   return RET_CHAR;
	case 'f':   return FORMFEED;
	case '\n':  return ' ';
	}
	if (c == 'x')
	{
		int c1;

		c = n = 0;
		while ((c1 = ishex(cch)) && n++ < 2)
		{
			c = (c << 4) + (c1 < 'A' ? c1 - '0' : c1 - 'A' + 10);
			gch(1);
		}
	}
	else if (c == 'd')
	{
		c = n = 0;
		while (ISDIGIT(cch) && n++ < 3)
		{
			c= c * 10 + cch - '0';
			gch(1);
		}
	}
	else if (isoct(c))
	{
		c -= '0';
		n = 0;
		while (isoct(cch) && n++ < 2)
		{
			c = (c << 3) + (cch - '0');
			gch(1);
		}
	}
	return c;
}


int isoct(int c)
{
	return (c <= '7' && c >= '0');
}


int ishex(int c)
{
	return (ISDIGIT(c) || ((c &= 0x5f) >='A' && c <= 'F')) ? c : 0;
}


int an(int c)
{
	return (chartab[c] == LETTER || chartab[c] == DIGIT);
}


int need(char key)
{
	register int i;
	static char	exptmsg[] = "expected X";

	if (sym == key)
	{
		getsym();
		return 0;
	}
	for (i = 0; i < 128; ++i)
	{
		if (chartab[i] == key)
		{
			break;
		}
	}
	exptmsg[sizeof(exptmsg) - 2] = i;
	error(exptmsg);

	return 1;
}
