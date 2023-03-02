/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef TREE_H
#define TREE_H

#include "parser.h"
#include "lexerDef.h"
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
    struct treenode *parent;
    token *tk;
} treenode;

treenode *initNode(Symbol sym);

treenode *addSibling(treenode *node, Symbol sym);

treenode *addChild(treenode *node, Symbol sym);

#endif