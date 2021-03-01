/*
 *      Writes compressed file to outfile.
 */

#include <stdio.h>
#include "lz1.h"

LZ_1(infile, outfile, bytes)
FILE     *infile, *outfile;
long     *bytes;
   {
   WORD  c, ent, n_ent, reslt, tag = TAG;

   for (free_ent = 0; free_ent < 256; free_ent++)
      {
      Code[free_ent].next = Code[free_ent].chain = NULL;
      Code[free_ent].suffix = free_ent;
      }

   lz1_init();
   lz_bytes = sizeof(tag);
   fwrite(&tag, 1, sizeof(tag), outfile);             /* mark as LZ */

   ent = getc(infile);
   while (!feof(infile) && (c = getc(infile)) != EOF)
      {
      /*
      ** Find the entry corresponding to the current entry suffixed
      ** with c.  Since the entries are sorted, suffix > c is as
      ** good as a null to indicate the need to create a new entry.
      */
      n_ent = Code[ent].chain;
      for (;;)
         {
         if (!n_ent || Code[n_ent].suffix > c)
            {
            output(ent, outfile);
            if (reslt = addentry(c, ent))
                /* return (reslt) */;
            ent = c;
            break;
            }
         else
            if (Code[n_ent].suffix == c)
               {
               ent = n_ent;
               break;
               }
            else
               n_ent = Code[n_ent].next;
         }
      }

   output(ent, outfile);                      /* put out final code */
   output(-1, outfile);               /* and -1 to flush and finish */
   *bytes = lz_bytes;
   return (0);
   }
/*page*/


addentry(c, ent)
WORD    c, ent;
   {
   register WORD   p_ent;

   /* if the table is full, there's nothing we can do. */
   if (free_ent >= maxmaxcode)
      return (-3);

   Code[free_ent].chain = NULL;
   Code[free_ent].suffix = c;

   if (!(p_ent = Code[ent].chain) || c < Code[p_ent].suffix)
      {
      Code[free_ent].next = p_ent;
      Code[ent].chain = free_ent;
      }
   else
      {
      while ((ent = Code[p_ent].next) && c >= Code[ent].suffix)
         p_ent = ent;
      Code[free_ent].next = ent;
      Code[p_ent].next = free_ent;
      }

   free_ent++;
   return (0);
   }
/*page*/
/*
 * Output the given code.
 */

output(code, ofp)
WORD     code;
FILE     *ofp;
   {
   WORD  n;

   if (code < 0)
      {
      /* at EOF--flush buffers and pack up */
      if (offset > 0)
         {
         fwrite(buf, 1, n = (offset + 7) / 8, ofp);
         lz_bytes += n;
         offset = 0;
         }
      fflush(ofp);
      return (0);
      }

   insert_bit(code);

   if ((offset += n_bits) == BytesToBits(n_bits))
      {
      fwrite(buf, 1, n_bits, ofp);
      lz_bytes += n_bits;
      offset = 0;
      }

   /*
    * If the next entry is going to be too big for the code size,
    * then increase it, if possible.
    */
   if (free_ent <= maxcode)
      return;

   /*
    * Write the whole buffer, because the input side won't
    * discover the size increase until after it has read it.
    */
   if (offset > 0)
      {
      fwrite(buf, 1, n_bits, ofp);
      lz_bytes += n_bits;
      offset = 0;
      }

   n_bits++;
   if (n_bits == maxbits)
      maxcode = maxmaxcode;
   else
      maxcode = (1 << n_bits) - 1;

   }


/* insert a value of "n_bits" bits at "offset" bits into buf */

insert_bit(value)
WORD    value;
   {
   WORD    w_offset, word, shift, size1, size2;

   word = offset >> LOG2WSIZE;
   w_offset = LowOrder(LOG2WSIZE) & offset;

   if (w_offset + n_bits <= WSIZE)
      {
      shift = (WSIZE - n_bits) - w_offset;
      buf[word] = (buf[word] & ~(LowOrder(n_bits) << shift)) |
         (value  << shift);
      }
   else
      {
      size1 = WSIZE - w_offset;
      size2 = n_bits - size1;
      buf[word] = (buf[word] & HighOrder(size1)) |
            ((unsigned) value >> size2);
      shift = WSIZE - size2;
      buf[word + 1] = (buf[word + 1] & ~(LowOrder(size2) << shift)) |
              (value << shift);
      }
   }
