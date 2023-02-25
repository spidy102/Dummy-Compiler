#include <stdio.h>
int main()
{
    int x = 1;

    switch (x)
    {
    case 1:
        if (1)
        {
            x += 2;
            
        }
    case 2:
        printf("inside 2 rn\n");
        x++;
    case 3:
        if (1)
        {
            printf("inside 3 rn");
            x++;
            break;
        }

    case 4:
        printf("isnide 4 rn\n");
    }
}