#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>
#include "token_name.h"

typedef enum
{
#include "nonterms.txt"
} nonTerminal;

typedef struct ruleNode
{
    union
    {
        nonTerminal nt;
        token_names t;
    };
    struct ruleNode *nextPtr;
    bool isTerminal;
} ruleNode;

#endif