/*
	$Id$
*/
#include "prep.h"
#include "eval.h"

/* Expression evaluator for cpp preprocessor:

	Operators (in descending order of precedence):

		()			parentheses
		! ~ -		logical negation, bitwise complement, arithmetic negation
		* / %		times, divide, remainder
		+ -			plus, minus
		<< >>		left shift, right shift
		< <= > >=	comparisons (except for ==, !=)
		== !=		equal, not equal
		&			bitwise and
		^			bitwise exclusive or
		|			bitwise inclusive or
		&&			McCarthy and (pseudo-Boolean)
		||			McCarthy inclusive or (pseudo-Boolean)
		?:			conditional expression

	Constant expressions forbid function calls, assignment operators,
	increment/decrement operators, and comma operators save as operands
	of sizeof (3.4).  The preprocessor further restricts its constant
	expressions, forbidding sizeof, casts, and enumeration constants,
	and requiring that it be an integral expression (3.8.1).  There
	is a "defined" "operator" which indicates whether an identifier
	is defined (so that theoretically #ifdef and #ifndef are superfluous).
*/

static char	badop[]		= "invalid operator in constant expression";
static char	badtyp[]	= "invalid type for constant expression";
static char	needexpr[]	= "missing operand";
static long	doit();		/* evaluate constant expressions */

int			gch();		/* snarfs text -- explicitly passed to scan() now */


int
eval(void)
{
	int		ok, uresult;
	long	result;
	char	*saveptr;
    char	pline[LINESIZE];	/* alleged expression with macros expanded */

	pptr = pline;
	scan(lptr, TRUE, gch);
	saveptr = lptr;
	lbase = lptr = pline;
	/*
	 * Once scan() is finished with the text, nothing looking like
	 * a function call can validly appear in the stuff to be evaluated...
	 */
	gch(0);
	getsym();
	ok = FALSE;
	if (sym == 0) {
		symptr = pline;	/* gotta make symptr plausible for error()! */
		error("constant expression expected");
	} else if (parsexp(&result, &uresult, 0)) {
		if (sym != 0)
			error("trailing text after constant expression");
		else
			ok = TRUE;
	}
	lptr = saveptr;
	symptr = lbase = line;
	return ok ? result : 0;
}


long
parsexp(resultp, uresultp, priority)
long	*resultp;		/* -> the result */
int		*uresultp;		/* -> unsignedness of result */
int		priority;
{
	long			rhs, lhs, tertius;
	int				urhs, ulhs, utert;
	register short	op, priop, rprec;

	if (!primary(&lhs, &ulhs)) {
		return FALSE;
	}

	while (isop() && priority <= symval) {
		op = sym;
		rprec = priop = (short) symval;
		getsym();

		switch (op) {
		default:
			if (op < ASSPLUS || op > ASSXOR) {
				rprec = priop + 1;
				break;
			}
		case ASSIGN:
			error(badop);	/* no assignment operators allowed! */
			return FALSE;
		case QUERY:
			;
		}

		if (parsexp(&rhs, &urhs, rprec)) {
			if (op == QUERY) {
				if (need(COLON)) {
					return FALSE;
				}
				if (!parsexp(&tertius, &utert, LEV_3)) {
					error("missing third operand for ?:");
					return FALSE;
				}
				if (lhs) {
					lhs = rhs;
					ulhs = urhs;
				} else {
					lhs = tertius;
					ulhs = utert;
				}
			} else {
				lhs = doit(op, ulhs = ulhs || urhs, lhs, rhs);
			}
		} else {
			error("missing operand");
			return FALSE;
		}
	}

	*resultp = lhs;
	*uresultp = ulhs;

	return TRUE;
}


int
chkpost()
{
	switch (sym) {
		case INCBEF:
		case DECBEF:
			error(badop);
			getsym();
			return FALSE;
	}
	return TRUE;
}


long
primary(resultp, uresultp)
long	*resultp;
int		*uresultp;
{
	long			nodep, prim;
	int				unodep, uprim;
	int				ok;
	register short	op;

	ok = FALSE;

	switch (sym) {
#ifdef FLOAT
	case DCONST:
#endif
	case STRING:
		error(badtyp);
		getsym();
		return FALSE;
	case NAME:
		error("warning: undefined name in preprocessor constant expression");
		ok = TRUE;	/* sort of... */
		nodep = 0;
		getsym();
		break;
	case CONST:
		unodep = FALSE;
		ok = TRUE;
		nodep = symval;
		getsym();
		break;
	case UCONST:
		unodep = TRUE;
		ok = TRUE;
		nodep = symval;
		getsym();
		break;
	case LPAREN:
		getsym();
		if (!parsexp(&nodep, &unodep, LEV_0)) {
noexp:
			error(needexpr);
			nodep = 0;
		}
		else if (!need(RPAREN))
			ok = TRUE;
		break;
	case NOT:
	case NEG:
	case COMPL:
		op = sym;
		getsym();
		if (primary(&prim, &uprim)) {
			nodep = doit(op, uprim, prim, 0);
			unodep = uprim;
			ok = TRUE;
		}
		break;
	case STAR:
	case DECBEF:
	case INCBEF:
	case AMPER:
		error(badop);
		getsym();
		break;
	}
	if (!ok)
		return FALSE;

	/* This handles an abberation in the syntax to allow
			foo++->bar;		formally it should be (foo++)->bar;
	*/
	ok = chkpost();

	switch (sym) {
	case LPAREN:
	case LBRACK:
	case DOT:
	case ARROW:
		ok = FALSE;
		error("invalid operation in preprocessor constant expression");
	}
	
	if (ok && chkpost()) {
		*resultp = nodep;
		*uresultp = unodep;
		return TRUE;
	}
	return FALSE;
}


int
isop()
{
	switch (sym) {
	case AMPER:
		sym = AND;
		symval = LEV_8;				/* "and" operator level */
		return TRUE;
	case STAR:
		sym = TIMES;
		symval = LEV_13;			/* multiplicative operator level */
		return TRUE;
	case NEG:
		sym = MINUS;
		symval = LEV_12;			/* additive operator level */
	case COMMA:
	case ASSIGN:
	case QUERY:
		return TRUE;
	case COLON:
		return FALSE;
	default:
		return (sym >= DBLAND && sym <= GT) ||
			   (sym >= ASSPLUS && sym <= ASSXOR);
	}
}

static long
doit(op, isunsigned, lhs, rhs)
int		op, isunsigned;
long	lhs, rhs;
{
	switch (op) {
	/* unary operators--ignore rhs */
	case NOT:
		return !lhs;
	case COMPL:
		return ~lhs;
	case NEG:
		return -lhs;
	/* binary operators */
	case PLUS:
		return lhs + rhs;
	case MINUS:
		return lhs - rhs;
	case TIMES:
		if (isunsigned)
			return (unsigned) lhs * (unsigned) rhs;
		else
			return lhs * rhs;
	case DIV:
		if (rhs == 0) {
			error("division by zero");
			return 0;
		}
		if (isunsigned)
			return (unsigned) lhs / (unsigned) rhs;
		else
			return lhs / rhs;
	case MOD:
		if (rhs == 0) {
			error("division by zero");
			return 0;
		}
		if (isunsigned)
			return (unsigned) lhs % (unsigned) rhs;
		else
			return lhs % rhs;
	case AND:
		return lhs & rhs;
	case OR:
		return lhs | rhs;
	case XOR:
		return lhs ^ rhs;
	case SHR:
		if (isunsigned)
			return (unsigned) lhs >> (unsigned) rhs;
		else
			return lhs >> rhs;
	case SHL:
		return lhs << rhs;
	case DBLAND:
		return lhs && rhs;
	case DBLOR:
		return lhs || rhs;
	case LT:
		if (isunsigned)
			return (unsigned) lhs < (unsigned) rhs;
		else
			return lhs < rhs;
	case LEQ:
		if (isunsigned)
			return (unsigned) lhs <= (unsigned) rhs;
		else
			return lhs <= rhs;
	case GT:
		if (isunsigned)
			return (unsigned) lhs > (unsigned) rhs;
		else
			return lhs > rhs;
	case GEQ:
		if (isunsigned)
			return (unsigned) lhs >= (unsigned) rhs;
		else
			return lhs >= rhs;
	case EQ:
		return lhs == rhs;
	case NEQ:
		return lhs != rhs;
	}
	error("invalid operator");
	return 0;
}

