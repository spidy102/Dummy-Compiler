#ifndef TYPECHECKERDEF_H
#define TYPECHECKERDEF_H

#include "symbolTable.h"
#include "ast.h"

void checkBounds(astNode *arr_ele, SymTablePointer *ptr, SymTablePointer *parent, bool printErrors);
void getAttributeType(astNode *node, SymTablePointer *symTable, bool printErors);
char *EnumToTypeString(types type);
void compareActualAndFormalParams(list *ipl, astNode *actual_params, SymTablePointer *symTable, bool printErrors);
void compareRetParams(list *opl, astNode *retParams, SymTablePointer *symTable, int line, bool printErrors);
void checkIfLoopStmtsDoNotRedefineLoopVariable(astNode *stmts, astNode *loopVar, bool printErrors);
void checkTypesForModule(SymTablePointer *symTable, astNode *stmts, bool printErrors);
void checkIfOutputParametersAreAssigned(astNode *stmts, SymTablePointer *module, bool printErrors);
void typeCheck(astNode *root, bool printErrors);

#endif