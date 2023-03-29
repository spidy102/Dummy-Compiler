/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#include "tree.h"
#include <stdlib.h>
#include "tree.h"

treenode *initNode(Symbol sym)
{
    treenode *root = malloc(sizeof(treenode));
    root->node = sym;
    root->child = NULL;
    root->nextSibling = NULL;
    root->parent = NULL;
    root->tk = NULL;
    root->rule_No = -1;
    root->inh = NULL;
    root->syn = NULL;
    root->addr = NULL;
}

treenode *addSibling(treenode *node, Symbol sym)
{
    treenode *nextSibling = node->nextSibling;
    while (nextSibling->nextSibling != NULL)
    {
        nextSibling = nextSibling->nextSibling;
    }
    treenode *newSibling = malloc(sizeof(treenode));
    newSibling->node = sym;
    nextSibling->nextSibling = newSibling;
    newSibling->nextSibling = NULL;
    return newSibling;
}

treenode *addChild(treenode *node, Symbol sym)
{
    node->child = initNode(sym);
    return node->child;
}
