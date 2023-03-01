#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "./data_structures/hashmap.h"
#include "./data_structures/twinbuffer.h"
#include "lexerDef.h"

// typedef enum

int line_num;

FILE *removeComments(twinbuffer *tb, char *filename)
{
    char c = readOneCharacter(tb);
    FILE *fp1 = fopen(filename, "w+");
    int s = 0;
    while (1)
    {
        if (c == 0)
        {
            break;
        }
        switch (s)
        {
        case 0:
            if (c == '*')
            {
                s = 17;
                c = readOneCharacter(tb);
            }
            else
            {
                fputc(c, fp1);
                c = readOneCharacter(tb);
            }
            break;
        case 17:
            if (c == '*')
            {
                s = 18;
                tb->begin = tb->fwd;
                c = readOneCharacter(tb);
            }
            else
            {
                s = 21;
            }
            break;
        case 21:
            fputc('*', fp1);
            s = 0;
            // c = readOneCharacter(tb);
            break;
        case 18:
            if (c == '*')
            {
                s = 19;
                c = readOneCharacter(tb);
            }
            else
            {
                if (c == '\n')
                    fputc('\n', fp1);
                c = readOneCharacter(tb);
            }
            break;
        case 19:
            if (c == '*')
            {
                s = 20;
            }
            else
            {
                s = 18;
                if (c == '\n')
                    fputc('\n', fp1);
                c = readOneCharacter(tb);
            }
            break;
        case 20:
            copyLexeme(tb, getSize(tb));
            s = 0;
            c = readOneCharacter(tb);
            break;
        }
    }
    return fp1;
}

// void twinbuffer()

token *make_token(int line_num, char *lexeme, token_names tok)
{
    token *tok1 = malloc(sizeof(token));
    tok1->line_num = line_num;
    tok1->str = lexeme;
    tok1->token = tok;
    return tok1;
}

int getSize(twinbuffer *tb)
{
    // this should just return tb->buffer_size right?

    int size = 0;
    if (tb->fwd >= tb->begin)
    {
        // printf("size calculation, %d %d", tb->fwd, tb->begin);
        size = tb->fwd + 1 - tb->begin;
    }
    else
    {
        size = tb->buffer_size - tb->begin + tb->fwd + 1;
    }
    return size;
}

char *copyLexeme(twinbuffer *tb, int size)
{
    char *lexeme = malloc(sizeof(char) * size);
    if (tb->fwd >= tb->begin)
    {
        int i = 0;
        for (int j = tb->begin; j <= tb->fwd; j++)
        {
            lexeme[i++] = tb->buffer[j];
        }
    }
    else
    {
        int j = 0;
        for (int i = tb->begin; i < tb->buffer_size; i++)
        {
            lexeme[j++] = tb->buffer[i];
        }

        for (int i = 0; i <= tb->fwd; i++)
        {
            lexeme[j++] = tb->buffer[i];
        }
    }
    // printf("fwd:%d begin %d\n", tb->fwd, tb->begin);
    tb->begin = tb->fwd + 1;
    if (tb->begin == tb->buffer_size)
        tb->begin = 0;
    // if (tb->fwd > 0 && tb->fwd < 512)
    //     printf("\nfwd:%d beg:%d\n", tb->fwd, tb->begin);
    return lexeme;
}

void error(twinbuffer *tb, int line_num)
{
    printf("Lexical error at line number: %d, lexeme read is %s\n", line_num, copyLexeme(tb, getSize(tb)));
    return;
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

// void find_token(char *id)
// {
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
// }

// token_names getNextToken(FILE *fp)
token *getNextToken(hashtable ht, twinbuffer *tb)
{

    char prev = ' ';
    int s = 0; // state
    char c = readOneCharacter(tb);
    while (1)
    {

        // full string here
        // printf("%d ascii read:%d %c\n", s, c, c);
        // if (prev == '\0' && c == prev)
        // {
        //     printf("nothing else to tokenize!");
        //     exit(0);
        // }
        if (c == 0 && tb->begin == tb->fwd)
        {
            printf("Nothing else to tokenize!");
            return NULL;
        }
        prev = c;
        switch (s) // note: should we add break after all cases? am adding here,
        // also what happens if none of the if cases get accepted, need to show error
        //  all cases in {}?
        // since we do return doesn't the program end, instead we can create a string buffer to keep adding the new tokens
        // to dilip: check semicolons after return, add breaks if you forget
        // change strings in returns back to normal if it is wrong
        // check if return syntax is right in all, like what is the second parameter?
        {
        case 0:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
            {
                s = 1;
                c = readOneCharacter(tb);
            }
            else if (c >= '0' && c <= '9')
            {
                s = 3;
                c = readOneCharacter(tb);
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
                c = readOneCharacter(tb);
            }
            else if (c == '<')
            {
                s = 22;
                c = readOneCharacter(tb);
            }
            else if (c == '>')
            {
                s = 28;
                c = readOneCharacter(tb);
            }
            else if (c == ',')
            {
                s = 34;
            }
            else if (c == '=')
            {
                s = 35;
                c = readOneCharacter(tb);
            }
            else if (c == ';')
            {
                s = 37;
            }
            else if (c == '!')
            {
                s = 38;
                c = readOneCharacter(tb);
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
                c = readOneCharacter(tb);
            }
            else if (c == ':')
            {
                s = 45;
                c = readOneCharacter(tb);
            }
            else if (c == '(')
            {
                s = 48;
            }
            else if (c == ')')
            {
                s = 49;
            }
            else if (c == '\r')
            {
                copyLexeme(tb, getSize(tb));
                c = readOneCharacter(tb);
            }
            else
            {
                s = 50;
            }
            break;
        case 1:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            {
                c = readOneCharacter(tb);
            }
            else
            {
                s = 2;
            }
            break;
        case 2:
            retract(1, tb);
            int size1 = getSize(tb);
            if (size1 > 20)
            {

                char *temp = copyLexeme(tb, size1);
                printf("Lexical error at line %d, lexeme %s is of length greater than 20\n", line_num, temp);
                s = 0;
                c = readOneCharacter(tb);
                break;
            }
            char *lexeme1 = copyLexeme(tb, size1);
            // printf("lexeme1:%s %d %c\n", lexeme1, size1, c);
            if (exists(&ht, lexeme1, size1))
            {
                token_names tokene = get(&ht, lexeme1, size1);
                // return token;//note: does this go to the string
                return make_token(line_num, lexeme1, tokene);
            }
            else
            {
                // return ID;//note: shouldnt we return the string of this
                return make_token(line_num, lexeme1, ID);
            }
        case 3:
            if (c >= '0' && c <= '9')
            {
                s = 3;
                c = readOneCharacter(tb);
            }
            else if (c == '.')
            {
                s = 4;
                c = readOneCharacter(tb);
            }
            else
            {
                s = 10;
            }
            break;
        case 10:
            retract(1, tb); // note here only retract one?
            int size2 = getSize(tb);
            char *lexeme2 = copyLexeme(tb, size2);
            int correspondingNumber = atoi(lexeme2);
            token *tk = malloc(sizeof(token));
            tk->line_num = line_num;
            tk->integer = correspondingNumber;
            tk->token = NUM;
            return tk;
        case 4:
            if (c >= '0' && c <= '9')
            {
                s = 5;
                c = readOneCharacter(tb);
            }
            else if (c == '.')
            {
                s = 11;
            }
            else
            {
                retract(1, tb);

                error(tb, line_num);
                s = 0;
                c = readOneCharacter(tb);
                // generate error here
            }
            break;
        case 11:
            retract(2, tb);
            int size3 = getSize(tb);
            char *lexeme3 = copyLexeme(tb, size3);
            int x = atoi(lexeme3);

            token *tk2 = malloc(sizeof(token));
            tk2->line_num = line_num;
            tk2->integer = x;
            tk2->token = NUM;
            return tk2;
            // return NUM;//note: shouldnt we return the string of this
        case 5:
            if (c >= '0' && c <= '9')
            {
                s = 5;
                c = readOneCharacter(tb);
            }
            else if (c == 'e' || c == 'E')
            {
                s = 6;
                c = readOneCharacter(tb);
            }
            else
            {
                s = 12;
            }
            break;
        case 12:
            retract(1, tb);
            // return RNUM;//note: shouldnt we return the string of this

            int size4 = getSize(tb);
            char *lexeme4 = copyLexeme(tb, size4);

            float rnum = atof(lexeme4);
            token *tk1 = malloc(sizeof(token));
            tk1->line_num = line_num;
            tk1->rnum = rnum;
            tk1->token = RNUM;
            return tk1;
        case 6:

            if (c == '+' || c == '-')
            {

                s = 7;
                c = readOneCharacter(tb);
            }
            else if (c >= '0' && c <= '9')
            {

                s = 8;
                c = readOneCharacter(tb);
            }
            else
            {
                // note: error
                retract(1, tb);
                error(tb, line_num);
                s = 0;
                c = readOneCharacter(tb);
            }
            break;
        case 7:
            if (c >= '0' && c <= '9')
            {
                s = 8;
                c = readOneCharacter(tb);
            }
            else
            {
                // note: error
                retract(1, tb);
                error(tb, line_num);
                s = 0;
                c = readOneCharacter(tb);
            }
            break;

        case 8:
            if (c >= '0' && c <= '9')
            {
                s = 8;
                c = readOneCharacter(tb);
            }
            else
                s = 9;
            break;

        case 9: // might not be acutate due to e
            retract(1, tb);
            char *lexeme5 = copyLexeme(tb, getSize(tb));
            float x1 = atof(lexeme5);
            token *tk3 = malloc(sizeof(token));
            tk3->line_num = line_num;
            tk3->rnum = x1; // maybe lexeme better?
            tk3->token = RNUM;
            return tk3;
            break;
        case 13:
            line_num++;
            s = 0;
            copyLexeme(tb, getSize(tb));
            c = readOneCharacter(tb);
            break;
        case 14:
            // return ;//note: what to return here
            s = 0;
            copyLexeme(tb, getSize(tb));
            c = readOneCharacter(tb);
            break;
        case 15:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), PLUS);
            // return "PLUS";//note: this return correct
        case 16:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), MINUS);
        case 17:
            if (c == '*')
            {
                s = 18;
                c = readOneCharacter(tb);
            }
            else
            {
                s = 21;
            }
            break;
        case 21:
            retract(1, tb);
            return make_token(line_num, copyLexeme(tb, getSize(tb)), MUL);
        case 18:
            if (c == '*')
            {
                s = 19;
                c = readOneCharacter(tb);
            }
            else if (c == '\n')
            {
                line_num++;
                c = readOneCharacter(tb);
            }
            else
            {
                s = 18;
                c = readOneCharacter(tb);
            }
            break;
        case 19:
            if (c == '*')
            {
                s = 20;
            }
            else
            {
                if (c == '\n')
                    line_num++;
                s = 18;
                c = readOneCharacter(tb);
            }
            break;
        case 20:
            // return; //note: comment
            s = 0;
            copyLexeme(tb, getSize(tb));
            c = readOneCharacter(tb);
            break;
        case 22:
            if (c == '<')
            {
                s = 23;
                c = readOneCharacter(tb);
            }
            else if (c == '=')
            {
                s = 25;
            }
            else
            {
                s = 26;
            }
            break;
        case 23:
            if (c == '<')
            {
                s = 24;
            }
            else
            {
                s = 27;
            }
            break;
        case 24:

            return make_token(line_num, copyLexeme(tb, getSize(tb)), DRIVERDEF);
        case 25:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), LE);
        case 26:
            retract(1, tb);
            return make_token(line_num, copyLexeme(tb, getSize(tb)), LT);
        case 27:
            retract(1, tb);
            return make_token(line_num, copyLexeme(tb, getSize(tb)), DEF);
        case 28:
            if (c == '>')
            {
                s = 29;
                c = readOneCharacter(tb);
            }
            else if (c == '=')
            {
                s = 32;
            }
            else
                s = 33;
            break;
        case 29:
            if (c == '>')
            {
                s = 30;
            }
            else
                s = 31;
            break;
        case 30:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), DRIVERENDDEF);
        case 31:
            retract(1, tb);
            return make_token(line_num, copyLexeme(tb, getSize(tb)), ENDDEF);
        case 32:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), GE);
        case 33:
            retract(1, tb);
            return make_token(line_num, copyLexeme(tb, getSize(tb)), GT);
        case 34:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), COMMA);
        case 35:
            if (c == '=')
                s = 36;
            else
            {
                // note: error
                retract(1, tb);
                error(tb, line_num);
                s = 0;
                c = readOneCharacter(tb);
            }
            break;
        case 36:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), EQ);
        case 37:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), SEMICOL);
        case 38:
            if (c == '=')
                s = 39;
            else
            {
                // note: error
                retract(1, tb);
                error(tb, line_num);
                s = 0;
                c = readOneCharacter(tb);
            }
            break;
        case 39:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), NE);
        case 40:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), DIV);
        case 41:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), SQBO);
        case 42:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), SQBC);
        case 43:
            if (c == '.')
                s = 44;
            else
            {
                // note: error
                retract(1, tb);
                error(tb, line_num);
                s = 0;
                c = readOneCharacter(tb);
            }
            break;
        case 44:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), RANGEOP);
        case 45:
            if (c == '=')
                s = 46;
            else
            {
                s = 47;
            }
            break;
        case 46:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), ASSIGNOP);
        case 47:
            retract(1, tb);
            return make_token(line_num, copyLexeme(tb, getSize(tb)), COLON);
        case 48:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), BO);
        case 49:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), BC);

        case 50:
            error(tb, line_num);
            s = 0;
            c = readOneCharacter(tb);
            break;
        }
    }
}

/* int main()
{
    FILE *fp = fopen("example.erp", "r");
    if (fp == NULL)
    {
        printf("File cannot be openened");
    }
    twinbuffer *tb;
    hashtable ht;
    line_num = 1;
    tb = twinbuffer_init(fp);
    ht = initHashtable();
    populate_hashtable(&ht);
    // printf("%c", readOneCharacter(tb));
    while (1)
    {
        token *tk = getNextToken(ht, tb);
        if (tk == NULL)
            break;
        if (tk->token == NUM)
        {
            printf("%d\n", tk->integer);
        }
        else if (tk->token == RNUM)
        {
            printf("%lf\n", tk->rnum);
        }
        else
        {
            printf("%s\n", tk->str);
        }
    }

    // removeComments(tb);

    // printf("%s", tb->buffer);

    // populate_hashtable(&ht);
    // bucket_node *ptr1 = ht.table[536]->bucket_ptr;

    // while (ptr1 != NULL)
    // {
    //     printf("\n%s\n", ptr1->str);
    //     ptr1 = ptr1->next;
    // }

    // exists(ht, "module", 6);
    // exists(ht, "mod1", 4);
    // exists(ht, "takes", 5);
    // exists(ht, "index", 5);

    // ptr1 = ht.table[536]->bucket_ptr;

    // while (ptr1 != NULL)
    // {
    //     printf("\n%s\n", ptr1->str);
    //     ptr1 = ptr1->next;
    // }
}
 */