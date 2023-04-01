/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef HASHMAP_H
#define HASHMAP_H

#include "token_name.h"
#include <stdbool.h>
#define HASHTABLE_SIZE 1000

typedef struct BUCKET_NODE
{
    void *str;
    token_names tok;
    struct BUCKET_NODE *next;
} bucket_node;

typedef struct HASHTABLE_NODE
{
    int idx;
    void *bucket_ptr;
} hashtable_node;

typedef struct HASHTABLE
{
    hashtable_node *table[HASHTABLE_SIZE];
} hashtable;

hashtable initHashtable();

int hash(void *str, int length);

bool insert(hashtable *ht, void *str, int length, token_names tok);

bool exists(hashtable *ht, void *str, int length);

bool delete(hashtable ht, void *str, int length);

token_names get(hashtable *ht, void *str, int length);

#endif