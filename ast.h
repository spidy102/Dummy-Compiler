#ifndef AST_H
#define AST_H

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
} astNode;

#endif