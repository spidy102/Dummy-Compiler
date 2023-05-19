/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include "hashmap.h"
#include "twinbuffer.h"

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
FILE* removeComments(twinbuffer *tb, char *filename);
// void initLexer(FILE *fp, twinbuffer *tb, hashtable ht);

extern int line_num;

#endif