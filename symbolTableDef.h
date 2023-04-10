#ifndef SYMBOLTABLEDEF_H
#define SYMBOLTABLE_H

#include "ast.h"
#include "symbolTable.h"

extern SymTablePointer *globalSymbolTable;
extern bool semanticallyCorrect;
extern bool semanticRulesPassed;
void populateGlobalSymbolTable(SymTablePointer *global, astNode *astRoot, int offset);
void populateModuleSymbolTable(SymTablePointer *module, astNode *root, int *offset);
void populateStmtsSymTable(SymTablePointer *module, astNode *stmts, int *offset);
void traverse_ast(astNode *root, SymTablePointer *st);
SymTablePointer *initSymTablePointer();
list *getListFromAST(astNode *pl, int *offset);
types getType(astNode *type);
SymTablePointer *append_scope_pointer(SymTablePointer *head, SymTablePointer *append);
int getOffset(SymTablePointer *pointer, int *offset);
void populateTypeInformation(SymTablePointer *pointer, astNode *temp, SymTablePointer *parent);
bool compareForArrayMatch(SymTablePointer *ptr1, SymTablePointer *ptr2, int line);
<<<<<<< HEAD
void printSymbolTable(SymTablePointer *pointer);
=======
void printSymbolTable(SymTablePointer *symTable);
>>>>>>> cd9fc5291a4c95f704a65d457cd7a65f9b8de89b

#endif