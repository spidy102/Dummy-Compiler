#include "tree.h"
#include <stdlib.h>

treenode *initTree(char rep)
{
    treenode *root = malloc(sizeof(treenode *));
    root->rep = rep;
    root->child = NULL;
    root->nextSibling = NULL;
}

void addSibling(treenode *node, char rep)
{
    treenode *nextSibling = node->nextSibling;
    while (nextSibling->nextSibling != NULL)
    {
        nextSibling = nextSibling->nextSibling;
    }
    treenode *newSibling = malloc(sizeof(treenode *));
    newSibling->rep = rep;
    nextSibling->nextSibling = newSibling;
    newSibling->nextSibling = NULL;
}
