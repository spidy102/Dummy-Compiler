/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

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
    int line_start;
    int line_end;
} astNode;

#endif