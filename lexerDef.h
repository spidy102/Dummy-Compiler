int line_num = 1;

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

token *getNextToken(FILE *fp, hashtable ht, twinbuffer *tb);
token *make_token(int line_num, char *lexeme, token_names tok);
int getSize(twinbuffer *tb);
char *copyLexeme(twinbuffer *tb, int size);
void populate_hashtable(hashtable *ht);