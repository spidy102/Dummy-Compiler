#include <stdio.h>
#include "twinbuffer.h"

int main()
{
    FILE *fp = fopen("example.erp", "rb");
    twinbuffer *tb = twinbuffer_init(fp);
    char temp;
    int i = 0;
    while (1)
    {
        temp = readOneCharacter(tb);
        if (temp == '\0')
            break;
        printf("%c", temp);
    }
    for (int i = 0; i < 7; i++)
    {

        printf("%d\n", tb->buffer[i]);
    }
}