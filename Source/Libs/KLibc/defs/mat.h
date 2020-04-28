
typedef struct {
   int      m_rows, m_cols;
   double   m_value[1];
   } MAT;


double   m_cofactor(), m_determinant();
MAT      *m_copy(), *m_create(), *m_invert(), *m_transpose(),
         *m_multiply(), *m_solve(), *m_add(), *m_sub(), *m_read();

#define  m_v(m, r, c)   (m->m_value[r * (m->m_cols) + c])
#define  M_NULL         ((MAT *)0)
