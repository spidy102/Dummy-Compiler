#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "./data_structures/hashmap.h"
#include "./data_structures/twinbuffer.h"
#include "lexerDef.h"

// typedef enum

// void twinbuffer()

token *make_token(int line_num, char *lexeme, token_names tok)
{
    token *tok1 = malloc(sizeof(token *));
    tok1->line_num = line_num;
    tok1->str = lexeme;
    tok1->token = tok;
    return tok1;
}

int getSize(twinbuffer *tb)
{
    int size = 0;
    if (tb->fwd - 1 > tb->begin)
    {
        // printf("size calculation, %d %d", tb->fwd, tb->begin);
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

// void find_token(char *id)
// {
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
// }

// token_names getNextToken(FILE *fp)

// void lexer_init(FILE *fp) {
//     twinbuffer *tb = twinbuffer_init(fp);
//     hashtable ht = initHashtable();

//     populate_hashtable(&ht);
// }

token *getNextToken(FILE *fp)
{
    twinbuffer *tb = twinbuffer_init(fp);
    hashtable ht = initHashtable();

    populate_hashtable(&ht);

    int s = 0; // state
    while (1)
    {
        char c = readOneCharacter(tb); // full string here

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
            break;
        case 1:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            {
                break;
            }
            else
            {
                s = 2;
                break;
            }
        case 2:
            retract(1, tb);
            int size1 = getSize(tb);
            char *lexeme1 = copyLexeme(tb, size1);
            if (exists(ht, lexeme1, size1))
            {
                token_names tokene = get(&ht, lexeme1, size1);
                // return token;//note: does this go to the string
                return make_token(line_num, lexeme1, tokene);
            }
            else
            {
                // printf("%s\n", lexeme);
                // return ID;//note: shouldnt we return the string of this
                return make_token(line_num, lexeme1, ID);
            }
        case 3:
            if (c >= '0' && c <= '9')
            {
                s = 3;
                break; // note should this break exist? yes it should
            }
            else if (c == '.')
            {
                s = 4;
                break; // note here also remove // no don't
            }
            else
            {
                s = 10;
                break;
            }
        case 10:
            retract(1, tb); // note here only retract one?
            int size2 = getSize(tb);
            char *lexeme2 = copyLexeme(tb, size2);
            int correspondingNumber = atoi(lexeme2);
            token *tk = malloc(sizeof(token *));
            tk->line_num = line_num;
            tk->integer = correspondingNumber;
            tk->token = NUM;
            return tk;
        case 4:
            if (c >= '0' && c <= '9')
            {
                s = 5;
                break; // note: break here?
            }
            else if (c == '.')
            {
                s = 11;
                break;
            }
            else
            {
                s = 50;
                break;
                // generate error here
            }
        case 11:
            retract(2, tb);
            int size3 = getSize(tb);
            char *lexeme3 = copyLexeme(tb, size3);
            int x = atoi(lexeme3);

            token *tk2 = malloc(sizeof(token *));
            tk2->line_num = line_num;
            tk2->integer = x;
            tk2->token = NUM;
            return tk2;
            // return NUM;//note: shouldnt we return the string of this
        case 5:
            if (c >= '0' && c <= '9')
            {
                s = 5;
                break; // note: here?
            }
            else if (c == 'e' || c == 'E')
            {
                s = 6;
                break;
            }
            else
            {
                s = 12;
            }

        case 12:
            retract(1, tb);
            // return RNUM;//note: shouldnt we return the string of this

            int size4 = getSize(tb);
            char *lexeme4 = copyLexeme(tb, size4);

            float rnum = atof(lexeme4);
            token *tk1 = malloc(sizeof(token *));
            tk1->line_num = line_num;
            tk1->rnum = rnum;
            tk1->token = RNUM;
            return tk1;

            break;

        case 6:
            if (c == '+' || c == '-')
            {
                s = 7;
                break;
            }
            else if (c >= '0' || c <= '9')
            {
                s = 8;
                break;
            }
            else
            {
                s = 50;
                break;
                // note: error
            }

        case 7:
            if (c >= '0' || c <= '9')
            {
                s = 8;
            }
            else
            {
                s = 50;
                // note: error
            }
            break;

        case 8:
            if (c >= '0' && c <= '9')
            {
                s = 8;
                break;
            }
            else
                s = 9;

        case 9: // might not be acutate due to e
            retract(1, tb);
            char *lexeme5 = copyLexeme(tb, getSize(tb));
            float x1 = atof(lexeme5);
            token *tk3 = malloc(sizeof(token *));
            tk3->line_num = line_num;
            tk3->rnum = x1; // maybe lexeme better?
            tk3->token = RNUM;
            return tk3;
            break;
        case 13:
            // note: need to add increase line number
            line_num++;
            // return ;//note: what to return here
            s = 0;
            break;
        case 14:
            // return ;//note: what to return here
            s = 0;
            break;
        case 15:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), PLUS);
            // return "PLUS";//note: this return correct
        case 16:
            return make_token(line_num, copyLexeme(tb, getSize(tb)), MINUS);
        case 17:
            // printf("%c", c);
            if (c == '*')
            {
                s = 18;
                break;
            }
            else
            {
                s = 21;
            }
            

        case 21:
            retract(1, tb);
            printf("multiplication here\n");
            return make_token(line_num, copyLexeme(tb, getSize(tb)), MUL);
        case 18:
        printf("hello %c\n", c);
        
            if (c == '*')
            {
                s = 19;
            }
            else
            {
                s = 18;
            }
            break;
        case 19:
            if (c == '*')
            {
                s = 20;
            }
            else
                s = 18;
            break;
        case 20:
            // return; //note: comment
            s = 0;
            break;
        case 22:
            if (c == '<')
            {
                s = 23;
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
                s = 30;
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
                s = 50;
                // note: error
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
                s = 50;
                // note: error
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
                s = 50;
                // note: error
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
            printf("invalid input\n");
            exit(0);
            break;

        default:
            printf("error: not a valid state (default)");
            break;
        }
    }
}

void removeComments(char *testcaseFile, char *cleanFile) {

    FILE *fp1 = fopen(testcaseFile,"r");
    FILE *fp2 = fopen(cleanFile,"w");

    char c = fgetc(fp1);
    int s = 0;

    while(c != EOF) {
        switch(s) {

            case 0:
                if(c == '*') {
                    s = 17;
                }
                else {
                    s = 0;
                    fputc(c,fp2);
                }
                break;
            
            case 17:
                if(c == '*') {
                    s = 18;
                }
                else {
                    s = 0;
                    fputc('*',fp2);
                    fputc(c,fp2);
                }
                break;
            
            case 18:
                if(c == '*') {
                    s = 19;
                }
                else {
                    fputc(' ',fp2);
                    fputc(' ',fp2);
                    fputc(c,fp2);
                    s = 18;
                }
                break;
            
            case 19:
                if(c == '*') {
                    fputc(' ',fp2);
                    fputc(' ',fp2);
                    s = 0;
                }
                else {
                    fputc('*',fp2);
                    fputc(c,fp2);
                    s = 18;
                }
                break;

        }
        c = fgetc(fp1);
    }

    fclose(fp1);
    fclose(fp2);

    printf("Removed comments");
}

int main()
{
    FILE *fp = fopen("example.erp", "r");
    printf("%d %d", getNextToken(fp)->token, LE);
}
