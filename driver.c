#include <stdio.h>
#include <stdlib.h>


int displayMenu(){
  printf("0. Exit\n");
  printf("1. Remove comments and display the comment free source code\n");
  printf("2. Display the token list\n");
  printf("3. Display the parse tree\n");
  printf("4. Display time taken to parse the source code\n");

  printf("Enter your choice [0-4]: ");

  //Take the input
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



int main(){

  while(1){

    int choice = displayMenu();
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
    } 
  }

  return 0;

}