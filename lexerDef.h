#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include "./data_structures/hashmap.h"
#include "./data_structures/twinbuffer.h"

typedef struct TOKEN
{
    int line_num;
    union
    {
        float rnum;
        int integer;
        char *str;
    };
    token_names token;

} token;

token *getNextToken(hashtable ht, twinbuffer *tb);
token *make_token(int line_num, char *lexeme, token_names tok);
int getSize(twinbuffer *tb);
char *copyLexeme(twinbuffer *tb, int size);
void populate_hashtable(hashtable *ht);
void error(twinbuffer *tb, int line_num);
void populate_hashtable(hashtable *ht);
// void initLexer(FILE *fp, twinbuffer *tb, hashtable ht);

extern int line_num;

#endif