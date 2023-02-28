#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "tree.h"
#include "stack.h"

stack *initStack()
{
    stack *st = malloc(sizeof(stack));
    st->top = NULL;
    return st;
}

treenode *top(stack *st)
{
    if (!isEmpty(st))
        return st->top->symbol;
    return NULL;
}

st_element *getTopPointer(stack *st)
{
    return st->top;
}

bool push(stack *st, treenode *node)
{
    st_element *currentTop = getTopPointer(st);
    st_element *newTop = malloc(sizeof(st_element));
    newTop->symbol = node;
    newTop->next = malloc(sizeof(st_element));
    newTop->next = currentTop;
    st->top = newTop;
    return true;
}

treenode *pop(stack *st)
{
    if (isEmpty(st))
        return NULL;
    st_element *currentTop = getTopPointer(st);
    st->top = currentTop->next;
    return currentTop->symbol;
}

bool isEmpty(stack *st)
{
    if (st->top == NULL)
        return true;
    return false;
}

void pushRecursive(stack *st, ruleNode *rule, treenode *parent)
{
    if (rule != NULL)
    {
        pushRecursive(st, rule->nextPtr, parent);
        Symbol sym;
        sym.isTerminal = rule->isTerminal;
        if (rule->isTerminal)
        {
            sym.t = rule->t;
        }
        else
        {
            sym.nt = rule->nt;
        }
        treenode *node = initNode(sym);
        if (rule->nextPtr != NULL)
        {
            treenode *sibling = top(st);
            node->nextSibling = sibling;
        }
        node->parent = parent;
        push(st, node);
    }
}

void printStack(stack *st)
{
    st_element *getTop = getTopPointer(st);
    while (getTop != NULL)
    {
        if (getTop->symbol->node.isTerminal)
        {
            printf("terminal: %d\n", getTop->symbol->node.t);
        }
        else
        {
            printf("nonterminal: %d\n", getTop->symbol->node.nt);
        }
        getTop = getTop->next;
    }
}