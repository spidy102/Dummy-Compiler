/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef __PARSER_DEF_H__
#define __PARSER_DEF_H__

#include "parser.h"
#include "set.h"
#include "tree.h"

#define NUM_RULES 151
#define NON_TERMINALS 78
#define TERMINALS 57

extern ruleNode *grammarHeadArray[NUM_RULES];

extern ull firsts[NON_TERMINALS + 1];
extern ull follows[NON_TERMINALS + 1];

extern ruleNode *parseTable[NON_TERMINALS][TERMINALS];

char *EnumToNTString(nonTerminal nt);
char *EnumToTString(token_names nt);
void printParseTree(treenode *root, FILE *fp);
void freeGrammar();
void freeList(ruleNode *head);
treenode *parseInputSourceCode(FILE *fp, twinbuffer *tb, hashtable ht);
void printParseTable();
void populateParseTable();
void getFollowSets(nonTerminal nT);
ull getFirstSetsOneRule(nonTerminal nT, int i);
void getFirstSets(nonTerminal nT);
void fill_grammar(FILE *fp);
bool isTerminal(char *str);
token_names TStringMappedEnum(char *str);
nonTerminal NTStringMappedEnum(char *str);
void computeFirstAndFollowSets();

#endif