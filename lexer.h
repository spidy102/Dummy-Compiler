/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "hashmap.h"
#include "twinbuffer.h"
#include "lexerDef.h"

token *getNextToken(hashtable ht, twinbuffer *tb);
token *make_token(int line_num, char *lexeme, token_names tok);
int getSize(twinbuffer *tb);
char *copyLexeme(twinbuffer *tb, int size);
void populate_hashtable(hashtable *ht);
void error(twinbuffer *tb, int line_num);
void populate_hashtable(hashtable *ht);
FILE *removeComments(twinbuffer *tb, char *filename);

#endif
