#ifndef AST_H
#define AST_H

#include "symbolTable.h"
#include "intermedCodeGen.h"

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
    quadruple *code;
    // need temporaries
    char name[25];
    int trueCase;
    int falseCase;
} astNode;

#endif