#ifndef AST_H
#define AST_H

#include "symbolTable.h"

typedef enum
{
#include "astLabels.txt"
} astNodeLabel;

typedef struct astNode
{
    astNodeLabel label;
    struct astNode *leftChild;
    struct astNode *nextSibling;
    token *tk;
    bool isNegative;
    SymTablePointer *symTable;
    types type;
} astNode;

#endif