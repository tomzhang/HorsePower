#include "../global.h"

#define STACK_SIZE 1024
typedef struct _stack{
    S targ;
    L lineno;
    Node *node;
    B isMarked;
}STACK;

static STACK method[STACK_SIZE];
static L stack_index, line_cnt;
static B isDotMermaid = false;

static void dotScan(Node *n);
static void dotMain(List *list);

// Temp solution: " -> ''
static void printDotNode(Node *x, B isDot){
    C temp[512]; temp[0]=0; prettyNodeBuffNoLine(temp, x);
    putchar('[');
    if(isDot) P("label=");
    putchar('"');
    DOI(strlen(temp),{if(temp[i]=='"')P("''");else P("%c",temp[i]);})
    putchar('"');
    if(isDot) P(", shape=box");
    putchar(']');
}

static void printDotDef(L x){
    if(!method[x].isMarked){
        P("    "); P("N%lld",x); printDotNode(method[x].node, true); P("\n");
        method[x].isMarked = true;
    }
}
static void printDot(L x, L y){
    printDotDef(x);
    printDotDef(y);
    P("    "); P("N%lld -> N%lld\n",x,y);
}

static void printDotMermaid(L x, L y){
    P("    ");
    P("N%lld",x); printDotNode(method[x].node, false);
    P(" --> ");
    P("N%lld",y); printDotNode(method[y].node, false);
    P("\n");
}

static void addToStack(Node *n, S x){ /* x can be NULL */
    if(stack_index >= STACK_SIZE){
        EP("stack full... max %d\n", STACK_SIZE);
    }
    //P("added: %s\n",x);
    method[++stack_index].targ   = x;
    method[stack_index].lineno   = line_cnt++;
    method[stack_index].node     = n;
    method[stack_index].isMarked = false;
}

static L fetchFromStack(S x){
    DOIr(stack_index, if(method[i].targ && !strcmp(method[i].targ,x))R i) R -1;
}

static void linkInStack(S x){
    L k = fetchFromStack(x);
    if(k < 0) EP("variable %s not found, total = %lld\n", x,stack_index);
    //P("linking! %lld %lld %lld\n",line_cnt,k,stack_index);
    if(!isDotMermaid)
        printDot(method[k].lineno, line_cnt-1);
    else
        printDotMermaid(method[k].lineno, line_cnt-1);
}

/* -------- dotScan* -------- */

static void dotScanModule(Node *n){
    dotMain(n->val.module.body);
}

static void dotScanMethod(Node *n){
    // method.name/param/type
    stack_index = -1; line_cnt = 0;
    if(!isDotMermaid) P("digraph G {\n");
    else P("graph TD\n");
    dotMain(n->val.method.list);
    if(!isDotMermaid) P("}\n");
}

static void dotScanParamExpr(Node *n){
    dotMain(n->val.listS);
}

static void dotScanSimpleStmt(Node *n){
    addToStack(n, fetchName(n->val.simpleStmt.name));
    dotScan(n->val.simpleStmt.expr);
}

static void dotScanCastStmt(Node *n){
    addToStack(n, fetchName(n->val.castStmt.name));
    dotScan(n->val.castStmt.expr);
}

static void dotScanReturn(Node *n){
    addToStack(n, NULL);
    dotScan(n->val.nodeS); 
}

static void dotScanID    (Node *n){ linkInStack(n->val.idS); }
static void dotScanExpr  (Node *n){ dotScan(n->val.expr.param); }
static void dotScanLiteralParam(Node *n){ dotScan(n->val.nodeS); }

static void dotScan(Node *n){
    //P("kind = %s\n", getKindName(n->kind));
    switch(n->kind){
        case     moduleK: dotScanModule    (n); break;
        case     methodK: dotScanMethod    (n); break;
        case simpleStmtK: dotScanSimpleStmt(n); break;
        case   castStmtK: dotScanCastStmt  (n); break;
        case     returnK: dotScanReturn    (n); break;
        case       exprK: dotScanExpr      (n); break;
        case  paramExprK: dotScanParamExpr (n); break;
        case         idK: dotScanID        (n); break;
        case literalParamK: dotScanLiteralParam(n); break;
        default: break;//WP("skipped node type: %s\n", getKindName(n->kind));
    }
}

static void dotMain(List *list){
    List *ptr = list;
    while(ptr){
        dotScan(ptr->val);
        ptr = ptr->next;
    }
}

void dotProg(Prog *root){
    printBanner("DOT Printer");
    dotMain(root->module_list);
}

