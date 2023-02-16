#include <stdio.h>
#include "./data_structures/twinbuffer.h"

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
    printf("\n%c\n\n%c", tb->buffer[1], tb->buffer[5]);
}