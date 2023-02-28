
#ifndef TREE_H
#define TREE_H

#include "../parser.h"
#include <stdbool.h>

typedef struct Symbol
{
    union
    {
        nonTerminal nt;
        token_names t;
    };
    bool isTerminal;
} Symbol;

typedef struct treenode
{
    Symbol node;
    struct treenode *child;
    struct treenode *nextSibling;
    token *tk;
} treenode;

treenode *initNode(Symbol sym);

treenode *addSibling(treenode *node, Symbol sym);

treenode *addChild(treenode *node, Symbol sym);

#endif