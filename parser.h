#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "./data_structures/set.h"
#include "./data_structures/token_name.h"

#define NUM_RULES 145
#define NON_TERMINALS 74
#define TERMINALS 55

typedef enum
{
#include "nonterms1.txt"
} nonTerminal;

typedef struct ruleNode
{
    // stores the entry as a string and contains a pointer to the next node in a rule
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