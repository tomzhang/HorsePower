#include "h_global.h"

BSTree T;
// BSTree psym[MAXPSYM];
BSTree *psym;
L sindex,sybmax;

void R_Rotate(BSTree *p) {
    BSTree lc;
    lc = (*p)->lchild;
    (*p)->lchild = lc->rchild;
    lc->rchild = (*p);
    (*p) = lc;
}

void L_Rotate(BSTree *p) {
    BSTree rc;
    rc = (*p)->rchild;
    (*p)->rchild = rc->lchild;
    rc->lchild = (*p);
    (*p) = rc;
}

void leftBalance(BSTree *T) { //make left child balance
    BSTree lc, rd;
    lc = (*T)->lchild;
    switch (lc->bf) {
        case LH: //LL
            (*T)->bf = lc->bf = EH;
            R_Rotate(T);
            break;
        case RH: //LR
            rd = lc->rchild;
            switch (rd->bf) {
                case LH:(*T)->bf = RH;
                    lc->bf = EH;
                    break;
                case EH:(*T)->bf = lc->bf = EH;
                    break;
                case RH:(*T)->bf = EH;
                    lc->bf = LH;
                    break;
            }
            rd->bf = EH;
            L_Rotate(&((*T)->lchild));
            R_Rotate(T);
			break;
    }
}

void rightBalance(BSTree *T) { //make right child balance
    BSTree rc, ld;
    rc = (*T)->rchild;
    switch (rc->bf) {
        case RH: //RR
            (*T)->bf = rc->bf = EH;
            L_Rotate(T);
            break;
        case LH: //RL
            ld = rc->lchild;
            switch (ld->bf) {
                case LH:(*T)->bf = EH;
                    rc->bf = RH;
                    break;
                case EH:(*T)->bf = rc->bf = EH;
                    break;
                case RH:(*T)->bf = LH;
                    rc->bf = EH;
                    break;
            }
            ld->bf = EH;
            R_Rotate(&((*T)->rchild));
            L_Rotate(T);
			break;
    }
}

/*
index initial -1
id is the returned value
 */
bool insertAVL(BSTree *T, Elemtype e, bool *taller, L *index, L *id)
{
    if (!(*T)) {
        BSTree Tx = (BSTree) malloc(sizeof (BSTnode));
		*T = Tx;
        estring_assign_e(&Tx->data,&e); //T->data = e;
        Tx->lchild = Tx->rchild = NULL;
        Tx->index = ++(*index); //insert successfully
        Tx->bf = 0; //should be initialized zero
        *id = *index;
        psym[*id] = Tx; //BTtree psym[10005];psym[id]->data 
        *taller = true;
    } else {
        if (EQ(e, (*T)->data)) { //insert fail
            *id = (*T)->index;
            *taller = false;
            return 0;
        }
        if (LT(e, (*T)->data)) {
            // if (!InsertAVL(T->lchild, e, taller, index, id))return 0;
			if (!insertAVL(&((*T)->lchild), e, taller, index, id))return 0;
            if (*taller) {
                switch ((*T)->bf) {
                    case LH:
                        leftBalance(T);
                        *taller = false;
                        break;
                    case EH:
                        (*T)->bf = LH;
                        *taller = true;
                        break;
                    case RH:
                        (*T)->bf = EH;
                        *taller = false;
                        break;
                }
            }
        } else {
            // if (!InsertAVL(T->rchild, e, taller, index, id))return 0;
			if (!insertAVL(&((*T)->rchild), e, taller, index, id)) return 0;
            if (*taller) {
                switch ((*T)->bf) {
                    case LH:
                        (*T)->bf = EH;
                        *taller = false;
                        break;
                    case EH:
                        (*T)->bf = RH;
                        *taller = true;
                        break;
                    case RH:
                        rightBalance(T);
                        *taller = false;
                        break;
                }
            }
        }
    }
    return 1;
}

void initSym()
{
	T=NULL;
}

void incSym()
{
	BSTree *t = AL(BSTree,sybmax*2); //(BSTree*)malloc(sizeof(BSTree)*sybmax*2);
	// DOI(sybmax, t[i]=psym[i]); DOI(sybmax, t[i+sybmax]=NULL); 
	memcpy(t,psym,sybmax*sizeof(BSTree)); memset(t+sybmax,0,sybmax*sizeof(BSTree));
	sybmax *= 2; free(psym); psym=t;
}

L getSymbol(char* name) {
    estring e;
    bool taller;
    L id;
	estring_assign_str(&e,name); // e = name;
    taller = false;
    id = -1;
	if(sindex >= sybmax-1) incSym();
    insertAVL(&T, e, &taller, &sindex, &id);
    return id;
}

void insertSym(hsym h1) {
    getSymbol(h1.str);
    return;
}

void cleanSym() {
    L c;
    deleteT(T); T = NULL;
    for (c = 0; c <= sindex; c++) {
        psym[c] = NULL;
    }
}

// remove all & in method arguments
void deleteT(BSTree T)
{
    if (T) {
        if (T->lchild) deleteT(T->lchild);
        if (T->rchild) deleteT(T->rchild);
        DeleteQ(T);
    }
}

B hsymEqual(hsym h1, hsym h2) {
	return strcmp(h1->str, h2->str)==0;
}

B hsymLess(hsym h1, hsym h2) {
	return strcmp(h1->str, h2->str) <0;
}

B hsymLessEqual(hsym h1, Elemtype h2) {
	return strcmp(h1->str, h2->str)<=0;
}

