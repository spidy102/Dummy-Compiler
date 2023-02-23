#include <stdio.h>
#include <ctype.h>
#include <string.h>

char dataReadFromFile[100];

int main (){

    char data[130][100];
    char nonTerminals[120][20];

    void createNodeList(int n); // function to create the list
    void displayList();         // function to display the list


    FILE *fp; // storing grammar contents from file to 2d array
    fp = fopen("Grammar.txt", "r");

    if (fp == NULL)
    {
        printf("Failure to open Grammar.txt");
    }
    else
    {
        // printf("Grammar.txt is opened.");
        int count = 0;   
        // printf("%d",count);    
        // while( fgets (dataReadFromFile, 90, fp) != NULL )
        // {
        //     strcpy(data[count],dataReadFromFile);
        //     count++;
        //     // Print the dataToBeRead
        //     // printf( "%s" , dataReadFromFile) ;
        // }

        fgets (dataReadFromFile, 90, fp);
        char * token = strtok(dataReadFromFile, " ");
         // loop through the string to extract all other tokens
        while( token != NULL ) {
            printf( " %s\n", token ); //printing each token
            
            token = strtok(NULL, " ");
        }

        struct node *forwardNode, *tempNode;
        startNode = (struct node *)malloc(sizeof(struct node));
        startNode->nodeToken = token;
        startNode ->nextPtr = NULL;
        int finalCountOfGrammar = count;
        // printing final count
        // printf("%d",finalCountOfGrammar);

        // Closing the file using fclose()
        fclose(fp) ;

        // for(int count = 0; count < finalCountOfGrammar;count++){
        //     for(int characterNumber = 0; data[count][characterNumber] != "→"; characterNumber++){

        //     }
        // }


    }
}

