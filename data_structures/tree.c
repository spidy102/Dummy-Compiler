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
