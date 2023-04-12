#ifndef SYMBOLTABLEDEF_H
#define SYMBOLTABLE_H

#include "ast.h"
#include "symbolTable.h"

extern SymTablePointer *globalSymbolTable;
extern bool semanticallyCorrect;
extern bool semanticRulesPassed;
extern bool arrayChecks;
void populateGlobalSymbolTable(SymTablePointer *global, astNode *astRoot, int offset, bool printErrors);
void populateModuleSymbolTable(SymTablePointer *module, astNode *root, int *offset, bool printErrors);
void populateStmtsSymTable(SymTablePointer *module, astNode *stmts, int *offset, bool printErrors);
void traverse_ast(astNode *root, SymTablePointer *st, bool printErrors);
SymTablePointer *initSymTablePointer();
list *getListFromAST(astNode *pl, int *offset);
types getType(astNode *type);
SymTablePointer *append_scope_pointer(SymTablePointer *head, SymTablePointer *append);
int getOffset(SymTablePointer *pointer, int *offset);
void populateTypeInformation(SymTablePointer *pointer, astNode *temp, SymTablePointer *parent, bool printErrors);
bool compareForArrayMatch(SymTablePointer *ptr1, SymTablePointer *ptr2, int line, bool printErrors);
void printSymbolTable(SymTablePointer *symTable, int nest);
void getActivationRecords();
void printArrayElements(SymTablePointer *symTable);

#endif