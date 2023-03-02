/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "parser.h"
#include "tree.h"

typedef struct st_el
{
    treenode *symbol;
    struct st_el *next;
} st_element;

typedef struct STACK
{
    st_element *top;
} stack;

stack *initStack();

treenode *top(stack *st);

st_element *getTopPointer(stack *st);

bool push(stack *st, treenode *node);

treenode *pop(stack *st);

bool isEmpty(stack *st);

void printStack(stack *st);

void pushRecursive(stack *st, ruleNode *rule, treenode *parent);

#endif