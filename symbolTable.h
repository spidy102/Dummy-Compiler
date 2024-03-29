/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "lexerDef.h"
#include "hashmap.h"

#define INT_WIDTH 2
#define BOOL_WIDTH 1
#define REAL_WIDTH 4

typedef enum
{
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_BOOLEAN,
    TYPE_ERROR,
    TYPE_ARR_INT,
    TYPE_ARR_REAL,
    TYPE_ARR_BOOL,
    TYPE_MISSING,
} types;

typedef struct array_type
{
    int low;
    int high;
    char *highLexeme;
    bool isNegHigh;
    bool low_;
    char *lowLexeme;
    bool isNegLow;
    bool high_;
    types type;
} array_type;

typedef struct list
{
    union
    {
        types typeIfNotArray;
        array_type typeIfArray;
    };
    token *tk;
    int width;
    int offset;
    bool isArray;
    struct list *next;
} list;

typedef enum
{
    GLOBALST,
    MODULEST,
    SCOPEST,
} SymTableType;

typedef struct SymTablePointer
{
    union
    {
        types typeIfNotArray;
        array_type typeIfArray;
    };
    bool isArray;
    list *input_para_list;
    list *output_para_list;
    hashtable *corrHashtable;
    struct SymTablePointer *parentHashTable;
    struct SymTablePointer *siblingHashTable;
    char *str;
    token *tk;
    int offset;
    int width;
    int line_start;
    int line_end;
    struct SymTablePointer *next;
    bool isAwaited;
    struct SymTablePointer *childScopeTable;
    SymTableType typeST;
    bool isDefined;
    bool isDeclared;
    bool called;
    int activationRecordSize;
} SymTablePointer;

#endif
