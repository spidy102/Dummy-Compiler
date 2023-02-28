#ifndef TWINBUFFER_H
#define TWINBUFFER_H

#include <stdio.h>
#include <stdbool.h>

#define SIZE 512

typedef struct twinbuffer
{
    char *buffer;
    int begin;
    int fwd;
    FILE *fp;
    bool end;
    int whichReloaded;
    int buffer_size;
} twinbuffer;

twinbuffer *twinbuffer_init(FILE *fp, int buffer_size);

char readOneCharacter(twinbuffer *tb);

void retract(int num, twinbuffer *tb);

#endif