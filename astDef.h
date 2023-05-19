/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

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
int countASTnodes(astNode *root);

#endif