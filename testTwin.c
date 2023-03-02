/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

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