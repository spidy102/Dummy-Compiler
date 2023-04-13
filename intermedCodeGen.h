/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

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