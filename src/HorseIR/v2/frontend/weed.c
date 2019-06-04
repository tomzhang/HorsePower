#include "../global.h"

/* declarations */

#define weedParamExpr weedListNode
#define weedExpr      weedListNode
#define weedReturn    weedListNode
#define weedBlock     weedListNode
#define weedVarDecl   weedListNode

static void weedNode    (Node *x);
static bool weedDate    (int x);
static bool weedMonth   (int x);
static bool weedMinute  (int x);
static bool weedSecond  (int x);
static bool weedTime    (int x);
static bool weedDT      (long long x);
static bool weedClex    (float *x);
static bool weedString  (char *x);
static void weedList    (List *x);
static void weedListNode(Node *x);

static int cntMain = 0;
extern Node *entryMain;

/* code blocks */

/*
 * bool: 0 or 1
 *   i8: [-128, 127]
 *  i16: [-2^15, 2^15-1]
 *  i32: [-2^31, 2^31-1]
 *  i64: [-2^63, 2^63-1]
 */

#define checkDay(x, n) ((x)>=1 && (x)<=(n))
#define isNotLeap(yy)  (yy%4!=0 || yy%400!=0)

#define checkBool(x)   (0==x || 1==x)
#define checkSmall(x)  (x>=-128 || x<=127);
#define checkShort(x)  (x>=-32768 || x<=32767)
#define checkInt(x)    (x>=INT_MIN || x<=INT_MAX);
#define checkFloat(x)  (x>=FLT_MIN || x<=FLT_MAX)
#define checkDouble(x) (x>=DBL_MIN || x<=DBL_MAX)

static bool weedInt(int x, Type t){
    switch(t){
        case   boolT: R checkBool(x);
        case     i8T: R checkSmall(x);
        case    i16T: R checkShort(x);
        case    i32T: R 1;
        case    f32T: R checkFloat(x);
        case    f64T: R checkDouble(x);
        default: R 0;
    }
}

static bool weedLong(long long x, Type t){
    switch(t){
        case boolT: R checkBool(x);
        case   i8T: R checkSmall(x);
        case  i16T: R checkShort(x);
        case  i32T: R checkInt(x);
        case  i64T: R 1;
        case  f32T: R checkFloat(x);
        case  f64T: R checkDouble(x);
        default: R 0;
    }
}

static bool weedFloat(double x, Type t){
    switch(t){
        case  f32T: R (x>=FLT_MIN || x<=FLT_MAX);
        case  f64T: R 1;
        default: R 0;
    }
}

static bool weedConst(Node *x, Type t){
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        case    intC: R weedInt  (p->valI, t);
        case  floatC: R weedFloat(p->valF, t);
        case   longC: R weedLong (p->valL, t);
        case   clexC: R   clexT == t && weedClex  (p->valX);
        case   charC: R   charT == t && weedString(p->valS);
        case    symC: R    symT == t && weedString(p->valS);
        case    strC: R    strT == t && weedString(p->valS);
        case   dateC: R   dateT == t && weedDate  (p->valI);
        case  monthC: R  monthT == t && weedMonth (p->valI);
        case     dtC: R     dtT == t && weedDT    (p->valL);
        case minuteC: R minuteT == t && weedMinute(p->valI);
        case secondC: R secondT == t && weedSecond(p->valI);
        case   timeC: R   timeT == t && weedTime  (p->valI);
        default: EP("Constant type not supported: %d\n", p->type);
    }
}

#define weedYY(x) (x>=1000 && x<=9999)
#define weedMM(x) (x>=1 && x<=12)
#define weedHH(x) (x>=0 && x<24)
#define weedM2(x) (x>=0 && x<60)
#define weedSS(x) (x>=0 && x<60)
#define weedLL(x) (x>=0 && x<1000)

static bool weedDate(int x){
    int yy=x/10000,mm=x/100%100,dd=x%100;
    if(weedYY(yy) && weedMM(mm)){
        switch(mm){
            case  1: R checkDay(dd, 31); break; //
            case  2: R checkDay(dd, isNotLeap(yy)?28:29); break;
            case  3: R checkDay(dd, 31); break; //
            case  4: R checkDay(dd, 30); break;
            case  5: R checkDay(dd, 31); break; //
            case  6: R checkDay(dd, 30); break;
            case  7: R checkDay(dd, 31); break; //
            case  8: R checkDay(dd, 31); break; //
            case  9: R checkDay(dd, 30); break;
            case 10: R checkDay(dd, 31); break; //
            case 11: R checkDay(dd, 30); break;
            case 12: R checkDay(dd, 31); break; //
        }
    }
    else R 0;
}

static bool weedMonth(int x){
    int yy=x/100,mm=x%100;
    R weedYY(yy) && weedMM(mm);
}

static bool weedMinute(int x){
    int hh=x/60, mm=x%60;
    R weedHH(hh) && weedM2(mm);
}

static bool weedSecond(int x){
    int ss=x%60;
    R weedMinute(x/60) && weedSS(ss);
}

static bool weedTime(int x){
    int ll=x%1000;
    R weedSecond(x/1000) && weedLL(ll);
}

static bool weedDT(long long x){
    R weedDate(x/86400000) && weedTime(x%86400000);
}

static bool weedClex(float *x){
    float rel = x[0], img = x[1];
    R checkFloat(rel) && checkFloat(img);
}

static bool weedString(char *x){
    while(*x){
        char c = *x; if(c<0 || c>255) R 0; x++;
    }
    R 1;
}

static void printConst(Node *x){
    //printNodeType(x);
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        case   intC: P("int: %d\n"   , p->valI); break;
        case floatC: P("float: %g\n" , p->valF); break;
        case  longC: P("long: %lld\n", p->valL); break;
        default: EP("Type not supported: %d\n", p->type);
    }
}

static Type getVectorType(Node *x){
    Type t = getType(x);
    switch(t){
        case   boolT: case     i8T: case   i16T: case  i32T: case  i64T:
        case    f32T: case    f64T: case  clexT: case charT: case  symT:
        case    strT: case   dateT: case monthT: case timeT: case   dtT:
        case minuteT: case secondT: return t;
        default: EP("Invalid type for constants: %s\n", x->val.type.typ);
    }
}

Type getType(Node *x){
    if(!x) EP("Empty type node found\n");
    if(x->val.type.isWild) R wildT;
    else if(x->val.type.cell) {
        TODO("Cell types not allowed.\n");
    }
    else {
        char *typ = x->val.type.typ;
        if(!strcmp(typ, "bool")) R boolT;
        else if(!strcmp(typ, "i8"  )) R i8T;
        else if(!strcmp(typ, "i16" )) R i16T;
        else if(!strcmp(typ, "i32" )) R i32T;
        else if(!strcmp(typ, "i64" )) R i64T;
        else if(!strcmp(typ, "f32" )) R f32T;
        else if(!strcmp(typ, "f64" )) R f64T;
        else if(!strcmp(typ, "clex")) R clexT;
        else if(!strcmp(typ, "char")) R charT;
        else if(!strcmp(typ, "sym" )) R symT;
        else if(!strcmp(typ, "str" )) R strT;
        else if(!strcmp(typ, "d"   )) R dateT;
        else if(!strcmp(typ, "m"   )) R monthT;
        else if(!strcmp(typ, "z"   )) R dtT;
        else if(!strcmp(typ, "w"   )) R minuteT;
        else if(!strcmp(typ, "v"   )) R secondT;
        else if(!strcmp(typ, "t"   )) R timeT;
        else if(!strcmp(typ, "list")) R listT;
        else if(!strcmp(typ, "dict")) R dictT;
        else if(!strcmp(typ, "enum")) R enumT;
        else if(!strcmp(typ, "table" )) R tableT;
        else if(!strcmp(typ, "ktable")) R ktableT;
        else if(!strcmp(typ, "func"  )) R funcT;
        else EP("Invalid type: %s\n", typ);
    }
    R 0;
}

// boolT =?= H_B ...
static char *getTypeStr2(Type t){
    switch(t){
        CaseLine(boolT);
        CaseLine(i8T);
        CaseLine(i16T);
        CaseLine(i32T);
        CaseLine(i64T);
        CaseLine(f32T);
        CaseLine(f64T);
        CaseLine(clexT);
        CaseLine(charT);
        CaseLine(symT);
        CaseLine(strT);
        CaseLine(dateT);
        CaseLine(monthT);
        CaseLine(minuteT);
        CaseLine(secondT);
        CaseLine(timeT);
        CaseLine(dtT);
        default: TODO("Add more type: %d\n",t);
    }
}

static char *getConstTypeStr(Node *x){
    ConstValue *p = x->val.nodeC;
    switch(p->type){
        CaseLine(intC);
        CaseLine(floatC);
        CaseLine(longC);
        CaseLine(clexC);
        CaseLine(charC);
        CaseLine(strC);
        CaseLine(symC);
        CaseLine(dateC);
        CaseLine(monthC);
        CaseLine(minuteC);
        CaseLine(secondC);
        CaseLine(timeC);
        CaseLine(dtC);
        default: TODO("Add more type: %d\n",p->type);
    }
}

/*
 * a type indicator is a must
 */
static void weedVector(Node *x){
    //printNodeType(x);
    List *p = x->val.vec.val;
    Type  t = getVectorType(x->val.vec.typ);
    while(p){
        if(!weedConst(p->val, t)){
            printNode(x);
            EP("Literal type or range error: (expect %s, find %s)\n", \
                    getTypeStr2(t),getConstTypeStr(p->val));
        }
        p = p->next;
    }
}

static void weedVar(Node *x){
    char *name = x->val.param.id;
    if(!strcmp(name, USCORE))
        EP("Underscore '" USCORE "' shouldn't have any type.\n");
}

static void weedStmt(Node *stmt){
    weedList(stmt->val.assignStmt.vars);
    weedNode(stmt->val.assignStmt.expr);
}

static void weedIf(Node *x){
    weedNode(x->val.ifStmt.condExpr);
    weedNode(x->val.ifStmt.thenBlock);
    weedNode(x->val.ifStmt.elseBlock);
}

static void weedWhile(Node *x){
    weedNode(x->val.whileStmt.condExpr);
    weedNode(x->val.whileStmt.bodyBlock);
}

static void weedRepeat(Node *x){
    weedNode(x->val.repeatStmt.condExpr);
    weedNode(x->val.repeatStmt.bodyBlock);
}

static void weedImport(Node *x){
    //printNodeType(x);
}

static void weedGlobal(Node *x){
    //printNodeType(x);
}

static void weedMethod(Node *x){
    //printNodeType(x);
    if(!strcmp(x->val.method.fname, "main")){
        if(cntMain == 0){ entryMain = x; cntMain++; }
        else EP("Only one main method expected in modules.\n");
    }
    weedNode(x->val.method.block);
}

static void weedModule(Node *module){
    //printNodeType(module);
    weedList(module->val.module.body);
}

static void weedCall(Node *x){
    //printNodeType(x);
    Node *param = x->val.call.param;
    if(param) weedNode(param);
}

static void weedCast(Node *x){
    //printNodeType(x);
    weedNode(x->val.cast.exp);
}

static void weedList(List *x){
    if(x){
        weedList(x->next);
        weedNode(x->val);
    }
}

static void weedListNode(Node *x){
    weedList(x->val.listS);
}

static void weedName(Node *x){
    if(x->val.name.one) ;
    else {
        char *id1 = x->val.name.id1;
        char *id2 = x->val.name.id2;
        if(!strcmp(id2, USCORE) || !strcmp(id1, USCORE))
            EP("Underscore '" USCORE "' can't be used in a global name reference\n");
    }
}

static void weedNode(Node *x){
    if(!x) R ;
    switch(x->kind){
        case    moduleK: weedModule(x);    break;
        case    importK: weedImport(x);    break;
        case    methodK: weedMethod(x);    break;
        case    globalK: weedGlobal(x);    break;
        case     blockK: weedBlock(x);     break;
        case       varK: weedVar(x);       break;
        case      callK: weedCall(x);      break;
        case      nameK: weedName(x);      break;
        case      funcK: break;
        case      castK: weedCast(x);      break;
        case    vectorK: weedVector(x);    break;
        case     constK: printConst(x);    break;
        case   argExprK: weedExpr(x);      break; /* list */
        case paramExprK: weedParamExpr(x); break; /* list */
        case      stmtK: weedStmt(x);      break;
        case        ifK: weedIf(x);        break;
        case     whileK: weedWhile(x);     break;
        case    repeatK: weedRepeat(x);    break;
        case    returnK: weedReturn(x);    break;
        case     breakK: break;
        case  continueK: break;
        case   varDeclK: weedVarDecl(x);   break;
        default: EP("Type unknown: %s\n", getNodeTypeStr(x));
    }
}

static void weedMainMethod(){
    if(cntMain == 1); // fine
    else if(cntMain == 0) EP("Main method not found\n");
    else EP("Only one main method expected, but %d found\n", cntMain);
}

static void init(){
    cntMain = 0;
    entryMain = NULL;
}

void weedProg(Prog *root){
    printBanner("Program Weeder");
    init();
    weedList(root->module_list);
    weedMainMethod();
}
