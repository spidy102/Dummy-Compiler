/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

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