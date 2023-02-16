#include <stdio.h>
#include <stdbool.h>
#include "./data_structures/hashmap.h"

// typedef enum

// void twinbuffer()

void populate_hashtable(hashtable *ht)
{
    insert(ht, "integer", 7, INTEGER);
    // printf("%d\n", get(ht, "integer", 7));
    insert(ht, "real", 4, REAL);
    insert(ht, "boolean", 7, BOOLEAN);
    insert(ht, "of", 2, OF);
    insert(ht, "array", 5, ARRAY);
    insert(ht, "start", 5, START);
    insert(ht, "end", 3, END);
    insert(ht, "declare", 7, DECLARE);
    insert(ht, "module", 6, MODULE);
    insert(ht, "driver", 6, DRIVER);
    insert(ht, "program", 7, PROGRAM);
    insert(ht, "get_value", 9, GET_VALUE);
    insert(ht, "print", 5, PRINT);
    insert(ht, "use", 3, USE);
    insert(ht, "with", 4, WITH);
    insert(ht, "parameters", 10, PARAMETERS);
    insert(ht, "takes", 5, TAKES);
    insert(ht, "input", 5, INPUT);
    insert(ht, "returns", 7, RETURNS);
    insert(ht, "for", 3, FOR);
    insert(ht, "in", 2, IN);
    insert(ht, "switch", 6, SWITCH);
    insert(ht, "case", 4, CASE);
    insert(ht, "break", 5, BREAK);
    insert(ht, "default", 7, DEFAULT);
    insert(ht, "while", 5, WHILE);
    insert(ht, "AND", 3, AND);
    insert(ht, "OR", 2, OR);
    insert(ht, "true", 4, TRUE);
    insert(ht, "false", 5, FALSE);
}

void find_token(char *id)
{
    hashtable ht = initHashtable();
    populate_hashtable(&ht);
    printf("%d", get(&ht, "switche", 6));
}

int main()
{
    find_token("switch");
}