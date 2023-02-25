#include <stdbool.h>
#include "./data_structures/token_name.h"
#include "./data_structures/set.h"

#define NUM_RULES 105
#define NON_TERMINALS 54
#define TERMINALS 55

typedef enum
{
#include "nonterms.txt"
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

ruleNode *grammarHeadArray[NUM_RULES];

ull firsts[NON_TERMINALS + 1];
ull follows[NON_TERMINALS + 1];

ruleNode *parseTable[NON_TERMINALS][TERMINALS];
