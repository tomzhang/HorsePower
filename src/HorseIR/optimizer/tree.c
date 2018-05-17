#include "global.h"

extern Prog *root;
extern int yylineno;

Prog *makeProg(List *module_list){
    Prog *p  = NEW(Prog);
    p->module_list = module_list;
    return p;
}

List *makeList(Node *val, List *next){
    List *t = NEW(List);
    t->val  = val;
    t->next = next;
    return t;
}

Node *makeNodeID(char *id){
    Node *n    = NEW(Node);
    n->kind    = idK;
    n->val.idS = strdup(id);
    n->lineno  = yylineno;
    return n;
}


// list -> node
Node *makeListKind(List *t, Kind k){
    Node *n     = NEW(Node);
    n->kind     = k;
    n->val.listS= t;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeKind(Node *d, Kind k){
    Node *n     = NEW(Node);
    n->kind     = k;
    n->val.nodeS= d;
    n->lineno   = yylineno;
    return n;
}
// non-terminals

Node *makeNodeModule(Node *name, List *body){
    Node *n     = NEW(Node);
    n->kind     = moduleK;
    n->val.module.name = name;
    n->val.module.body = body;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeModuleMethod(Node *name, Node *type, List *stmt){
    Node *n     = NEW(Node);
    n->kind     = methodK;
    n->val.method.name = name;
    n->val.method.type = type;
    n->val.method.list = stmt;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeImport(Node *cID){
    Node *n     = NEW(Node);
    n->kind     = importK;
    n->val.nodeS= cID;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeSimpleStmt1(Node *name, Node *type, Node *expr){
    Node *n     = NEW(Node);
    n->kind     = simpleStmtK;
    n->val.simpleStmt.name = name;
    n->val.simpleStmt.type = type;
    n->val.simpleStmt.expr = expr;
    n->lineno   = yylineno;
}

Node *makeNodeSimpleStmt2(Node *name, Node *type, Node *expr, Node *cast_type){
    Node *n     = NEW(Node);
    n->kind     = castStmtK;
    n->val.castStmt.name = name;
    n->val.castStmt.type = type;
    n->val.castStmt.expr = expr;
    n->val.castStmt.cast = cast_type;
    n->lineno   = yylineno;
}

Node *makeNodeReturnStmt(Node *rtn){
    Node *n     = NEW(Node);
    n->kind     = returnK;
    n->val.nodeS =rtn;
    n->lineno   = yylineno;
}

Node *makeNodeExpr(Node *func, Node *paramExpr){
    Node *n     = NEW(Node);
    n->kind     = exprK;
    n->val.expr.func  = func;
    n->val.expr.param = paramExpr;
    n->lineno   = yylineno;
}

Node *makeNodeFunction(char *name){
    Node *n     = NEW(Node);
    n->kind     = funcK;
    n->val.idS  = strdup(name);
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeParamLiteral(Node *paramValue){
    Node *n     = NEW(Node);
    n->kind     = literalParamK;
    n->val.nodeS= paramValue;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeLiteralBool(List *int_list){
    return makeListKind(int_list, literalBoolK);
}

Node *makeNodeLiteralChar(char *charValue){
    Node *n     = NEW(Node);
    n->kind     = literalCharK;
    n->val.charS= strdup(charValue);
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeLiteralInt(List *int_list, Node *type){
    return makeListKind(int_list, literalIntK);
}

Node *makeNodeIntType(pType typ){
    return makeNodeType(typ);
}

Node *makeNodeLiteralFloat(List *float_list, Node *type){
    return makeListKind(float_list, literalFloatK);
}

Node *makeNodeFloatType(pType typ){
    return makeNodeType(typ);
}

Node *makeNodeLiteralSymbol(List *symbol_list){
    return makeListKind(symbol_list, literalSymK);
}

Node *makeNodeLiteralDate(List *date_list){
    return makeListKind(date_list, literalDateK);
}

Node *makeNodeCompoundID1(char *name1, char *name2){
    Node *n     = NEW(Node);
    n->kind     = compoundK;
    n->val.compoundID.name1 = strdup(name1);
    n->val.compoundID.name2 = strdup(name2);
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeCompoundID2(char *name1){
    return makeNodeCompoundID1(name1, "*");
}

// types

Node *makeNodeType(pType typ){
    Node *n     = NEW(Node);
    n->kind     = typeK;
    n->val.typeS= typ;
    n->lineno   = yylineno;
    return n; 
}


Node *makeNodeTypeWithName(char *type){
    if(!strcmp(type, "table"))return makeNodeType(tableT);
    if(!strcmp(type, "table"))return makeNodeType(ktableT);
    error("type has not been specified");
}

// literals

Node *makeNodeIntValue(int value, char op){
    Node *n     = NEW(Node);
    n->kind     = intK;
    switch(op){
        case '+': n->val.intS =      value; break;
        case '-': n->val.intS = -1 * value; break;
        default: error("unexpected op for int");
    }
    return n;
}

Node *makeNodeFloatValue(double value, char op){
    Node *n     = NEW(Node);
    n->kind     = floatK;
    switch(op){
        case '+': n->val.floatS =      value; break;
        case '-': n->val.floatS = -1 * value; break;
        default: error("unexpected op for float");
    }
    return n;
}

Node *makeNodeConstSymbol(char *value){
    Node *n     = NEW(Node);
    n->kind     = symK;
    n->val.charS= strdup(value);
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeConstDate(int value){
    Node *n     = NEW(Node);
    n->kind     = dateK;
    n->val.dateS= value;
    n->lineno   = yylineno;
    return n;
}

Node *makeNodeParamExpr(List *param_list){
    return makeListKind(param_list, paramExprK);
}
