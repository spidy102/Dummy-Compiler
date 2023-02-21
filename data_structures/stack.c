#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

stack *initStack()
{
    stack *st = malloc(sizeof(stack *));
    st->top = NULL;
    return st;
}

char top(stack *st)
{
    if (!isEmpty(st))
        return st->top->NTOrT;
    return ' ';
}

st_element *getTopPointer(stack *st)
{
    return st->top;
}

bool push(stack *st, char NTOrT)
{
    st_element *currentTop = getTopPointer(st);
    st_element *newTop = malloc(sizeof(st_element *));
    newTop->NTOrT = NTOrT;
    newTop->next = malloc(sizeof(st_element *));
    newTop->next = currentTop;
    st->top = newTop;
    return true;
}

char pop(stack *st)
{
    if (isEmpty(st))
        return ' ';
    st_element *currentTop = getTopPointer(st);
    st->top = currentTop->next;
    return currentTop->NTOrT;
}

bool isEmpty(stack *st)
{
    if (st->top == NULL)
        return true;
    return false;
}