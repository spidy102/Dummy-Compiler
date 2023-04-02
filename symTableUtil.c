#include <stdbool.h>
#include "symbolTable.h"
#include <stdio.h>
#include <string.h>
#include "symTableUtil.h"

bool existsInAnySymTable(SymTablePointer *st, char *str)
{
    while (st->typeST != GLOBALST)
    {

        if (existsInSymTable(st->corrHashtable, str))
        {
            return true;
        }

        st = st->parentHashTable;
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