setuser(id)
int id;
   {
   struct registers r;

   r.rg_y = id;
   _os9(F_SUSER, &r);
   }
