/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "token_name.h"
#include "tree.h"
#include "parser.h"
#include "parserDef.h"
#include <time.h>

void displayImplementationStatus()
{
  printf("____________________________________________________________\n");
  printf("Implementation Status:\n\n");
  printf("a) FIRST and FOLLOW Sets automated.\n");
  printf("b) Both lexical and syntax analyser modules are implemented.\n");
  printf("c) Modules work with the test cases provided.\n");
  printf("d) Parse Tree is constructed successfully.\n");
  printf("____________________________________________________________\n");
}

void displayMenu()
{

  printf("____________________________________________________________\n");
  printf("0. Exit\n");
  printf("1. Remove comments and display the comment free source code.\n");
  printf("2. Display the token list.\n");
  printf("3. Display the parse tree.\n");
  printf("4. Display time taken to parse the source code.\n");
  printf("____________________________________________________________\n");

  printf("Enter your choice [0-4]: ");
}

void displayFile(FILE *fp)
{
  printf("Displaying the file...\n");
  char c;
  fseek(fp, 0, SEEK_SET);
  while ((c = fgetc(fp)) != EOF)
  {
    printf("%c", c);
  }
  printf("\n");
}

int takeInput()
{
  int choice;
  while (1)
  {
    if (scanf("%d", &choice) == 1)
      break;
    else
    {
      printf("Invalid choice. Please try again.\n");
      printf("Enter your choice [0-4]: ");
      while (getchar() != '\n');
    }
  }
  return choice;
}

FILE *openfile(char *filename, char *mode)
{
  FILE *fp = fopen(filename, mode);
  if (fp == NULL)
  {
    printf("Error opening file. Exiting.\n");
    exit(1);
  }
  return fp;
}

char *EnumToString(token_names nt)
{
  int i = 0;
  FILE *fp = fopen("tokens.txt", "r");
  char *buffer = malloc(sizeof(char) * 100);
  while (fgets(buffer, 100, fp) != NULL)
  {
    buffer[strlen(buffer) - 3] = '\0';
    // printf("%s\n", buffer);

    if (nt == i)
      return buffer;
    i++;
  }
}

void copyFile(FILE* fp1, FILE* fp2)
{
  char c;
  fseek(fp1, 0, SEEK_SET);
  fseek(fp2, 0, SEEK_SET);
  while ((c = fgetc(fp1)) != EOF)
  {
    fputc(c, fp2);
  }
}

int main(int argc, char *argv[]){

  // expect 2 strings and an integer as arguments
  if (argc != 4)
  {
    printf("Invalid number of arguments. Expected 3 arguments. Exiting.\n");
    exit(1);
  }
  // ensure that the size of buffer is a valid integer
  for (int i = 0; i < strlen(argv[3]); i++)
  {
    if (argv[3][i] < '0' || argv[3][i] > '9')
    {
      printf("Invalid size of buffer. Expected an integer. Exiting.\n");
      exit(1);
    }
  }

  // parse the arguments
  char *testcase = argv[1];
  char *parsetreeOutFile = argv[2];
  int size_of_buffer = atoi(argv[3]);

  if (size_of_buffer % 2 != 0)
  {
    printf("Invalid size of buffer. Expected an even integer. Exiting.\n");
    exit(1);
  }

  displayImplementationStatus();
  displayMenu();
  int choice = takeInput();

  clock_t start_time, end_time;
  double total_CPU_time, total_CPU_time_in_seconds;

  while (1)
  {

    if (choice == 0)
      exit(0);
    else if (choice == 1)
    {
      twinbuffer *twin_buf;
      FILE* fp = openfile(testcase, "r");

      //copy the file to a new file
      FILE* fp2 = openfile("copy", "w+");
      copyFile(fp, fp2);
      fclose(fp2);

      fseek(fp, 0, SEEK_SET);
      twin_buf = twinbuffer_init(fp, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);
      FILE* fp1 = removeComments(twin_buf, "copy");
      displayFile(fp1);
      fclose(fp1);
      fclose(fp);
    }
    else if (choice == 2)
    {
      twinbuffer *twin_buf;
      FILE* fp = openfile(testcase, "r");
      twin_buf = twinbuffer_init(fp, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);
      line_num = 1;
      token *tk = getNextToken(ht, twin_buf);
      while (tk != NULL)
      {
        if (tk->token == NUM)
        {
          printf("%d %d %s\n", tk->integer, tk->line_num, EnumToString(tk->token));
          tk = getNextToken(ht, twin_buf);
          continue;
        }
        else if (tk->token == RNUM)
        {
          printf("%f %d %s\n", tk->rnum, tk->line_num, EnumToString(tk->token));
          tk = getNextToken(ht, twin_buf);
          continue;
        }
        printf("%s %d %s\n", tk->str, tk->line_num, EnumToString(tk->token));
        tk = getNextToken(ht, twin_buf);
      }
      fclose(fp);

    }
    else if (choice == 3)
    {

      FILE* ft = openfile(testcase, "r+");
      FILE* fg = openfile("Grammar.txt", "r");
      FILE* fpt = openfile(parsetreeOutFile, "w");

      twinbuffer* twin_buf = twinbuffer_init(ft, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);

      // start_time = clock();
      // invoke your lexer and parser here
      // computeFirstAndFollowSets();
      fill_grammar(fg);
      populateParseTable();
      fseek(ft, 0, SEEK_SET);
      treenode *root = parseInputSourceCode(ft, twin_buf, ht);
      printParseTree(root, fpt);
      freeGrammar();

      fclose(ft);
      fclose(fg);
      fclose(fpt);
    }
    else if (choice == 4)
    {
      FILE* ft = openfile(testcase, "r+");
      FILE* fg = openfile("Grammar.txt", "r");
      FILE* fpt = openfile(parsetreeOutFile, "w");

      twinbuffer* twin_buf = twinbuffer_init(ft, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);

      start_time = clock();
      // invoke your lexer and parser here
      // computeFirstAndFollowSets();
      fill_grammar(fg);
      populateParseTable();
      fseek(ft, 0, SEEK_SET);
      treenode *root = parseInputSourceCode(ft, twin_buf, ht);
      printParseTree(root, fpt);

      end_time = clock();
      total_CPU_time = (double)(end_time - start_time);
      total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

      fclose(ft);
      fclose(fg);
      fclose(fpt);
      printf("Time taken to parse the source code: %lf seconds\n", total_CPU_time_in_seconds);
    }
    else
    {
      printf("Invalid choice. Please try again.\n");
      printf("Enter your choice [0-4]: ");
      choice = takeInput();
    }

    displayMenu();
    choice = takeInput();

  }

  return 0;
}