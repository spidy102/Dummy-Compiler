#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define GRAMMAR_SIZE 120
#define MAX_TOKEN_SIZE 30
#define MAX_LINE_SIZE 100
char dataReadFromFile[100];

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

int main(){
    FILE* fp = openGrammar("Grammar.txt");
    int count = 0;
    // create an array of heads
    ruleNode* headArray[GRAMMAR_SIZE];
    while(fgets(dataReadFromFile, MAX_LINE_SIZE, fp) != NULL){
        ruleNode* head = createRuleList(dataReadFromFile);
        headArray[count] = head;
        count++;
    
    }
    printList(headArray[64]);
    return 0;
}



    
























