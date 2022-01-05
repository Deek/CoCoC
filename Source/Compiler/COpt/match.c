#include "op.h"
#include "ctype.h"

#ifndef _OS9
# include <fnmatch.h>
#endif
#include <stdint.h>

match (s, p)
char *s,*p;
{
	if (p == NULL) {
		return 1;
#ifdef _OS9
	} else if (patmatch (p, s, 0)) {
#else
	} else if (0 == fnmatch (p, s, FNM_NOESCAPE)) {
#endif
		return 1;
	}

	while (*p) {
		if (*p == '<') {
			if (isdigit (*s)) {
				unsigned n = 0;
				do {
					n = n * 10 + *s++ - '0';
				} while (isdigit (*s));

				if (n > 255)
					return 0;
			} else return 0;
			p++;
		} else if (*s++ != *p++)
			return 0;
	}
	return (*s == '\0');
}


static char *
mcpy (dp, pp, source)
char *dp, *pp, *source;
{
	register char *sp = source;
	int skip, count;

	skip = count = 0;
	for ( ; *pp; pp++) {
		if (*pp == *sp) {
			skip++;
			continue;
		} else if (*pp == '?') {
			count++;
			continue;
		} else if (*pp == '*') {
			pp++;
			for (sp = source+skip+count; *sp; sp++) {
				if (strcmp (pp, sp)) {	/* rest identical? */
					count++;
					continue;
				} else goto done;
			}
			continue;
		} else {
			skip++;
		}
	}
done:
	strncpy (dp, source+skip, count);
	dp += count;
	*dp = '\0';
}


matchcpy (driver, src1, pat1, src2, pat2)
char *driver, *src1, *pat1, *src2, *pat2;
{
	char tmp[ARGSIZE];
	register char *dp = driver;
	char *tp = tmp;

	*tmp = '\0';
	while (*dp) switch (*dp) {
		default:
			*tp++ = *dp++;
			break;
		case '\\':
			dp++;
			if (!*dp) {
				error ("invalid reference");
			} else if (*dp == '1') {
				dp++;	/* skip past the 1 */
				tp = mcpy (tp, pat1, src1);
			} else if (*dp == '2') {
				dp++;	/* skip past the 2 */
				tp = mcpy (tp, pat2, src2);
			}
	}
	if (strlen(tmp))
		strcpy (src1, tmp);
}
