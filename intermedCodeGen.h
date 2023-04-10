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
    char operand1[25];
    int offsetOperand1;
    char operand2[25];
    int offsetOperand2;
    struct quadruple *next;
} quadruple;

extern quadruple *globalHead;
extern int labelCount;
extern int tempCount;

#endif