#include <stdio.h>
#include <stdlib.h>
#include "twinbuffer.h"

twinbuffer *twinbuffer_init(FILE *fp)
{
    twinbuffer *tb = malloc(sizeof(twinbuffer *));
    tb->fp = fp;
    tb->begin = 0;
    tb->fwd = 0;
    tb->end = false;
    tb->buffer = malloc(sizeof(SIZE) * sizeof(char));
    return tb;
}

char readOneCharacter(twinbuffer *tb)
{
    FILE *fp = tb->fp;
    // if (!(tb->begin == 0 && tb->fwd == 0))
    // {
    //     tb->fwd++;
    // }
    if (tb->begin == 0 && tb->fwd == 0)
    {
        int x = fread(tb->buffer, 1, SIZE / 2, fp);
        if (x < SIZE / 2)
        {
            tb->buffer[x] = '\0';
        }
        // printf("%d", x);
        // printf("%d\n", read);
        // if (read < SIZE / 2)
        // {
        //     tb->buffer[read] = '\0';
        // }
        char temp = tb->buffer[tb->fwd];
        tb->fwd++;
        return temp;
    }
    else if (tb->fwd == SIZE / 2)
    {
        int read = fread(&tb->buffer[SIZE / 2], 1, SIZE / 2, fp);
        char temp = tb->buffer[tb->fwd];
        if (read < SIZE / 2)
        {
            tb->buffer[read + SIZE / 2] = '\0';
        }
        tb->fwd++;
        return temp;
    }
    else if (tb->fwd == SIZE)
    {
        int read = fread(tb->buffer, 1, SIZE / 2, fp);
        char temp = tb->buffer[0];
        if (read < SIZE / 2)
        {
            tb->buffer[read] = '\0';
        }
        tb->fwd = 1;
        return temp;
    }
    else
    {
        return tb->buffer[tb->fwd++];
    }
}

void retract(int num, twinbuffer *tb)
{
    int cur_fwd = tb->fwd;
    if (cur_fwd - num < 0)
    {
        tb->fwd -= SIZE - (cur_fwd - num);
    }
    else
    {
        tb->fwd -= num;
    }
}