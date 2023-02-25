#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

nonTerminal NTStringMappedEnum(char *str)
{

    int i = 0;
    FILE *fp = fopen("nonterms.txt", "r");
    char buffer[100];
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 3] = '\0';
        if (strcmp(buffer, str) == 0)
        {

            return i;
        }
        i++;
    }

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
            return i;
        }
        i++;
    }

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
        grammarHeadArray[i] = malloc(sizeof(ruleNode *));
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

                ptr->nextPtr = malloc(sizeof(ruleNode *));
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

void getFollowSets(nonTerminal nT)
{
    for (int i = 0; i < NUM_RULES; i++)
    {
        ruleNode *head = grammarHeadArray[i];
        ruleNode *SymOnRHS = grammarHeadArray[i]->nextPtr;
        while (SymOnRHS != NULL)
        {
            // printf("hemlo??");
            if (!SymOnRHS->isTerminal)
            {
                if (SymOnRHS->nt == nT)
                {
                    printf("which rule: %d %d\n", i, nT);
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
                            if (!SymOnRHS->isTerminal && SymOnRHS->nt == nT)
                            {
                                isDone = true;
                                break;
                            }
                            getFollowSets(head->nt);
                            union_two_sets(&follows[nT], &follows[nT], &follows[head->nt]);
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

int main()
{
    for (int i = 0; i <= NON_TERMINALS; i++)
    {
        firsts[i] = (ull)0;
    }
    FILE *fp = fopen("Grammar.txt", "r");
    fill_grammar(fp);
    // ruleNode *ptr = grammarHeadArray[3];
    // while (ptr != NULL)
    // {
    //     if (ptr->isTerminal)
    //     {
    //         printf("terminal: %d\n", ptr->t);
    //     }
    //     else
    //     {
    //         printf("non terminal %d\n", ptr->nt);
    //     }
    //     ptr = ptr->nextPtr;
    // }
    // printf("%d", grammarHeadArray[108]->nt);

    getFollowSets(moduleDeclarations);

    print_set_elements(&follows[moduleDeclarations]);
    freeGrammar();
}