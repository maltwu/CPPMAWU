
struct _st_logical
{
    char opr; //操作数记录可为 '|','&','0','1'（分叉点)
    _st_logical *pleft;
    _st_logical *pright;
};
typedef  _st_logical STLOGIC;
typedef  _st_logical * PSTLOGIC;

PSTLOGIC makeTreeByExpr(const char *);
void printTree(PSTLOGIC,int);
char AcculateTree(PSTLOGIC , int &, int &);
void freeTree(PSTLOGIC);