#undef assert	/* ensure it's not defined */
#ifndef NDEBUG
# if defined(__STDC__) || defined(__DCC__)
#  define assert(ex) {if(!(ex)){fprintf(stderr,"%s:%d: Assertion '%s' failed\n", __FILE__, __LINE__, #ex);abort();}}
# else
#  define assert(ex) {if(!(ex)){fprintf(stderr,"%s:%d: Assertion 'ex' failed\n", __FILE__, __LINE__);abort();}}
# endif
#else
# define assert(ex) (0)
#endif
