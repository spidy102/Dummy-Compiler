#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "../parser.h"
typedef struct st_el
{
    ruleNode *rule;
    struct st_el *next;
} st_element;

typedef struct STACK
{
    st_element *top;
} stack;

stack *initStack();

ruleNode *top(stack *st);

st_element *getTopPointer(stack *st);

bool push(stack *st, ruleNode *rule);

ruleNode *pop(stack *st);

bool isEmpty(stack *st);

#endif