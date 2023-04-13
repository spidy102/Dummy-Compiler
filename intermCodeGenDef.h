/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef INTERMCODEGENDEF_H
#define INTERMCODEGEN_H

#include "ast.h"
#include "astDef.h"
#include "intermedCodeGen.h"

char *EnumToOperatorString(operators nt);
void startIntermCodeGen(astNode *root);
quadruple *stmtsCodeGen(astNode *stmts, SymTablePointer *symTable);
quadruple *generateSwitchCaseCode(astNode *stmts);
void getGen(astNode *node, SymTablePointer *symTable);
quadruple *generateForLoopCode(astNode *stmts);
quadruple *generateWhileLoopCode(astNode *stmts);
void getExpressionsCode(astNode *expr, SymTablePointer *symTable);
int updateOffsets(char *str, SymTablePointer *symTable, types type);
SymTablePointer *getModulesSymTable(SymTablePointer *symTable);
quadruple *appendAtEnd(quadruple *head, quadruple *append);
quadruple *initQuadruple();
int newLabel();
int newTemp();

#endif