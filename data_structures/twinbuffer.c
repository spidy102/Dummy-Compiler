#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "twinbuffer.h"

twinbuffer *twinbuffer_init(FILE *fp, int buffer_size)
{
    twinbuffer *tb = malloc(sizeof(twinbuffer));
    tb->fp = fp;
    tb->begin = 0;
    tb->fwd = -1;
    tb->end = false;
    tb->whichReloaded = -1;
    tb->buffer_size = buffer_size;
    tb->buffer = malloc(tb->buffer_size * sizeof(char));

    memset(tb->buffer, '\0', tb->buffer_size);
    return tb;
}

char readOneCharacter(twinbuffer *tb)
{
    FILE *fp = tb->fp;

    // if (!(tb->begin == 0 && tb->fwd == 0))
    // {
    //     tb->fwd++;
    // }
    if (tb->begin == 0 && tb->fwd == -1)
    {
        tb->whichReloaded = 0;
        int x = fread(tb->buffer, 1, tb->buffer_size / 2, fp);
        if (x < tb->buffer_size / 2)
        {
            tb->buffer[x] = '\0';
        }

        for (int i = x; i < tb->buffer_size / 2; i++)
        {
            tb->buffer[i] = '\0';
        }
        // printf("%d", x);
        // printf("%d\n", read);
        // if (read < SIZE / 2)
        // {
        //     tb->buffer[read] = '\0';
        // }
        char temp = tb->buffer[++tb->fwd];
        return temp;
    }
    else if (tb->fwd == tb->buffer_size / 2 - 1 && tb->whichReloaded == 0)
    {
        tb->whichReloaded = 1;
        int read = fread(&tb->buffer[tb->buffer_size / 2], 1, tb->buffer_size / 2, fp);
        char temp = tb->buffer[++tb->fwd];
        if (read < tb->buffer_size / 2)
        {
            tb->buffer[read + tb->buffer_size / 2] = '\0';
        }
        for (int i = tb->buffer_size/ 2 + read; i < tb->buffer_size; i++)
        {
            tb->buffer[i] = '\0';
        }
        return temp;
    }
    else if (tb->fwd == tb->buffer_size - 1 && tb->whichReloaded == 1)
    {
        tb->whichReloaded = 0;
        int read = fread(&tb->buffer[0], 1, tb->buffer_size / 2, fp);
        char temp = tb->buffer[0];
        if (read < tb->buffer_size / 2)
        {
            tb->buffer[read] = '\0';
        }

        for (int i = read; i < tb->buffer_size/ 2; i++)
        {
            tb->buffer[i] = '\0';
        }
        tb->fwd = 0;
        return temp;
    }
    else
    {
        tb->fwd = (++tb->fwd) % tb->buffer_size;
        return tb->buffer[tb->fwd];
    }
}

void retract(int num, twinbuffer *tb)
{
    int cur_fwd = tb->fwd;
    if (cur_fwd - num < 0)
    {
        tb->fwd = tb->buffer_size + (cur_fwd - num);
    }
    else
    {
        tb->fwd -= num;
    }
}