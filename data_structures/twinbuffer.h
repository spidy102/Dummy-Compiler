#include <stdio.h>
#include <stdbool.h>

#define SIZE 10

typedef struct twinbuffer
{
    char *buffer;
    int begin;
    int fwd;
    FILE *fp;
    bool end;
} twinbuffer;

twinbuffer *twinbuffer_init(FILE *fp);

char readOneCharacter(twinbuffer *tb);

void retract(int num, twinbuffer *tb);