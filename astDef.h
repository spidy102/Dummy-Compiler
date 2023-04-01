#ifndef ASTDEF_H
#define ASTDEF_H
#include "tree.h"

astNode *constructAST(treenode *root);
treenode *getNodeWithSymbol(nonTerminal nt, treenode *rootPass);
void freeRHSList(treenode *root);
char *EnumToASTString(astNodeLabel nt);
astNode *initASTNode(astNodeLabel label, astNode *leftChild);
astNode *append_at_end(astNode *head, astNode *append);
void inorder_ast(astNode *root);

#endif