/*
 'malloc.c' - K&R malloc() and free()
*/

union header {
 struct {
  union header *ptr;
  unsigned size;
 } s;
 long x;
};

typedef union header Header;


#define NALLOC 512 /* minimum # blocks to request... */
#define NULL 0

static Header base;
static Header *freep=NULL;


Header *morecore(nu)
register unsigned nu;
{
 Header *sbrk();
 Header *up;

 if (nu<NALLOC)
  nu=NALLOC;

 up=sbrk(nu * sizeof(Header));

 if (up == (Header *)-1)
  return NULL;

 up->s.size=nu;
 free(up+1);
 return (freep);
}


Header *malloc(nbytes)
unsigned nbytes;
{
 register Header *p;
 Header *prevp;
 unsigned nunits;

 nunits=(nbytes+sizeof(Header)-1)/sizeof(Header)+1;

 if ((prevp=freep) == NULL)
  {
   base.s.ptr=freep=prevp=&base;
   base.s.size=0;
  };

 for (p=prevp->s.ptr;;prevp=p,p=p->s.ptr)
  {
   if (p->s.size>=nunits)
    {
     if (p->s.size == nunits)
      prevp->s.ptr=p->s.ptr;
     else
      {
       p->s.size-=nunits;
       p+=p->s.size;
       p->s.size=nunits;
      };
     freep=prevp;
     return (p+1);
    };
   if (p == freep)
    if ((p=morecore(nunits))==NULL)
     return (NULL);
  };
}


free(bp)
Header *bp;
{
 register Header *p;

 bp--;

 for (p=freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
  if (p>=p->s.ptr && (bp > p || bp < p->s.ptr))
   break;

 if (bp+bp->s.size == p->s.ptr)
  {
   bp->s.size += p->s.ptr->s.size;
   bp->s.ptr=p->s.ptr->s.ptr;
  }
 else
  bp->s.ptr=p->s.ptr;

 if (p+p->s.size == bp)
  {
   p->s.size += bp->s.size;
   p->s.ptr = bp->s.ptr;
  }
 else
  p->s.ptr=bp;
 freep=p;
}
