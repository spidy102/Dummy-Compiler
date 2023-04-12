#ifndef INTERMEDCODEGEN_H
#define INTERMEDCODEGEN_H

#include "token_name.h"

typedef enum
{
#include "operators.txt"
} operators;

typedef struct quadruple
{
    operators op;
    char resultant[25];
    int offsetRes;
    SymTablePointer *resPtr;
    char operand1[25];
    int offsetOperand1;
    SymTablePointer *op1Ptr;
    char operand2[25];
    int offsetOperand2;
    SymTablePointer *op2Ptr;
    struct quadruple *next;
} quadruple;

extern quadruple *globalHead;
extern int labelCount;
extern int tempCount;

#endif