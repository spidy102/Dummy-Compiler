#include <stdbool.h>
#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symTableUtil.h"

bool existsInAnySymTable(SymTablePointer *st, char *str)
{
    SymTablePointer *temp = st;
    while (st->typeST != GLOBALST)
    {
        // printf("inside%s\n\n", st->str);
        if (existsInSymTable(st->corrHashtable, str))
        {
            return true;
        }

        st = st->parentHashTable;
    }

    SymTablePointer *moduleST = temp;
    while (moduleST->typeST != MODULEST)
    {
        moduleST = moduleST->parentHashTable;
    }

    if (strcmp(moduleST->str, "driver") != 0)
    {

        list *ipl = moduleST->input_para_list;

        while (ipl != NULL)
        {
            if (strcmp(ipl->tk->str, str) == 0)
            {
                return true;
            }
            ipl = ipl->next;
        }
    }

    return false;
}

bool insertSymTable(hashtable *ht, SymTablePointer *str)
{
    SymTablePointer *typecasted = str;
    int getHash = hash(typecasted->str, strlen(typecasted->str));
    SymTablePointer *ptr = ht->table[getHash]->bucket_ptr;

    if (ptr == NULL)
    {
        ht->table[getHash]->bucket_ptr = typecasted;
        typecasted->next = NULL;
        return true;
    }
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = typecasted;
    ptr->next->next = NULL;
    return false;
}

bool existsInSymTable(hashtable *ht, char *str)
{
    int getHash = hash(str, strlen(str));
    SymTablePointer *ptr = ht->table[getHash]->bucket_ptr;
    while (ptr != NULL)
    {

        if (strcmp(ptr->str, str) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

SymTablePointer *getFromSymTable(hashtable *ht, char *str)
{
    int getHash = hash(str, strlen(str));

    SymTablePointer *ptr = ht->table[getHash]->bucket_ptr;

    while (ptr != NULL)
    {
        if (strcmp(ptr->str, str) == 0)
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

SymTablePointer *getPointerFromList(list *ipl)
{
    SymTablePointer *ptr = malloc(sizeof(SymTablePointer));
    ptr->isArray = ipl->isArray;
    ptr->str = ipl->tk->str;
    ptr->tk = ipl->tk;
    if (ptr->isArray)
    {
        ptr->typeIfArray.high = ipl->typeIfArray.high;
        ptr->typeIfArray.low = ipl->typeIfArray.low;
        ptr->typeIfArray.high_ = ipl->typeIfArray.high_;
        ptr->typeIfArray.low_ = ipl->typeIfArray.low_;
        ptr->typeIfArray.type = ipl->typeIfArray.type;
    }
    else
    {
        ptr->typeIfNotArray = ipl->typeIfNotArray;
    }
    ptr->offset = ipl->width;
    return ptr;
}

SymTablePointer *getFromAnySymTable(SymTablePointer *st, char *str)
{
    while (st->typeST != GLOBALST)
    {
        if (existsInSymTable(st->corrHashtable, str))
        {

            return getFromSymTable(st->corrHashtable, str);
        }
        if (st->typeST == MODULEST)
        {
            list *ipl = st->input_para_list;
            while (ipl != NULL)
            {
                if (strcmp(str, ipl->tk->str) == 0)
                {
                    return getPointerFromList(ipl);
                }
                ipl = ipl->next;
            }
        }
        st = st->parentHashTable;
    }

    return NULL;
}