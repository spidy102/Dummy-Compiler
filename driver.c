#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void displayMenu(){
  printf("0. Exit\n");
  printf("1. Remove comments and display the comment free source code\n");
  printf("2. Display the token list\n");
  printf("3. Display the parse tree\n");
  printf("4. Display time taken to parse the source code\n");

  printf("Enter your choice [0-4]: ");

}

int takeInput(){
  int choice;
  while(1){
    if(scanf("%d", &choice) == 1) break;
    else{
      printf("Invalid choice. Please try again.\n");
      printf("Enter your choice [0-4]: ");
      while(getchar() != '\n');
    }
  }
  return choice;
}

int main(int argc, char *argv[]){

  //expect 2 strings and an integer as arguments
  if (argc != 4){
    printf("Invalid number of arguments. Expected 3 arguments. Exiting.\n");
    exit(1);
  }
  //ensure that the size of buffer is a valid integer
  for (int i = 0; i < strlen(argv[3]); i++){
    if (argv[3][i] < '0' || argv[3][i] > '9'){
      printf("Invalid size of buffer. Expected an integer. Exiting.\n");
      exit(1);
    }
  }

  //parse the arguments
  char *testcase = argv[1];
  char *parsetreeOutFile = argv[2];
  int size_of_buffer = atoi(argv[3]);

  displayMenu();
  int choice = takeInput();
  while(1){

    if (choice == 0) exit(0);
    else if (choice == 1){
      printf("You chose 1\n");
      break;
    }
    else if (choice == 2){
      printf("You chose 2\n");
      break;
    }
    else if (choice == 3){
      printf("You chose 3\n");
      break;
    }
    else if (choice == 4){
      printf("You chose 4\n");
      break;
    }
    else{
      printf("Invalid choice. Please try again.\n");
      printf("Enter your choice [0-4]: ");
      choice = takeInput();
    } 
  }

  return 0;

}