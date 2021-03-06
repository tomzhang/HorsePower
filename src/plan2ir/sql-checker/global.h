#ifndef H_GLOBAL_H
#define H_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define P printf
#define R return
#define SP sprintf
#define FP fprintf
#define PN(x)     P("%s\n",x)
#define newLine() P("\n")
#define INFO(s)   FP(stderr, s "(%s:%d:%s):\n\t", __FILE__, __LINE__, __func__)
#define WP(...)   FP(stderr,__VA_ARGS__)  // warning
#define EP(...)   do{INFO("[ERROR]"); FP(stderr,__VA_ARGS__); exit(1);}while(0)  // error
#define TODO(...) do{INFO("[TODO]");  FP(stderr,__VA_ARGS__); exit(1);}while(0)  // todo

#endif
