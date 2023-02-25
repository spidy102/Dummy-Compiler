#include <stdio.h>
#include <string.h>

int main() {
    FILE* fp = fopen("Grammar.txt","r");
    char buffer[100];
    fgets(buffer, 100, fp);
    char* tk = strtok(buffer, " \n");
    printf("%s\n", tk);
    tk = strtok(NULL, " \n");
    printf("%s\n", tk);
    tk = strtok(NULL, " \n");
    printf("%s%ld\n", tk,strlen(tk));
    tk = strtok(NULL, " \n");
    printf("%s%ld\n", tk,strlen(tk));
    tk = strtok(NULL, " \n");
    printf("%s\n", tk);
    if (tk==NULL) {
        printf("hemlo?");
    }
}