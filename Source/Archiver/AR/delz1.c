/*
 * Decompress the input file.
 */

#include <stdio.h>
#include "lz1.h"

WORD
de_LZ_1(infile, outfile, bytes)
FILE     *infile, *outfile;
long     bytes;
   {
   WORD  tag, finchar, code, oldcode, incode, stack_top = MAXSTACK;
   WORD  getcode();
   char  stack[MAXSTACK];

   if (fread(&tag, sizeof(tag), 1, infile) < 1 || tag != TAG)
      return (-2);

   lz1_init();
   lz_bytes = bytes - sizeof(tag);
   for (free_ent = 0; free_ent < 256; free_ent++)
      {
      Code[free_ent].next = Code[free_ent].chain = NULL;
      Code[free_ent].prefix = NULL;
      Code[free_ent].suffix = free_ent;
      }

   finchar = oldcode = getcode(infile);
   putc((char)finchar, outfile);       /* first code must be 8 bits */

   while ((code = getcode(infile)) != -1)
      {
      incode = code;
      if (code >= free_ent)       /* Special case for KwKwK string. */
         {
         stack[--stack_top] = finchar;
         code = oldcode;
         }

      while (code >= 256)   /* Generate characters in reverse order */
         {
         stack[--stack_top] = Code[code].suffix;
         code = Code[code].prefix;
         }
      stack[--stack_top] = finchar = Code[code].suffix;

      /* And put them out in forward order */
      fwrite(&stack[stack_top], 1, MAXSTACK - stack_top, outfile);
      stack_top = MAXSTACK;

      if ((code = free_ent) < maxmaxcode)     /* Generate new entry */
         {
         Code[code].prefix = oldcode;
         Code[code].suffix = finchar;
         free_ent++;
         }
      oldcode = incode;                  /* Remember previous code. */
      }
   return (code);
   }
/*page*/
/*
 * Read one code from the input file.  If EOF, return -1.
 * Inputs:
 *      infile:    Input file.
 * Outputs:
 *      code or -1 is returned.
 */

WORD
getcode(infile)
FILE    *infile;
   {
   WORD    code;
   static WORD size = 0;

   if (offset >= size || free_ent > maxcode)
      {
      if (free_ent > maxcode)
         {
         n_bits++;              /* new entry too big, increase size */
         maxcode = (n_bits == maxbits) ? maxmaxcode : (1 << n_bits) - 1;
         }
      if (lz_bytes <= 0)
         return (-1);                                      /* "eof" */
      size = (lz_bytes > (long) n_bits) ? n_bits : (int) lz_bytes;
      size = fread(buf, 1, size, infile);    /* read new buffer too */
      if (size <= 0)
         return (-1);
      lz_bytes -= size;
      offset = 0;
      /*
       * Convert size to bits, and round down so that stray bits
       * at the end aren't treated as data
       */
      size = BytesToBits(size) - (n_bits - 1);
      }
   code = fetch();
   offset += n_bits;

   return(code);
   }


fetch()
   {
   WORD    w_offset, word, size2;

   word = offset >> LOG2WSIZE;
   w_offset = LowOrder(LOG2WSIZE) & offset;

   if (w_offset + n_bits <= WSIZE)
      {
      return(((unsigned) buf[word] >> ((WSIZE - n_bits) - w_offset))
         & LowOrder(n_bits));
      }
   else
      {
      size2 = n_bits - (WSIZE - w_offset);
      return(((buf[word] << size2) |
            ((unsigned) buf[word + 1] >> (WSIZE - size2)))
            & LowOrder(n_bits));
      }
   }
