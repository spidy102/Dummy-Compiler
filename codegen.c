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
#include "codeGenDef.h"
#include <string.h>

void genCode(FILE *fp)
{
    fprintf(fp, "global main\n");
    fprintf(fp, "extern printf\n");
    fprintf(fp, "extern scanf\n");
    fprintf(fp, "\n");
    fprintf(fp, "section .data\n");
    fprintf(fp, "inpMessage db 'Enter an Integer value:',0\n");
    fprintf(fp, "outputMessage db `Output:%%d\\n`,0\n");
    fprintf(fp, "outputMessageArray db `Output:`,0\n");
    fprintf(fp, "newLine db `\\n`,0\n");
    fprintf(fp, "true equ 1\n");
    fprintf(fp, "false equ 0\n");
    fprintf(fp, "intFormat db \"%%d\",0\n");
    fprintf(fp, "arrayPrintFormat db \"%%d \",0\n");
    fprintf(fp, "arrMessage db `Input: Enter %%d array elements for range %%d to %%d\\n`,0\n");
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
                fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 16);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 16);
            }

            break;
        }
        case OP_GETVALUE:
        {
            if (globalHead->op1Ptr->isArray)
            {
                fprintf(fp, "mov rdi, arrMessage\n");
                fprintf(fp, "mov rsi, %d\n", (globalHead->op1Ptr->typeIfArray.high - globalHead->op1Ptr->typeIfArray.low + 1));
                fprintf(fp, "mov rdx, %d\n", globalHead->op1Ptr->typeIfArray.low);
                fprintf(fp, "mov rcx, %d\n", globalHead->op1Ptr->typeIfArray.high);
                fprintf(fp, "mov rax, 0\n");
                fprintf(fp, "call printf WRT ..plt\n");

                int startOffset = globalHead->op1Ptr->offset + 1;
                int numElements = (globalHead->op1Ptr->typeIfArray.high - globalHead->op1Ptr->typeIfArray.low + 1);
                if (globalHead->op1Ptr->typeIfArray.type == TYPE_INTEGER)
                {
                    for (int i = 0; i < numElements; i++)
                    {
                        fprintf(fp, "mov rdi, intFormat\n");
                        fprintf(fp, "lea rax, DWORD[rbp-8-%d]\n", startOffset * 16);
                        fprintf(fp, "mov rsi, rax\n");
                        fprintf(fp, "call scanf WRT ..plt\n");
                        startOffset += 2;
                    }
                }
            }
            else
            {
                // handles integer
                fprintf(fp, "mov rdi, inpMessage\n");
                fprintf(fp, "mov rax, 0\n");
                fprintf(fp, "call printf WRT ..plt\n");
                fprintf(fp, "mov rdi, intFormat\n");
                fprintf(fp, "lea rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "mov rsi, rax\n");
                fprintf(fp, "call scanf WRT ..plt\n");
            }
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
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand1);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
            }

            fprintf(fp, "imul rax, rbx\n");
            fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 16);
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
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand1);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
            }

            fprintf(fp, "sub rax, rbx\n");
            fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 16);
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
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (globalHead->offsetOperand1 == -1 && globalHead->offsetOperand2 != -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand1);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
            }

            fprintf(fp, "add rax, rbx\n");
            fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 16);
            break;
        }
        case OP_PRINT:
        {
            if (globalHead->op1Ptr->isArray)
            {
                fprintf(fp, "mov rdi, outputMessageArray\n");
                fprintf(fp, "mov rax,0\n");
                fprintf(fp, "call printf WRT ..plt\n");

                int startOffset = globalHead->op1Ptr->offset + 1;
                int numElements = (globalHead->op1Ptr->typeIfArray.high - globalHead->op1Ptr->typeIfArray.low + 1);
                if (globalHead->op1Ptr->typeIfArray.type == TYPE_INTEGER)
                {
                    for (int i = 0; i < numElements; i++)
                    {
                        fprintf(fp, "mov rdi, arrayPrintFormat\n");
                        fprintf(fp, "movsxd rsi, DWORD[rbp-8-%d]\n", startOffset * 16);
                        fprintf(fp, "call printf WRT ..plt\n");
                        startOffset += 2;
                    }
                    fprintf(fp, "mov rdi, newLine\n");
                    fprintf(fp, "call printf WRT ..plt\n");
                }
            }
            else
            {
                fprintf(fp, "mov rdi, outputMessage\n");
                fprintf(fp, "movsxd rsi, DWORD[rbp-8-%d]\n", globalHead->offsetOperand1 * 16);
                fprintf(fp, "mov rax,0\n");
                fprintf(fp, "call printf WRT ..plt\n");
            }
            break;
        }
        case LABEL:
        {
            if (strcmp(globalHead->operand1, "driver") == 0)
            {
                fprintf(fp, "main:\n");
                fprintf(fp, "push rbp\n");
                fprintf(fp, "mov rbp, rsp\n");
                fprintf(fp, "sub rsp, %d\n", getFromSymTable(globalSymbolTable->corrHashtable, globalHead->operand1)->activationRecordSize * 16);
            }
            else if (strcmp(globalHead->operand1, "exit_driver") == 0)
            {
                fprintf(fp, "leave\n");
                fprintf(fp, "ret\n");
            }
            else
            {
                fprintf(fp, "%s:\n", globalHead->operand1);
            }
            break;
        }
        case JUMP:
        {
            fprintf(fp, "jmp %s\n", globalHead->operand1);
            break;
        }
        case OP_GT:
        case OP_LE:
        case OP_LT:
        case OP_GE:
        case OP_NE:
        case OP_EQ:
        {
            int offset1 = globalHead->offsetOperand1;
            int offset2 = globalHead->offsetOperand2;
            if (offset1 == -1 && offset2 == -1)
            {
                fprintf(fp, "mov rax, %s\n", globalHead->operand1);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (offset2 == -1)
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d];hello\n", 16 * globalHead->offsetOperand1);
                fprintf(fp, "mov rbx, %s\n", globalHead->operand2);
            }
            else if (offset1 == -1)
            {
                fprintf(fp, "mov rax, %s\n", globalHead->operand1);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d];hello\n", 16 * globalHead->offsetOperand2);
            }
            else
            {
                fprintf(fp, "movsxd rax, DWORD[rbp-8-%d];hello\n", 16 * globalHead->offsetOperand1);
                fprintf(fp, "movsxd rbx, DWORD[rbp-8-%d]\n", 16 * globalHead->offsetOperand2);
            }
            fprintf(fp, "cmp rax, rbx\n");
            operators op = globalHead->op;
            globalHead = globalHead->next;
            if (op == OP_GT)
            {
                fprintf(fp, "jg %s\n", globalHead->operand2);
            }
            else if (op == OP_LT)
            {
                fprintf(fp, "jl %s\n", globalHead->operand2);
            }
            else if (op == OP_LE)
            {
                fprintf(fp, "jle %s\n", globalHead->operand2);
            }
            else if (op == OP_GE)
            {
                fprintf(fp, "jge %s\n", globalHead->operand2);
            }
            else if (op == OP_EQ)
            {
                fprintf(fp, "je %s\n", globalHead->operand2);
            }
            else if (op == OP_NE)
            {
                fprintf(fp, "jne %s\n", globalHead->operand2);
            }
            break;
        }
        case LW:
        {
            fprintf(fp, "movsxd rax, DWORD[rbp-8-%d]\n", globalHead->offsetOperand2 * 16);
            fprintf(fp, "mov rbx, rbp\n");
            fprintf(fp, "imul rax, 16\n");
            fprintf(fp, "sub rbx, rax\n");
            fprintf(fp, "sub rbx,8\n");
            fprintf(fp, "movsxd rax, DWORD[rbx]\n");
            fprintf(fp, "mov QWORD[rbp-8-%d], rax\n", globalHead->offsetRes * 16);
            break;
        }
        default:
            break;
        }
        globalHead = globalHead->next;
    }
}

// int main()
// {
//     globalSymbolTable = initSymTablePointer();
//     globalSymbolTable->typeST = GLOBALST;
//     globalSymbolTable->parentHashTable = NULL;
//     hashtable ht1 = initHashtable();
//     globalSymbolTable->corrHashtable = &ht1;
//     FILE *fp = fopen("random4.txt", "r");
//     twinbuffer *tb = twinbuffer_init(fp, 256);
//     fill_grammar(fopen("Grammar.txt", "r"));
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
//     populateParseTable();
//     treenode *root = parseInputSourceCode(fp, tb, ht);
//     astNode *astRoot = constructAST(root);
//     // inorder_ast(astRoot);
//     FILE *fp1 = fopen("temp.asm", "w");
//     populateGlobalSymbolTable(globalSymbolTable, astRoot, 0, true);
//     // if (semanticallyCorrect)
//     typeCheck(astRoot, true);
//     getActivationRecords();

//     if (semanticallyCorrect && semanticRulesPassed)
//     {
//         startIntermCodeGen(astRoot);
//     }
//     // FILE *fp1 = fopen("temp.asm", "w");
//     quadruple *qp = globalHead;
//     genCode(fp1);
//     globalHead = qp;
//     while (globalHead != NULL)
//     {
//         printf("%20s %20s %20s %20s\n", EnumToOperatorString(globalHead->op), globalHead->operand1, globalHead->operand2, globalHead->resultant);
//         globalHead = globalHead->next;
//     }
// }