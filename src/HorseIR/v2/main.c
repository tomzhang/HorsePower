#include "global.h"

Prog *root;
Node *entryMain;
int yyparse(); /* see y.tab.c */
extern FILE *yyin;
extern int yylineno;

static void runInterpreterCore(){
    tic();
    HorseInterpreter();
    time_toc("Interpretation time (ms): %g\n", elapsed);
}

static void runCompilerCore(){
    tic();
    if(numOpts < 1)
        HorseCompilerNaive();     // compiler.c
    else
        HorseCompilerOptimized(); // optimizer.c
    time_toc("Compile time (ms): %g\n", elapsed);
}

static void parseInput(char *file_path){
    yylineno = 1;
    if(!(yyin=fopen(file_path, "r"))){
        EP("File not found: %s\n", file_path);
    }
    WP("parsing ....\n");
    tic();
    int ret = yyparse();
    fclose(yyin);
    if(ret == 0) WP("Successfully parsed: %s!\n",file_path);
    else { EP("Parsing failed: %s\n",file_path);}
    time_toc("Parsing time (ms): %g\n", elapsed);
}

static void envInit(char *file){
    parseInput(file);
    initGlobal();
    weedProg(root);  // weed types/main
}

static void envInterpreter(char *file){
    envInit(file);
    buildSymbolTable(root);
    runInterpreterCore();
}

static void envCompiler(char *file){
    envInit(file);
    buildSymbolTable(root);
    analyzeSR(root);
    propagateTypeShape(root);
    runCompilerCore();
}

static void envPrettyPrint(char *file){
    envInit(file);
    printProg(root);
}

static void envDotPrint(char *file){
    envInit(file);
    TODO("dot print\n");
    //dotProg(root);
}

int main(int argc, char *argv[]){
    int r = getLongOption(argc, argv);
    if(r) BAD_TRY();
    else {
        switch(optMode){
            case InterpNaiveM: envInterpreter(qPath); break;
            case    CompilerM: envCompiler(qPath);    break;
            case   InterpJITM: TODO("interpreter jit mode\n"); break;
            case PrettyPrintM: envPrettyPrint(qPath); break;
            case    DotPrintM: envDotPrint(qPath);    break;
            case       StatsM: handleStats(qStats);   break;
            case  ExperimentM: TODO("experiment\n");  break;
            case     VersionM: version();   break;
            case      HelperM: GOOD_TRY();  break;
            case     UnknownM:  BAD_TRY();  break;
            default: EP("Option mode not supported: %d\n", optMode);
        }
    }
    return 0;
}

