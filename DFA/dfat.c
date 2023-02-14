#include <stdio.h>
int main(){
    int s = 0; //state

    char c = 'a'; // full string here

    switch (s)
    {
        case 0:
            if ((c >='a' && c<='z') ||  (c>='A' && c<='Z')|| c == '_'){
                s = 1;
            }
            else if(c >='0' && c<='9'){
                s = 3;
            }
            else if(c == '\n'){
                s = 13;
            }
            else if(c == '\t' || c == ' '){
                s = 14;
            }
            else if(c == '+'){
                s = 15;
            }
            else if(c == '-'){
                s = 16;
            }
            else if(c == '*'){
                s = 17;
            }
            else if(c == '<'){
                s = 22;
            }
            else if(c == '>'){
                s =28;
            }
            else if(c == ','){
                s = 34;
            }
            else if(c == '='){
                s = 35;
            }
            else if(c == ';'){
                s = 37;
            }
            else if(c == '!'){
                s = 38;
            }
            else if(c == '/'){
                s = 40;
            }
            else if(c == '['){
                s = 41;
            }
            else if(c == ']'){
                s = 42;
            }
            else if(c == '.'){
                s = 43;
            }
            else if(c == ':'){
                s = 45;
            }
            else if(c == '('){
                s = 48;
            }
            else if(c == ')'){
                s = 49;
            }
            else{
                s= 50;
            }
        default:
            break;
    }
    printf("%d", s);
}