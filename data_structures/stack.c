#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

stack *initStack()
{
    stack *st = malloc(sizeof(stack *));
    st->top = NULL;
    return st;
}

ruleNode *top(stack *st)
{
    if (!isEmpty(st))
        return st->top->rule;
    return NULL;
}

st_element *getTopPointer(stack *st)
{
    return st->top;
}

bool push(stack *st, ruleNode *rule)
{
    st_element *currentTop = getTopPointer(st);
    st_element *newTop = malloc(sizeof(st_element *));
    newTop->rule = rule;
    newTop->next = malloc(sizeof(st_element *));
    newTop->next = currentTop;
    st->top = newTop;
    return true;
}

ruleNode *pop(stack *st)
{
    if (isEmpty(st))
        return NULL;
    st_element *currentTop = getTopPointer(st);
    st->top = currentTop->next;
    return currentTop->rule;
}

bool isEmpty(stack *st)
{
    if (st->top == NULL)
        return true;
    return false;
}