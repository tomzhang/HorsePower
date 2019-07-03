#ifndef __H_TRANSLATOR__
#define __H_TRANSLATOR__

/* declaration */

/* Interpreter */
I HorseInterpreter();
O runInterpreter();
V fetchVector(Node *n);

/* Compiler */
I HorseCompilerNaive();
I HorseCompilerOptimized();

/* Interpreter v2 */
I HorseInterpreter2();


/* helper functions */
#define totalVar totalElement
I getHType(Type t);
O loadConst(Node *n, V x, L k, I t);

/* code gen functions (need code and ptr) */
#define glueCode(s)  strcat(ptr,s)
#define glueLine()   strcat(ptr,"\n")
#define cleanCode()  code[0]=0
#define resetCode()  if(ptr[0]!=0) ptr+=strlen(ptr)
#define glueChar(c)  do{resetCode(); ptr[0]=c; ptr[1]=0; ptr++;}while(0)
#define glueInt(x)   do{resetCode(); SP(ptr, "%d", x);     }while(0)
#define glueLong(x)  do{resetCode(); SP(ptr, "%lld", x);   }while(0)
#define glueAny(...) do{resetCode(); SP(ptr, __VA_ARGS__); }while(0)
#define glueCodeLine(x)  do{genIndent(); resetCode(); SP(ptr, "%s\n",x); }while(0)
#define glueAnyLine(...) do{genIndent(); glueAny(__VA_ARGS__);glueLine();}while(0)

#endif
