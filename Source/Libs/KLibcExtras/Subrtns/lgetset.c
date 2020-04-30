/*
** local getstat
**  r_a = path number       r_b = code
**  r_x = parameter         r_y = parameter          r_u = parameter
*/

lgetstat(r_a, r_b, r_x, r_y, r_u)
char r_a, r_b;
int  r_x, r_y, r_u;
   {
   struct registers  reg;

   reg.rg_a = r_a;
   reg.rg_b = r_b;
   reg.rg_x = r_x;
   reg.rg_y = r_y;
   reg.rg_u = r_u;
   if (_os9(I_GETSTT, &reg) == -1)
         exit (reg.rg_b);
   return (reg.rg_b);
   }


/*
** local setstat
**  r_a = path number       r_b = code
**  r_x = parameter         r_y = parameter          r_u = parameter
*/

lsetstat(r_a, r_b, r_x, r_y, r_u)
char r_a, r_b;
int  r_x, r_y, r_u;
   {
   struct registers  reg;

   reg.rg_a = r_a;
   reg.rg_b = r_b;
   reg.rg_x = r_x;
   reg.rg_y = r_y;
   reg.rg_u = r_u;
   if (_os9(I_SETSTT, &reg) == -1)
         exit (reg.rg_b);
   return (reg.rg_b);
   }
