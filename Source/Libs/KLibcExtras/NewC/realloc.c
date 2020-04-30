/*
 * realloc.c
 *  changes the size of a malloc'ed object to a new size
 *  if 'now' is null, it just performs a malloc
 */

typedef struct header {
   struct header  *ptr;
   unsigned       size;
   } HEADER;


realloc(now, need)
char     *now;
unsigned need;
   {
   char     *p;
   unsigned osize;
   HEADER   *hp;


   if ((p = malloc(need)) != 0)
      if (now)
         {
         hp = ((HEADER *) now) - 1;
         osize = (hp->size - 1) << 2;
         memcpy(p, now, need > osize ? osize : need);
         free(now);
         }
   return (p);
   }
