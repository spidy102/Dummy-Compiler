int line_num = 0;

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
    
}token;
