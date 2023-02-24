#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#define MAX_TOKEN_SIZE 30
#define MAX_LINE_SIZE 100
char dataReadFromFile[MAX_LINE_SIZE];

//open the grammar 
FILE* openGrammar(char* fileName){
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Failure to open Grammar.txt");
    }
    else
    {
        printf("Grammar.txt is opened.");
    }
    return fp;
}

// create a structure to store the entries of the grammar
typedef struct ruleNode {
    // stores the entry as a string and contains a pointer to the next node in a rule
    char grammarEntry[MAX_TOKEN_SIZE];
    struct ruleNode *nextPtr;

} ruleNode;

// function to add a node to the rule
void addNode(ruleNode* head, char* grammarEntry){
    ruleNode* newNode = (ruleNode*)malloc(sizeof(ruleNode));
    strcpy(newNode->grammarEntry, grammarEntry);
    newNode->nextPtr = NULL;
    ruleNode* temp = head;
    while(temp->nextPtr != NULL){
        temp = temp->nextPtr;
    }
    temp->nextPtr = newNode;
}

//function to create a linked list for each rule
ruleNode* createRuleList(char* dataReadFromFile){
    char * token = strtok(dataReadFromFile, " ");
    ruleNode* head = (ruleNode*)malloc(sizeof(ruleNode));
    strcpy(head->grammarEntry, token);
    head->nextPtr = NULL;
    token = strtok(NULL, " ");
    while( token != NULL ) {
        addNode(head, token);
        token = strtok(NULL, " ");
    }
    return head;
}

// function to print the linked list
void printList(ruleNode* head){
    ruleNode* temp = head;
    while(temp != NULL){
        printf("%s ", temp->grammarEntry);
        temp = temp->nextPtr;
    }
    printf("\n");
}


// function to check if the string is a non terminal
int isNonTerminal(char* token){
    if(token[0] == '<'){
        return 1;
    }
    return 0;
}

// obtain the non terminals from the grammar
void getNonTerminals(ruleNode* head, char* nonTerminals){
    ruleNode* temp = head;
    while(temp != NULL){
        if(isNonTerminal(temp->grammarEntry)){
            strcat(nonTerminals, temp->grammarEntry);
            strcat(nonTerminals, " ");
        }
        temp = temp->nextPtr;
    }
    //print the non terminals
    printf("%s\n", nonTerminals);
}


void getFirst(char* nonTerminal, ruleNode* headArray[], int grammarSize) {
    // TODO: CORRECT THIS!
    // printf("First(%s) = ", nonTerminal);
    for(int i = 0; i < grammarSize; i++){
        if(strcmp(nonTerminal, headArray[i]->grammarEntry) == 0){
            ruleNode* temp = headArray[i]->nextPtr;
            while(temp != NULL){
                if(isNonTerminal(temp->grammarEntry)){
                    getFirst(temp->grammarEntry, headArray, grammarSize);
                    if (strcmp(temp->grammarEntry,"ε") == 0){
                        getFirst(temp->nextPtr->grammarEntry, headArray, grammarSize);
                    }
                }
                else{
                    if (strcmp(temp->grammarEntry,"ε") != 0){
                        printf("%s ", temp->grammarEntry);
                    }
                    break;
                }
                temp = temp->nextPtr;
            }
        }

    }
}



int main(){
    FILE* fp = openGrammar("Grammar.txt");
    int grammarSize = 0;
    // find the size of the grammar
    while(fgets(dataReadFromFile, MAX_LINE_SIZE, fp) != NULL){
        grammarSize++;
    }
    // move the file pointer to the beginning of the file
    fseek(fp, 0, SEEK_SET);

    // create an array of heads
    ruleNode* headArray[grammarSize];
    int count = 0;
    while(fgets(dataReadFromFile, MAX_LINE_SIZE, fp) != NULL){
        ruleNode* head = createRuleList(dataReadFromFile);
        headArray[count] = head;
        count++;
    }
    
    char nonTerminals[grammarSize][MAX_TOKEN_SIZE];
    for(int i = 0; i < grammarSize; i++){
        strcpy(nonTerminals[i], headArray[i]->grammarEntry);
    }
    // remove duplicates from the nonTerminals array
    for(int i = 0; i < grammarSize; i++){
        for(int j = i + 1; j < grammarSize; j++){
            if(strcmp(nonTerminals[i], nonTerminals[j]) == 0){
                strcpy(nonTerminals[j], "");
            }
        }
    }
    getFirst("<program>", headArray, grammarSize);


   
    return 0;
}



    
























