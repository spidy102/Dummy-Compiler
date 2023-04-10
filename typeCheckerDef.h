#ifndef TYPECHECKERDEF_H
#define TYPECHECKERDEF_H

#include "symbolTable.h"
#include "ast.h"

void checkBounds(astNode *arr_ele, SymTablePointer *ptr, SymTablePointer *parent);
void getAttributeType(astNode *node, SymTablePointer *symTable);
char *EnumToTypeString(types type);
void compareActualAndFormalParams(list *ipl, astNode *actual_params, SymTablePointer *symTable);
void compareRetParams(list *opl, astNode *retParams, SymTablePointer *symTable, int line);
void checkIfLoopStmtsDoNotRedefineLoopVariable(astNode *stmts, astNode *loopVar);
void checkTypesForModule(SymTablePointer *symTable, astNode *stmts);
void checkIfOutputParametersAreAssigned(astNode *stmts, SymTablePointer *module);
void typeCheck(astNode *root);

#endif