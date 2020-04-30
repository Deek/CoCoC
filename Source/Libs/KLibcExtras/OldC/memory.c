/*
** Memory allocation routines for the OS9 C compiler
**
**  quite a bit like K&R, pp. 175-177
**  basically a tag allocation scheme using next fit algo
**
**  Difference is that in our case alignment is not
**   a problem.
**
**  Management is in units the size of the tag structure.
**   The number of bytes requested is converted into a quantity
**   of units to allocate by division, rounded up, and one more
**   added for the tag record.  The returned pointer is a byte
**   pointer pointing to the first unit past the tag record.
**
**  Malloc(count) is the normal entry point, where count is in
**   bytes.  It returns a pointer to space for 'count' bytes.
**  Calloc(count, size) is slightly more sophisticated, returning
**   space for 'count' elements of size 'size', which is initialized
**   to 0.
**  Free(ptr) returns a block of storage as allocated by one of the
**   above.  If the returned block has an adjacent neighbor, they
**   are combined.  Calling free() with a bad pointer is bad for the
**   system's health.
*/


typedef  struct   header {
         struct header   *ptr;
         unsigned size;
         } HEADER;

static   HEADER   base;
static   HEADER   *allocp = (HEADER *) 0;

/*page*/
/*
** get memory from the system, in NALLOC quantities of units
*/

#define  NALLOC   256

static   HEADER   *
morecore(nu)
unsigned nu;                                     /* number of units */
   {
   extern   char  *sbrk();
   register char  *cp;
   HEADER         *up;
   int            rnu;

   rnu = NALLOC * ((nu + NALLOC - 1) / NALLOC);         /* how much */
   cp = sbrk(rnu * sizeof(HEADER));           /* get it from system */
   if ((int)cp == -1)
      return (0);                              /* return 0 on error */
   up = (HEADER *) cp;             /* make unit ptr out of char ptr */
   up->size = rnu;                 /* set in size of block in units */
   free((char *) (up + 1));              /* link into existing list */
   return (allocp);
   }


char  *
malloc(nbytes)
unsigned nbytes;
   {
   HEADER            *morecore();
   register HEADER   *p, *q, *junk;
   int               nunits;

   nunits = 1 + (nbytes + sizeof(HEADER) - 1) / sizeof(HEADER);
   if ((q = allocp) == 0)                    /* if no free list yet */
      {
      base.ptr = allocp = q = &base;      /* create empty free list */
      base.size = 0;
      }
   for (p = q->ptr; ; q = p, p = p->ptr)      /* scan the free list */
      {
      if (p->size >= nunits)                         /* big enough? */
         {
         if (p->size == nunits)                          /* exactly */
            q->ptr = p->ptr;
         else
            {                               /* allocate on tail end */
            p->size -= nunits;
            p += p->size;
            p->size = nunits;
            }
         allocp = q;
         return ((char *) (p + 1));
         }
      if (p == allocp)                   /* worked around free list */
         if ((p = morecore(nunits)) == 0)
            return (0);                    /* ouch - no memory left */
      }
   }
/*page*/
free(ap)
char  *ap;
   {
   register HEADER   *p,*q;

   p = (HEADER *) ap - 1;            /* get a pointer to tag record */
   for (q = allocp; !(p > q && p < q->ptr); q = q->ptr) /* find loc */
      if (q >= q->ptr && (p > q || p < q->ptr))
         break;                              /* at one end or other */

   if (p + p->size == q->ptr)             /* join to upper neighbor */
      {
      p->size += q->ptr->size;
      p->ptr = q->ptr->ptr;
      }
   else
      p->ptr = q->ptr;

   if (q + q->size == p)                  /* join to lower neighbor */
      {
      q->size += p->size;
      q->ptr = p->ptr;
      }
   else
      q->ptr = p;

   allocp = q;
   }


char  *
calloc(count, size)
unsigned count, size;
   {
   char           *malloc();
   register char  *p;
   unsigned       i;

   p = malloc(i = count * size);
   if (p)
      memset(p, i);
   return(p);
   }


static
memset(p, n)
register char  *p;
int   n;
   {
   while (n--)
      *p++ = '\0';
   }
