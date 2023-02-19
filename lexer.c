#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "./data_structures/hashmap.h"
#include "./data_structures/twinbuffer.h"

// typedef enum

// void twinbuffer()

int getSize(twinbuffer *tb)
{
    int size = 0;
    if (tb->fwd - 1 > tb->begin)
    {
        printf("size calculation, %d %d", tb->fwd, tb->begin);
        size = tb->fwd - 1 - tb->begin + 1;
    }
    else
    {
        size = SIZE - tb->begin + tb->fwd;
    }
    return size;
}

char *copyLexeme(twinbuffer *tb, int size)
{

    char *lexeme = malloc(sizeof(char) * size);
    if (tb->fwd > tb->begin)
    {
        int i = 0;
        for (int j = tb->begin; j <= tb->fwd - 1; j++)
        {
            lexeme[i++] = tb->buffer[j];
        }
    }
    else
    {
        int j = 0;
        for (int i = tb->begin; i < SIZE; i++)
        {
            lexeme[j++] = tb->buffer[i];
        }

        for (int i = 0; i <= tb->fwd - 1; i++)
        {
            lexeme[j++] = tb->buffer[i];
        }
    }
    tb->begin = tb->fwd - 1;
    tb->fwd++;
    return lexeme;
}

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
}

token_names getNextToken(FILE *fp)
{
    twinbuffer *tb = twinbuffer_init(fp);
    hashtable ht = initHashtable();
    populate_hashtable(&ht);
    int s = 0; // state

    while (1)
    {
        char c = readOneCharacter(tb); // full string here
        switch (s)
        {
        case 0:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
            {
                s = 1;
            }
            else if (c >= '0' && c <= '9')
            {
                s = 3;
            }
            else if (c == '\n')
            {
                s = 13;
            }
            else if (c == '\t' || c == ' ')
            {
                s = 14;
            }
            else if (c == '+')
            {
                s = 15;
            }
            else if (c == '-')
            {
                s = 16;
            }
            else if (c == '*')
            {
                s = 17;
            }
            else if (c == '<')
            {
                s = 22;
            }
            else if (c == '>')
            {
                s = 28;
            }
            else if (c == ',')
            {
                s = 34;
            }
            else if (c == '=')
            {
                s = 35;
            }
            else if (c == ';')
            {
                s = 37;
            }
            else if (c == '!')
            {
                s = 38;
            }
            else if (c == '/')
            {
                s = 40;
            }
            else if (c == '[')
            {
                s = 41;
            }
            else if (c == ']')
            {
                s = 42;
            }
            else if (c == '.')
            {
                s = 43;
            }
            else if (c == ':')
            {
                s = 45;
            }
            else if (c == '(')
            {
                s = 48;
            }
            else if (c == ')')
            {
                s = 49;
            }
            else
            {
                s = 50;
            }
            break;
        case 1:
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == '_')
            {
                break;
            }
            else
            {

                s = 2;
            }
        case 2:
            retract(1, tb);
            int size = getSize(tb);
            char *lexeme = copyLexeme(tb, size);
            if (exists(ht, lexeme, size))
            {
                token_names token = get(&ht, lexeme, size);
                return token;
            }
            else
            {
                // printf("%s\n", lexeme);
                return ID;
            }
        case 3:
            if (c >= '0' && c <= '9')
            {
                break;
            }
            else if (c == '.')
            {
                s = 4;
                break;
            }
            else
            {
                s = 10;
            }
        case 10:
            retract(2, tb);
            int size4 = getSize(tb);
            char *lexeme4 = copyLexeme(tb, size4);
            int correspondingNumber = atoi(lexeme4);
            return NUM;
        case 4:
            if (c >= '0' && c <= '9')
            {
                s = 5;
                break;
            }
            else if (c == '.')
            {
                s = 11;
            }
            else
            {
                // generate error here
            }
        case 11:
            retract(2, tb);
            int size1 = getSize(tb);
            char *lexeme1 = copyLexeme(tb, size1);
            int x = atoi(lexeme1);
            return NUM;
        case 5:
            if (c >= '0' && c <= '9')
            {
                break;
            }
            else
            {
                s = 12;
            }
        case 12:
            retract(1, tb);
            char *lexeme3 = copyLexeme(tb, getSize(tb));
            float rnum = atof(lexeme3);
            return RNUM;
        default:
            break;
        }
    }
}

int main()
{
    FILE *fp = fopen("example.erp", "r");
    printf("%d %d", getNextToken(fp), NUM);
}