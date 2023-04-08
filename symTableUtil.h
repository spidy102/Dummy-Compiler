
#ifndef SYMTABLEUTIL_H
#define SYMTABLEUTIL_H

#include "symbolTable.h"

bool insertSymTable(hashtable *ht, SymTablePointer *str);

bool existsInSymTable(hashtable *ht, char *str);

bool existsInAnySymTable(SymTablePointer *st, char *str);

SymTablePointer *getFromSymTable(hashtable *ht, char *str);

SymTablePointer *getFromAnySymTable(SymTablePointer *st, char *str);

SymTablePointer *getPointerFromList(list *ipl);

#endif