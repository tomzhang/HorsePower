#ifndef H_GLOBAL_H
#define H_GLOBAL_H

#if defined(__APPLE__)
    #define H_MACOS
#endif

#if defined(__linux__)
    #define H_LINUX
#endif

#if defined(__MINGW32__)
    #define H_WINDOWS
#endif

/* declare additional macros */
#ifdef H_LINUX
  #define _BSD_SOURCE
/* check for ANSI extensions: e.g. strdup */
  #ifdef __STDC_ALLOC_LIB__
    #define __STDC_WANT_LIB_EXT2__ 1
  #else
    #define _POSIX_C_SOURCE 200810L
  #endif
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h> /* va_start */

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* declaration */
void  *newM(int n);
void   error(const char* s);
void   my_tic();
double my_toc(bool isPrint);
void   time_clear();

#define NEW(typ)    (typ* )newM(sizeof(typ))
#define NEW2(typ,n) (typ**)newM(sizeof(typ*)*(n))
#define NEWL(typ,n) (typ* )newM(sizeof(typ)*(n))
#define instanceOf(x,t) (x!=NULL&&(x->kind)==(t))
#define CaseLine(x) case x: R #x

#define P printf
#define R return
#define SP sprintf
#define FP fprintf
#define PN(x)     P("%s\n",x)
#define newLine() P("\n")
#define WP(...)   do{FP(stderr,__VA_ARGS__);}while(0)  // warning
#define FLine()   FP(stderr, "\n")
#define INFO(s)   FP(stderr, s "(%s:%d:%s):\n\t", __FILE__, __LINE__, __func__)
#define FT(s,...) FP(stdout,s,__VA_ARGS__)
#define FS(x)     FT("%s",x)
#define sEQ(x,y)  (!strcmp(x,y))   // string equal
#define sNEQ(x,y) (0!=strcmp(x,y)) // string not equal

/* For debugging */
#define printBanner(s) WP("/*==== "s" ====*/\n")
#define EP(...)   do{INFO("[ERROR]");  FP(stderr,__VA_ARGS__); FLine(); exit(1);}while(0)  // error
#define TODO(...) do{INFO("[TODO]");   FP(stderr,__VA_ARGS__); FLine(); exit(2);}while(0)  // todo
#define STOP(...) do{P("%-15s>> ",__FUNCTION__); P(__VA_ARGS__); getchar();}while(0)

/* time functions */
#define tic() my_tic()
#define toc() my_toc(1)
#define calc_toc() my_toc(0)
#define time_toc(...) do{E elapsed=calc_toc();P("// ");P(__VA_ARGS__);}while(0)

/* constants */
#define uscore "_"  // underscore
#define comma  ','
#define Indent2 "  "
#define Indent4 "    "
#define CODE_MAX_SIZE 10240

#include "frontend/tree.h"
#include "frontend/weed.h"
#include "frontend/pretty.h"
#include    "backend/common.h"
#include   "analysis/common.h"
#include       "util/common.h"
#include "translator/common.h"
#include  "optimizer/common.h"

void initGlobal();
void initBackend();
void initStats();
void initTablesByQid(I id);

#ifdef	__cplusplus
}
#endif

#endif
