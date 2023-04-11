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
    fprintf(fp, "section .data\n");
    fprintf(fp, "inpMessage db 'Enter an Integer value:',0\n");
    fprintf(fp, "outputMessage db 'Output:%%d',0\n");
    fprintf(fp, "intFormat db \"%%d\",0\n");
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
                fprintf(fp, "mov rax, %s\n", op1);
                fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 8);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 8);
            }

            break;
        }
        case OP_GETVALUE:
        {
            // handles integer
            fprintf(fp, "mov rdi, inpMessage\n");
            fprintf(fp, "mov rax, 0\n");
            fprintf(fp, "call printf WRT ..plt\n");
            fprintf(fp, "mov rdi, intFormat\n");
            fprintf(fp, "lea rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
            fprintf(fp, "mov rsi, rax\n");
            fprintf(fp, "call scanf WRT ..plt\n");
            break;
        }
        case OP_MUL:
        {
            char *op1 = globalHead->operand1;
            char *op2 = globalHead->operand2;

            if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "mov rax, %s\n", globalHead->operand1);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 != -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 8);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand1);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 8);
            }

            fprintf(fp, "imul rax, rbx\n");
            fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 8);
            break;
        }
        case OP_MINUS:
        {
            char *op1 = globalHead->operand1;
            char *op2 = globalHead->operand2;

            if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "mov rax, %s\n", globalHead->operand1);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 != -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 8);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand1);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 8);
            }

            fprintf(fp, "sub rax, rbx\n");
            fprintf(fp, "mov DWORD[rbp-8-%d], eax\n", globalHead->offsetRes * 8);
            break;
        }
        case OP_PLUS:
        {
            char *op1 = globalHead->operand1;
            char *op2 = globalHead->operand2;

            if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "mov rax, %s\n", globalHead->operand1);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 != -1 && globalHead->offsetOperand2 == -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 8);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand1);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 8);
            }

            fprintf(fp, "add rax, rbx\n");
            fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 8);
            break;
        }
        case OP_PRINT:
        {
            fprintf(fp, "mov rdi, outputMessage\n");
            fprintf(fp, "movsxd rsi, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 8);
            fprintf(fp, "mov rax,0\n");
            fprintf(fp, "call printf WRT ..plt\n");
            break;
        }
        case LABEL:
        {
            if (strcmp(globalHead->operand1, "driver") == 0)
            {
                fprintf(fp, "main:\n");
                fprintf(fp, "push rbp\n");
                fprintf(fp, "mov rbp, rsp\n");
                fprintf(fp, "sub rsp, %d\n", getFromSymTable(globalSymbolTable->corrHashtable, globalHead->operand1)->activationRecordSize * 8);
            }
            else if (strcmp(globalHead->operand1, "exit_driver") == 0)
            {
                fprintf(fp, "leave\n");
                fprintf(fp, "ret\n");
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
    // inorder_ast(astRoot);
    populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
    // if (semanticallyCorrect)
    typeCheck(astRoot);
    getActivationRecords();

    if (semanticallyCorrect && semanticRulesPassed)
    {
        startIntermCodeGen(astRoot);
    }
    // FILE *fp1 = fopen("temp.asm", "w");
    // quadruple *qp = globalHead;
    // genCode(fp1);
    // globalHead = qp;
    // while (globalHead != NULL)
    // {
    //     printf("%20s %20s %20s %20s\n", EnumToOperatorString(globalHead->op), globalHead->operand1, globalHead->operand2, globalHead->resultant);
    //     globalHead = globalHead->next;
    // }
}