#include "../global.h"

extern Prog *root;

static void printBannerLocal(S msg){
    WP("/*==== ");
    WP("Optimizing %s", msg);
    WP(" ====*/\n");
}

static void optimizerMain(OC opt){
    printBannerLocal(getOptStr(opt));
    switch(opt){
        case OPT_FE: optElementwise(); break;
        case OPT_FP: optPattern();     break;
        default: TODO("Add impl. %s",  getOptStr(opt));
    }
}

static bool isOptimizeAll(){
    DOI(numOpts, if(qOpts[i] == OPT_ALL) R true) R false;
}

static void init(){
    buildUDChain(root);
}

I HorseCompilerOptimized(){
    printBanner("Start Optimizing");
    init();
    if(isOptimizeAll()){
        TODO("Add impl.");
    }
    else {
        DOI(numOpts, optimizerMain(qOpts[i]));
    }
    R 0;
}

