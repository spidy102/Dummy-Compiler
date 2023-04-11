#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "lexerDef.h"
#include "tree.h"
#include "parser.h"
#include "parserDef.h"
#include "ast.h"
#include "symbolTableDef.h"
#include "astDef.h"
#include "typeCheckerDef.h"
#include "symTableUtil.h"
#include "intermedCodeGen.h"
#include "intermCodeGenDef.h"
#include <string.h>

void genCode(FILE *fp)
{
    fprintf(fp, "global main\n");
    fprintf(fp, "extern printf\n");
    fprintf(fp, "extern scanf\n");
    fprintf(fp, "\n");
    fprintf(fp, "section .data");
    fprintf(fp, "inpMessage db 'Enter an Integer value:\n',0");
    fprintf(fp, "section .text\n");
    while (globalHead != NULL)
    {
        switch (globalHead->op)
        {
        case OP_ASSIGN:
        {
            char *op1 = globalHead->operand1;
            char *res = globalHead->resultant;

            if (globalHead->offsetOperand1 == -1)
            {
                fprintf(fp, "movsxd rax, %s\n", op1);
                fprintf(fp, "movsxd DWORD[rbp-8-%d], rax\n", globalHead->offsetRes);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1);
                fprintf(fp, "movsxd DWORD[rbp-8-%d], rax\n", globalHead->offsetRes);
            }

            break;
        }
        case OP_GETVALUE:
        {
            // handles integer
            fprintf(fp, "mov rdi, inpMessage\n");
            fprintf(fp, "mov rax, 0\n");
            fprintf(fp, "call printf\n");
            fprintf(fp, "mov rdi, intFormat\n");
            fprintf(fp, "lea rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1);
            fprintf(fp, "mov rsi, rax\n");
            fprintf(fp, "call scanf\n");
            break;
        }
        case OP_MUL:
        {
            char *op1 = globalHead->operand1;
            char *op2 = globalHead->operand2;

            if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "mov eax, %s\n", globalHead->operand1);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 != -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "movsxd eax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd eax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand1);
            }

            fprintf(fp, "imul eax, ebx\n");
            fprintf(fp, "movsxd DWORD[rbp-8-%d], eax\n", globalHead->offsetRes);
            break;
        }
        case OP_MINUS:
        {
            char *op1 = globalHead->operand1;
            char *op2 = globalHead->operand2;

            if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "mov eax, %s\n", globalHead->operand1);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 != -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "movsxd eax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd eax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand1);
            }

            fprintf(fp, "sub eax, ebx\n");
            fprintf(fp, "movsxd DWORD[rbp-8-%d], eax\n", globalHead->offsetRes);
            break;
        }
        case OP_PLUS:
        {
            char *op1 = globalHead->operand1;
            char *op2 = globalHead->operand2;

            if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "mov eax, %s\n", globalHead->operand1);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 != -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "movsxd eax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd eax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2);
                fprintf(fp, "mov ebx, %s\n", globalHead->operand1);
            }

            fprintf(fp, "add eax, ebx\n");
            fprintf(fp, "movsxd DWORD[rbp-8-%d], eax\n", globalHead->offsetRes);
            break;
        }
        case OP_PRINT:
        {
            fprintf(fp, "mov rdi, DWORD[rbp-8-%d]", globalHead->offsetRes);
            fprintf(fp, "mov rax,0");
            fprintf(fp, "call printf\n");
            break;
        }
        case LABEL:
        {
            if (strcmp(globalHead->operand1, "driver") == 0)
            {
                fprintf(fp, "main:\n");
            }
            break;
        }
        default:
            break;
        }
        globalHead = globalHead->next;
    }
}

int main()
{
    globalSymbolTable = initSymTablePointer();
    globalSymbolTable->typeST = GLOBALST;
    globalSymbolTable->parentHashTable = NULL;
    hashtable ht1 = initHashtable();
    globalSymbolTable->corrHashtable = &ht1;
    FILE *fp = fopen("random4.txt", "r");
    twinbuffer *tb = twinbuffer_init(fp, 256);
    fill_grammar(fopen("Grammar.txt", "r"));
    hashtable ht = initHashtable();
    populate_hashtable(&ht);
    populateParseTable();
    treenode *root = parseInputSourceCode(fp, tb, ht);
    astNode *astRoot = constructAST(root);
    inorder_ast(astRoot);
    populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
    // if (semanticallyCorrect)
    typeCheck(astRoot);
    if (semanticallyCorrect && semanticRulesPassed)
    {
        startIntermCodeGen(astRoot);
    }
    FILE *fp1 = fopen("temp.asm", "a");
    quadruple *qp = globalHead;
    genCode(fp1);
    globalHead = qp;
    while (globalHead != NULL)
    {
        printf("%20s %20s %20s %20s\n", EnumToOperatorString(globalHead->op), globalHead->operand1, globalHead->operand2, globalHead->resultant);
        globalHead = globalHead->next;
    }
}