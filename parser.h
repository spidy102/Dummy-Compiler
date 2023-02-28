#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "./data_structures/set.h"
#include "./data_structures/token_name.h"

#define NUM_RULES 150
#define NON_TERMINALS 78
#define TERMINALS 57

typedef enum
{
#include "nonterms1.txt"
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

extern ruleNode *grammarHeadArray[NUM_RULES];

extern ull firsts[NON_TERMINALS + 1];
extern ull follows[NON_TERMINALS + 1];

extern ruleNode *parseTable[NON_TERMINALS][TERMINALS];

#endif