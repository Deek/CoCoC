#ifndef _ASSERT_H_
#define _ASSERT_H_
#ifndef NDEBUG
# define assert(ex) {if(!(ex)){fprintf(stderr,"Assertion 'ex' failed: file %s, line %d\n", __FILE__, __LINE__);abort();}}
#else
# define assert(ex) ;
#endif
#endif	/* _ASSERT_H_ */
