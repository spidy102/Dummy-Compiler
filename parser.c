/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include "token_name.h"
#include "hashmap.h"
#include "lexerDef.h"
#include "parserDef.h"
#include "twinbuffer.h"
#include "stack.h"
#include "tree.h"

ruleNode *grammarHeadArray[NUM_RULES];

ull firsts[NON_TERMINALS + 1];
ull follows[NON_TERMINALS + 1];

ruleNode *parseTable[NON_TERMINALS][TERMINALS];

bool isSyntaticallyCorrect = true;

char *EnumToNTString(nonTerminal nt)
{
    int i = 0;
    FILE *fp = fopen("nonterms.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open");
        exit(0);
    }
    char *buffer = malloc(sizeof(char) * 100);
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 2] = '\0';
        // printf("%s\n", buffer);

        if (nt == i)
        {
            fclose(fp);
            return buffer;
        }
        i++;
    }
    fclose(fp);
}

char *EnumToTString(token_names nt)
{
    int i = 0;
    FILE *fp = fopen("tokens.txt", "r");
    char *buffer = malloc(sizeof(char) * 100);
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 3] = '\0';
        // printf("%s\n", buffer);

        if (nt == i)
        {
            fclose(fp);
            return buffer;
        }
        i++;
    }
    fclose(fp);
}

nonTerminal NTStringMappedEnum(char *str)
{

    int i = 0;
    FILE *fp = fopen("nonterms.txt", "r");
    char buffer[100];
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 2] = '\0';
        // printf("%s\n", buffer);
        if (strcmp(buffer, str) == 0)
        {
            fclose(fp);
            return i;
        }
        i++;
    }
    fclose(fp);
    return -1;
}

token_names TStringMappedEnum(char *str)
{
    int i = 0;
    FILE *fp = fopen("tokens.txt", "r");
    char buffer[100];

    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 3] = '\0';
        if (strcmp(buffer, str) == 0)
        {
            fclose(fp);
            return i;
        }
        i++;
    }
    fclose(fp);
    return -1;
}

bool isTerminal(char *str)
{
    if (str[0] >= 'A' && str[0] <= 'Z')
        return true;
    return false;
}

void fill_grammar(FILE *fp)
{
    char buffer[200];
    int i = 0;
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 2] = '\0';
        grammarHeadArray[i] = malloc(sizeof(ruleNode));
        ruleNode *ptr = grammarHeadArray[i];
        char *tk = strtok(buffer, " \n");

        while (tk != NULL)
        {
            if (isTerminal(tk))
            {

                // if (tk[strlen(tk) - 1] == '\n')
                // {
                //     tk[strlen(tk) - 1] = '\0';
                // }
                ptr->t = TStringMappedEnum(tk);
                ptr->isTerminal = true;
            }
            else
            {
                // if (tk[strlen(tk) - 1] == '\n')
                // {
                //     tk[strlen(tk) - 1] = '\0';
                // }
                ptr->nt = NTStringMappedEnum(tk);
                ptr->isTerminal = false;
            }
            tk = strtok(NULL, " \n");
            if (tk == NULL)
            {

                ptr->nextPtr = NULL;
                break;
            }
            else
            {

                ptr->nextPtr = malloc(sizeof(ruleNode));
                ptr = ptr->nextPtr;
            }
        }

        i++;
    }
}

void getFirstSets(nonTerminal nT)
{

    for (int i = 0; i < NUM_RULES; i++)
    {
        if (grammarHeadArray[i]->nt == nT)
        {
            ruleNode *SymOnRHS = grammarHeadArray[i]->nextPtr;
            while (1)
            {
                if (SymOnRHS->isTerminal)
                {
                    add_in_set(&firsts[nT], SymOnRHS->t);
                    break;
                }
                else
                {
                    getFirstSets(SymOnRHS->nt);
                    if (contains_in_set(&firsts[SymOnRHS->nt], EPSILON))
                    {
                        union_two_sets(&firsts[nT], &firsts[nT], &firsts[SymOnRHS->nt]);
                        if (SymOnRHS->nextPtr == NULL)
                        {
                            break;
                        }
                        else
                        {
                            remove_from_set(&firsts[nT], EPSILON);
                            SymOnRHS = SymOnRHS->nextPtr;
                        }
                    }
                    else
                    {
                        union_two_sets(&firsts[nT], &firsts[nT], &firsts[SymOnRHS->nt]);
                        break;
                    }
                }
            }
        }
    }
}

ull getFirstSetsOneRule(nonTerminal nT, int i)
{
    ull first_set = 0;

    if (grammarHeadArray[i]->nt == nT)
    {

        ruleNode *SymOnRHS = grammarHeadArray[i]->nextPtr;
        while (1)
        {

            if (SymOnRHS->isTerminal)
            {

                add_in_set(&first_set, SymOnRHS->t);
                return first_set;
            }
            else
            {

                getFirstSets(SymOnRHS->nt);
                // print_set_elements(&firsts[SymOnRHS->nt]);
                if (contains_in_set(&firsts[SymOnRHS->nt], EPSILON))
                {
                    union_two_sets(&first_set, &first_set, &firsts[SymOnRHS->nt]);
                    if (SymOnRHS->nextPtr == NULL)
                    {
                        return first_set;
                    }
                    else
                    {
                        remove_from_set(&first_set, EPSILON);
                        SymOnRHS = SymOnRHS->nextPtr;
                    }
                }
                else
                {
                    union_two_sets(&first_set, &first_set, &firsts[SymOnRHS->nt]);
                    return first_set;
                }
            }
        }
    }
}

void getFollowSets(nonTerminal nT)
{

    for (int i = 0; i < NUM_RULES; i++)
    {
        ruleNode *head = grammarHeadArray[i];
        ruleNode *SymOnRHS = grammarHeadArray[i]->nextPtr;
        while (SymOnRHS != NULL)
        {
            if (!SymOnRHS->isTerminal)
            {
                if (SymOnRHS->nt == nT)
                {
                    bool isDone = false;
                    while (1)
                    {
                        if (SymOnRHS->nextPtr != NULL)
                        {
                            if (SymOnRHS->nextPtr->isTerminal)
                            {
                                add_in_set(&follows[nT], SymOnRHS->nextPtr->t);
                                isDone = true;
                                break;
                            }
                            else
                            {
                                getFirstSets(SymOnRHS->nextPtr->nt);
                                union_two_sets(&follows[nT], &follows[nT], &firsts[SymOnRHS->nextPtr->nt]);
                                if (contains_in_set(&firsts[SymOnRHS->nextPtr->nt], EPSILON))
                                {
                                    remove_from_set(&follows[nT], EPSILON);
                                    SymOnRHS = SymOnRHS->nextPtr;
                                }
                                else
                                {
                                    isDone = true;
                                    break;
                                }
                            }
                        }
                        else
                        {

                            if (head->nt == nT)
                            {
                                isDone = true;
                                break;
                            }
                            getFollowSets(head->nt);
                            union_two_sets(&follows[nT], &follows[nT], &follows[head->nt]);
                            isDone = true;
                            break;
                        }
                    }
                    if (isDone)
                    {
                        break;
                    }
                }
                else
                {
                    SymOnRHS = SymOnRHS->nextPtr;
                }
            }
            else
            {
                SymOnRHS = SymOnRHS->nextPtr;
            }
        }
    }
}

ruleNode *getEpsilonRule(nonTerminal nt)
{
    for (int i = 0; i < NUM_RULES; i++)
    {
        if (grammarHeadArray[i]->nt == nt)
        {
            ruleNode *rule = grammarHeadArray[i];
            if (rule->nextPtr->isTerminal && rule->nextPtr->t == EPSILON)
            {
                return rule;
            }
        }
    }
    // should not reach here
    return NULL;
}

void populateParseTable()
{
    for (int i = 0; i < NUM_RULES; i++)
    {
        // if (grammarHeadArray[i]->nextPtr->isTerminal)

        // if (grammarHeadArray[i]->nextPtr->t == EPSILON)
        // {

        //     getFollowSets(grammarHeadArray[i]->nt);
        //     int j = 0;
        //     ull temp = follows[grammarHeadArray[i]->nt];
        //     while (temp != 0)
        //     {
        //         if (temp % 2 == 1)
        //         {
        //             parseTable[grammarHeadArray[i]->nt][j] = grammarHeadArray[i];
        //         }
        //         temp /= 2;
        //         j++;
        //     }
        // }
        ull first_set = getFirstSetsOneRule(grammarHeadArray[i]->nt, i);
        if (contains_in_set(&first_set, EPSILON))
        {
            // remove_from_set(&first_set, EPSILON);
            getFollowSets(grammarHeadArray[i]->nt);
            ull follow_set = follows[grammarHeadArray[i]->nt];
            // if (contains_in_set(&follow_set, EPSILON))
            // {
            //     remove_from_set(&follow_set, EPSILON);
            // }
            int j = 0;
            ruleNode *epsilonRule = getEpsilonRule(grammarHeadArray[i]->nt);
            // if (epsilonRule == NULL)
            // {
            //     printf("hello we have a problem at %s non terminal\n", EnumToNTString(grammarHeadArray[i]->nt));
            // }
            while (follow_set != 0)
            {
                if (follow_set % 2 == 1)
                {
                    // printf("what number:%d, nt: %s\n", i, EnumToNTString(grammarHeadArray[i]->nt));
                    // ruleNode *new_rule = malloc(sizeof(ruleNode));
                    // new_rule->isTerminal = false;
                    // new_rule->nt = grammarHeadArray[i]->nt;
                    // new_rule->nextPtr = malloc(sizeof(ruleNode));
                    // new_rule->nextPtr->isTerminal = true;
                    // new_rule->nextPtr->t = EPSILON;
                    // new_rule->nextPtr->nextPtr = NULL;
                    parseTable[grammarHeadArray[i]->nt][j] = epsilonRule;
                }
                follow_set /= 2;
                j++;
            }
        }
        int j = 0;
        ull temp = first_set;
        while (temp != 0)
        {
            if (temp % 2 == 1)
            {
                parseTable[grammarHeadArray[i]->nt][j] = grammarHeadArray[i];
            }
            temp /= 2;
            j++;
        }
    }
}

void printParseTable()
{
    for (int i = 0; i < NON_TERMINALS; i++)
    {
        for (int j = 0; j < TERMINALS; j++)
        {
            if (parseTable[i][j] == NULL)
            {
                // printf("No rule ");
            }
            else if (parseTable[i][j]->nextPtr->isTerminal)
            {
                printf("terminal:%d i:%d,j:%d \n", parseTable[i][j]->nextPtr->t, i, j);
            }
            else if (!parseTable[i][j]->nextPtr->isTerminal)
            {

                printf("nonterminal:%d i:%d,j:%d \n ", parseTable[i][j]->nextPtr->nt, i, j);
            }
        }
        printf("\n");
    }
}

char *tolowercase(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] = (char)tolower(str[i]);
    }
    return str;
}

int getRuleNumber(ruleNode *rule)
{
    for (int i = 0; i < NUM_RULES; i++)
    {

        if (grammarHeadArray[i] == rule)
        {
            return i;
        }
    }
    // should not reach here
    return -1;
}

treenode *parseInputSourceCode(FILE *fp, twinbuffer *tb, hashtable ht)
{
    line_num = 1;
    stack *st = initStack();
    token *lookAhead = getNextToken(ht, tb);
    Symbol startSymbol;
    startSymbol.isTerminal = false;
    startSymbol.nt = program;
    treenode *start = initNode(startSymbol);
    start->parent = initNode(startSymbol);
    push(st, start);
    // printf("%s", lookAhead->str);
    while (lookAhead != NULL)
    {
        // printf("token passed?%s\n", EnumToTString(lookAhead->token));
        treenode *top1 = top(st);
        if (top1 == NULL)
        {
            // report error E4

            printf("Error at line number %d: top of the stack is empty\n", lookAhead->line_num);
            isSyntaticallyCorrect = false;
            return start;
        }
        else if (top1->node.isTerminal)
        {
            if (top1->node.t == lookAhead->token)
            {
                treenode *parent = pop(st);
                parent->tk = lookAhead;
                lookAhead = getNextToken(ht, tb);
            }
            else
            {
                // report error e1;
                if (lookAhead->token == NUM)
                {
                    char *nt = EnumToTString(top1->node.t);
                    printf("Error: %s was expected at line number %d, got %d instead\n", tolowercase(nt), lookAhead->line_num, lookAhead->integer);
                    isSyntaticallyCorrect = false;
                    free(nt);
                }

                else if (lookAhead->token == RNUM)
                {
                    char *t = EnumToTString(top1->node.t);
                    printf("Error: %s was expected at line number %d, got %lf instead\n", tolowercase(t), lookAhead->line_num, lookAhead->rnum);
                    isSyntaticallyCorrect = false;
                    free(t);
                }

                else
                {
                    char *t = EnumToTString(top1->node.t);
                    printf("Error: %s was expected at line number %d, got %s instead\n", tolowercase(t), lookAhead->line_num, lookAhead->str);
                    isSyntaticallyCorrect = false;
                    free(t);
                }

                // error recovery
                // printf("Stack:\n");
                // printStack(st);

                treenode *temp = pop(st);

                temp->tk = lookAhead;
                // lookAhead = getNextToken(ht, tb);

                if (temp == NULL)
                {
                    return start;
                }
            }
        }
        else if (!top1->node.isTerminal)
        {
            ruleNode *rule = parseTable[top1->node.nt][lookAhead->token];

            ruleNode *temp = rule;

            // int ruleNo = getRuleNumber(rule);

            // printf("Rule:\n");
            // while (temp != NULL)
            // {
            //     if (temp->isTerminal)
            //     {
            //         printf("terminal: %s\n", EnumToTString(temp->t));
            //     }
            //     else
            //     {
            //         printf("non terminal: %s\n", EnumToNTString(temp->nt));
            //     }
            //     temp = temp->nextPtr;
            // }

            // printf("\n");

            if (parseTable[top1->node.nt][lookAhead->token] != NULL)
            {
                // excluding epsilon
                if (rule->nextPtr->isTerminal && rule->nextPtr->t == EPSILON)
                {
                    treenode *parent = pop(st);

                    parent->rule_No = getRuleNumber(parseTable[top1->node.nt][lookAhead->token]);
                    // adding epsilon in the leaf nodes
                    Symbol sym;
                    sym.isTerminal = true;
                    sym.t = EPSILON;
                    treenode *epsilonNode = initNode(sym);
                    parent->child = epsilonNode;
                    epsilonNode->parent = parent;
                    continue;
                }
                treenode *parent = pop(st);
                ruleNode *rule = parseTable[top1->node.nt][lookAhead->token]->nextPtr;
                int ruleNo = getRuleNumber(parseTable[top1->node.nt][lookAhead->token]);
                parent->rule_No = ruleNo;
                pushRecursive(st, rule, parent);
                treenode *child = top(st);
                parent->child = child;
                child->parent = parent;
            }
            else
            {
                // report error e2
                // printf("Stack:\n");
                // printStack(st);
                printf("Error at line number %d: Rule entry in the parse table is empty!\n", lookAhead->line_num);
                isSyntaticallyCorrect = false;
                // printf("%s %s", EnumToNTString(top1->node.nt), EnumToTString(lookAhead->token));
                ull synchronisation_set = 0;
                // getFirstSets(top1->node.nt);
                // union_two_sets(&synchronisation_set, &synchronisation_set, &firsts[top1->node.nt]);
                getFollowSets(top1->node.nt);
                union_two_sets(&synchronisation_set, &synchronisation_set, &follows[top1->node.nt]);
                // being extra cautious in the sync set
                // add_in_set(&synchronisation_set, START);
                // add_in_set(&synchronisation_set, END);
                // add_in_set(&synchronisation_set, ENDDEF);
                // add_in_set(&synchronisation_set, DRIVERENDDEF);

                // print_set_elements(&synchronisation_set);

                while (lookAhead != NULL && !contains_in_set(&synchronisation_set, lookAhead->token))
                {
                    lookAhead = getNextToken(ht, tb);
                }
                // lookAhead = getNextToken(ht, tb);
                pop(st);
                if (top(st) == NULL)
                {
                    return start;
                }
            }
        }
    }
    // printStack(st);

    if (!isEmpty(st))
    {
        // report error E3;
        treenode *top1 = top(st);
        while (top1 != NULL && !top1->node.isTerminal)
        {
            if (parseTable[top1->node.nt][EPSILON] != NULL)
            {

                Symbol sym;
                sym.isTerminal = true;
                sym.t = EPSILON;
                top1->child = initNode(sym);
                top1->child->parent = top1;
                treenode *parent = pop(st);

                parent->rule_No = getRuleNumber(parseTable[top1->node.nt][EPSILON]);
                top1 = top(st);
            }
            else
            {
                break;
            }
        }
        if (!isEmpty(st))
        {
            printf("Error: Input source code has been consumed but stack is non empty\n");
            isSyntaticallyCorrect = false;
        }
        // exit(0);
    }
    return start;
}

void freeList(ruleNode *head)
{
    ruleNode *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->nextPtr;
        free(temp);
    }
}

void freeGrammar()
{
    for (int i = 0; i < NUM_RULES; i++)
    {
        freeList(grammarHeadArray[i]);
    }
}

void printParseTree(treenode *root, FILE *fp)
{

    if (root == NULL)
    {
        return;
    }
    printParseTree(root->child, fp);

    if (root->node.isTerminal)
    {
        if (root->node.t == EPSILON)
        {
            char *nt = EnumToNTString(root->parent->node.nt);
            char *t = EnumToTString(root->node.t);

            fprintf(fp, "lexeme: ----- line number: ----- token_name: ----- value: ----- parentNode: %5s isLeaf:  Yes NodeSymbol: %s\n", nt, t);
            free(nt);
            free(t);
        }
        else
        {

            token *tk = root->tk;
            if (tk->token == NUM)
            {
                char *nt = EnumToNTString(root->parent->node.nt);
                char *t = EnumToTString(tk->token);
                fprintf(fp, "lexeme: ----- line number: %5d token_name: %5s value: %5d parentNode: %5s isLeaf:  Yes\n", tk->line_num, t, tk->integer, nt);
                free(nt);
                free(t);
            }
            else if (tk->token == RNUM)
            {
                char *nt = EnumToNTString(root->parent->node.nt);
                char *t = EnumToTString(tk->token);
                fprintf(fp, "lexeme: ----- line number: %5d token_name: %5s value: %5lf parentNode: %s isLeaf:  Yes\n", tk->line_num, t, tk->rnum, nt);
                free(nt);
                free(t);
            }

            else
            {
                char *nt = EnumToNTString(root->parent->node.nt);
                char *t = EnumToTString(tk->token);
                fprintf(fp, "lexeme: %5s line number: %5d token_name: %5s value: ----- parentNode: %s isLeaf:  Yes\n", tk->str, tk->line_num, t, nt);
                free(nt);
                free(t);
            }
        }
        // fprintf(fp, "TERMINAL: %s\n", EnumToTString(root->tk->token));
    }
    else
    {
        char *nt = EnumToNTString(root->parent->node.nt);
        char *t = EnumToNTString(root->node.nt);
        // if (node)
        if (root->node.nt == program)
        {
            fprintf(fp, "lexeme: ----- line number: ----- token_name: ----- value: ----- parentNode: %5s isLeaf:   No NodeSymbol: %s\n", "ROOT", t);
        }
        else
            fprintf(fp, "lexeme: ----- line number: ----- token_name: ----- value: ----- parentNode: %5s isLeaf:   No NodeSymbol: %5s\n", nt, t);
        free(nt);
        free(t);
        // fprintf(fp, "NON-TERMINAL: %s\n", EnumToNTString(root->node.nt));
    }
    if (root->child != NULL)
    {
        treenode *temp = root->child->nextSibling;
        while (temp != NULL)
        {
            printParseTree(temp, fp);
            temp = temp->nextSibling;
        }
        free(temp);
    }
}

void computeFirstAndFollowSets()
{
    for (int i = 0; i <= NON_TERMINALS; i++)
    {

        firsts[i] = (ull)0;
        printf("%d\nhello", i);
        getFirstSets(i);
        follows[i] = (ull)0;
        getFollowSets(i);
    }
}

// int main()
// {
//     for (int i = 0; i <= NON_TERMINALS; i++)
//     {
//         firsts[i] = (ull)0;
//         follows[i] = (ull)0;
//     }

//     FILE *fp = fopen("Grammar.txt", "r");
//     fill_grammar(fp);

//     computeFirstAndFollowSets();
//     // printf("computed");

//     FILE *fp1 = fopen("example.erp", "r");

//     FILE *fp2 = fopen("parseTree.txt", "w");

//     populateParseTable();
//     twinbuffer *tb = twinbuffer_init(fp1, 512);
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
//     treenode *root = parseInputSourceCode(fp1, tb, ht);

//     printParseTree(root, fp2);

//     // printf("%s", EnumToTString(1));
//     printf("ran well!!\n");
//     //  ruleNode *ptr = grammarHeadArray[62];

//     // while (ptr != NULL)
//     // {
//     //     if (ptr->isTerminal)
//     //     {
//     //         printf("terminal: %d\n", ptr->t);
//     //     }
//     //     else
//     //     {
//     //         printf("non terminal %d\n", ptr->nt);
//     //     }
//     //     ptr = ptr->nextPtr;
//     // }
//     // printf("%d", n9);
//     // getFirstSets(whichStmt);
//     // getFollowSets(whichStmt);
//     // // ull first_set = getFirstSetsOneRule(n20, 61);
//     // print_set_elements(&firsts[whichStmt]);
//     // print_set_elements(&follows[whichStmt]);

//     // getFirstSets(factor);
//     // print_set_elements(&firsts[factor]);

//     // getFirstSets(statement);
//     // print_set_elements(&firsts[statement]);

//     // printParseTable();
//     freeGrammar();
// }