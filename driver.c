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
#include "lexer.h"
#include "lexerDef.h"
#include "token_name.h"
#include "tree.h"
#include "parser.h"
#include "parserDef.h"
#include "ast.h"
#include "astDef.h"
#include "symbolTable.h"
#include "symbolTableDef.h"
#include "symTableUtil.h"
#include "typeCheckerDef.h"
#include <time.h>

void displayImplementationStatus()
{
  printf("____________________________________________________________\n");
  printf("LEVEL 4: Symbol table, type checking and semantic rules module work.\n");
  printf("Handled static and dynamic arrays in type checking.\n"); 
  printf("____________________________________________________________\n");
}

void displayMenu()
{

  printf("____________________________________________________________\n");
  printf("0. Exit\n");
  printf("1. Display the token list.\n");
  printf("2. Display the parse tree.\n");
  printf("3. Display the abstract syntax tree.\n");
  printf("4. Display number of AST and parse tree nodes and their size.\n"); 
  printf("5. Display symbol table.\n");
  printf("6. Display activation record size.\n");
  printf("7. Display type expressions and width of array variables.\n");
  printf("8. Display Semantic Errors and compilation time.\n");
  printf("9. Generate assembly code.\n");
  printf("____________________________________________________________\n");

  printf("Enter your choice [0-9]: ");
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
      printf("Enter your choice [0-9]: ");
      while (getchar() != '\n')
        ;
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

void copyFile(FILE *fp1, FILE *fp2)
{
  char c;
  fseek(fp1, 0, SEEK_SET);
  fseek(fp2, 0, SEEK_SET);
  while ((c = fgetc(fp1)) != EOF)
  {
    fputc(c, fp2);
  }
}

int main(int argc, char *argv[])
{

  // expect 2 strings as arguments
  if (argc != 3)
  {
    printf("Invalid number of arguments. Expected 2 arguments. Exiting.\n");
    exit(1);
  }
  // ensure that the size of buffer is a valid integer
/*   for (int i = 0; i < strlen(argv[3]); i++)
  {
    if (argv[3][i] < '0' || argv[3][i] > '9')
    {
      printf("Invalid size of buffer. Expected an integer. Exiting.\n");
      exit(1);
    }
  }
 */
  // parse the arguments
  char *testcase = argv[1];
  char *asmfile = argv[2];
  int size_of_buffer = 256;        //2 * atoi(argv[3]);

  /*   if (size_of_buffer % 2 != 0)
    {
      printf("Invalid size of buffer. Expected an even integer. Exiting.\n");
      exit(1);
    }
   */
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
      printf("\nDisplaying the token list:\n");
      twinbuffer *twin_buf;
      FILE *fp = openfile(testcase, "r");
      twin_buf = twinbuffer_init(fp, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);
      line_num = 1;
      token *tk = getNextToken(ht, twin_buf);
      printf("%20s %20s %20s\n", "Lexeme", "Line number", "Token");
      while (tk != NULL)
      {
        if (tk->token == NUM)
        {
          printf("%20d %20d %20s\n", tk->integer, tk->line_num, EnumToString(tk->token));
          tk = getNextToken(ht, twin_buf);
          continue;
        }
        else if (tk->token == RNUM)
        {
          printf("%20f %20d %20s\n", tk->rnum, tk->line_num, EnumToString(tk->token));
          tk = getNextToken(ht, twin_buf);
          continue;
        }
        printf("%20s %20d %20s\n", tk->str, tk->line_num, EnumToString(tk->token));
        tk = getNextToken(ht, twin_buf);
      }
      fclose(fp);

    }
    else if (choice == 2)
    {
      FILE *ft = openfile(testcase, "r+");
      FILE *fg = openfile("Grammar.txt", "r");
      FILE *fpt = stdout; //openfile(parsetreeOutFile, "w");

      twinbuffer *twin_buf = twinbuffer_init(ft, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);
      fill_grammar(fg);
      populateParseTable();
      fseek(ft, 0, SEEK_SET);
      treenode *root = parseInputSourceCode(ft, twin_buf, ht);
      printf("\nDisplaying the parse tree:\n");
      printParseTree(root, fpt);
      freeGrammar();

      fclose(ft);
      fclose(fg);

    }
    else if (choice == 3)
    {
      FILE *ft = openfile(testcase, "r+");
      FILE *fg = openfile("Grammar.txt", "r");
      
      twinbuffer *twin_buf = twinbuffer_init(ft, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);
      fill_grammar(fg);
      populateParseTable();
      fseek(ft, 0, SEEK_SET);
      treenode *root = parseInputSourceCode(ft, twin_buf, ht);
      astNode *astRoot = constructAST(root);
      printf("\nDisplaying inorder traversal of AST:\n");
      display_inorder_ast(astRoot);
      printf("\nTraversal completed!\n");
      freeGrammar();
      fclose(ft);
      fclose(fg);

    }
    else if (choice == 4)
    {

    }
    else if (choice == 5)
    {

      globalSymbolTable = initSymTablePointer();
      globalSymbolTable->typeST = GLOBALST;
      globalSymbolTable->parentHashTable = NULL;
      hashtable ht1 = initHashtable();
      globalSymbolTable->corrHashtable = &ht1;

      FILE *ft = openfile(testcase, "r+");
      FILE *fg = openfile("Grammar.txt", "r");
      
      twinbuffer *twin_buf = twinbuffer_init(ft, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);
      fill_grammar(fg);
      populateParseTable();
      fseek(ft, 0, SEEK_SET);
      treenode *root = parseInputSourceCode(ft, twin_buf, ht);
      astNode *astRoot = constructAST(root);
      inorder_ast(astRoot);
      populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
      printf("\nDisplaying the global symbol table:\n");
      printSymbolTable(globalSymbolTable);

    }
    else if (choice == 6)
    {

    }
    else if (choice == 7)
    {

    }
    else if (choice == 8)
    {
      FILE *ft = openfile(testcase, "r+");
      FILE *fg = openfile("Grammar.txt", "r");
      //FILE *fpt = openfile(parsetreeOutFile, "w");

      twinbuffer *twin_buf = twinbuffer_init(ft, size_of_buffer);
      hashtable ht = initHashtable();
      populate_hashtable(&ht);

      start_time = clock();
      // invoke your lexer and parser here
      // computeFirstAndFollowSets();
      fill_grammar(fg);
      populateParseTable();
      fseek(ft, 0, SEEK_SET);
      treenode *root = parseInputSourceCode(ft, twin_buf, ht);

      end_time = clock();
      total_CPU_time = (double)(end_time - start_time);
      total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

      fclose(ft);
      fclose(fg);
      //fclose(fpt);
      printf("The number of CPU ticks taken to parse the source code: %lf\n", total_CPU_time);
      printf("Time taken to parse the source code: %lf seconds\n", total_CPU_time_in_seconds);
    }
    else if (choice == 9)
    {

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