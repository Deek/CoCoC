#ifdef REGCONTS
#include "cj.h"

static wlktr();
static fixnode();
static graft();

direct expnode *dregcont,*xregcont,*currexpr;
/* extern int fixnode(),graft(); */
extern expnode *subtree(),*treecont(),*regcont();


clrconts()
{
    setdreg(NULL);
    setxreg(NULL);
}


setdreg(tree)
register expnode *tree;
{
    walktree(tree,fixnode);
    if (dregcont) {
#ifdef DEBUG
        prtree(dregcont,"<*** D contained:");
#endif
        reltree(dregcont);
        dregcont = NULL;
    }
    if (tree) {
        if (regcont(tree) == NULL) {
            dregcont = newnode(0,0,0,0,0,0);
            nodecopy(tree,dregcont);
#ifdef DEBUG
            prtree(dregcont,">*** D given:");
#endif
        } else {
            reltree(tree->left);
            reltree(tree->right);
        }
        tree->op = DREG;
        tree->left = tree->right = NULL;
    }
    setcurr(NULL);
}


setxreg(tree)
register expnode *tree;
{
    walktree(tree,fixnode);
    if (xregcont) {
#ifdef DEBUG
        prtree(xregcont,"<<<< X contained:");
#endif
        reltree(xregcont);
        xregcont = NULL;
    }
    if (tree) {
        if (regcont(tree) == NULL) {
            xregcont = newnode(0,0,0,0,0,0);
            nodecopy(tree,xregcont);
#ifdef DEBUG
            prtree(xregcont,">>>> X given:");
#endif
        } else {
            reltree(tree->left);
            reltree(tree->right);
        }
        tree->op = XREG;
        tree->left = tree->right = NULL;
    }
    setcurr(NULL);
}


setcurr(tree)
register expnode *tree;
{
    walktree(tree,fixnode);
    if (currexpr) {
#ifdef DEBUG
        prtree(currexpr,"<=== currexpr contained:");
#endif
        reltree(currexpr);
        currexpr = NULL;
    }
    if (tree) {
        currexpr = newnode(0,0,0,0,0,0);
        nodecopy(tree,currexpr);
#ifdef DEBUG
        prtree(currexpr,">=== currexpr given:");
#endif
        tree->left = tree->right = NULL;
    }
}


cmptrees(tree,cont)
register expnode *tree, *cont;
{
    if (cont == NULL) return tree == NULL;
    if (tree == NULL) return 0;
    if (tree->op == cont->op
               && tree->val.num == cont->val.num
               && tree->modifier == cont->modifier) {
        return cmptrees(tree->left,cont->left)
                        && cmptrees(tree->right,cont->right);
    }
    return 0;
}


expnode *
subtree(tree,cont)
register expnode *tree,*cont;
{
    expnode *t;

    if (cont == NULL) return NULL;
    if (cmptrees(tree,cont)) return cont;
    if (t = subtree(tree,cont->left)) return t;
    return subtree(tree,cont->right);
}


expnode *
treecont(tree,op)
register expnode *tree;
{
    expnode *t;

    if (t = tree) {
        if (tree->op != op) {
            if ((t = treecont(tree->left,op)) == NULL)
                t = treecont(tree->right,op);
        }
    }
    return t;
}


expnode *
regcont(tree)
register expnode *tree;
{
    expnode *t;

    if (t = tree) {
        if ((tree->op & INDIRECT) == 0) {
            switch (tree->op) {
#ifdef USE_YREG
                case YREG:
#endif
                case UREG:
                    if (t->val.num || t->left || t->right) break;
                default:
                    if ((t = regcont(tree->left)) == NULL)
                        t = regcont(tree->right);
                case DREG:
                case XREG:
                case XIND:
                case YIND:
                case UIND:
                    ;
            }
        }
    }
    return t;
}


static
fixnode(tree)
register expnode *tree;
{
    if (tree) {
        if (tree->op & INDIRECT) {
            tree->op = STAR;
            tree->val.num = 0;
        } else switch (tree->op) {
            case DREG:
                graft(dregcont,tree);
                break;
            case XREG:
                graft(xregcont,tree);
                break;
            case UREG:
                graft(currexpr,tree);
                break;
            case XIND:
            case UIND:
                tree->op = STAR;
                tree->val.num = 0;
                break;
        }
    }
}


static
graft(limb,tree)
register expnode *limb, *tree;
{
    if (limb && tree->left == NULL && tree->right == NULL) {
        nodecopy(limb,tree);
        tree->left = treecopy(limb->left);
        tree->right = treecopy(limb->right);
    }
}


expnode *
treecopy(tree)
register expnode *tree;
{
    expnode *t;

    if (t = tree) {
        nodecopy(tree,t = newnode(0,0,0,0,0,0));
        t->left = treecopy(tree->left);
        t->right = treecopy(tree->right);
    }
    return t;
}


rplcnode(tree,reg)
register expnode *tree;
{
    tree->op = reg;
    tree->val.num = 0;
    reltree(tree->left);
    reltree(tree->right);
    tree->left = tree->right = NULL;
}

static int (*walkfunc)();

walktree(tree,funct)
expnode *tree;
int (*funct)();
{
    walkfunc = funct;
    wlktr(tree);
}


static
wlktr(tree)
register expnode *tree;
{
    if (tree) {
        wlktr(tree->left);
        wlktr(tree->right);
        (*walkfunc)(tree);
    }
}
#endif  /* REGCONTS */
