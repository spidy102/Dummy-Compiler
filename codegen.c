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
#include <string.h>

void genCode(FILE *fp)
{
    fprintf(fp, "global main\n");
    fprintf(fp, "extern printf\n");
    fprintf(fp, "extern scanf\n");
    fprintf(fp, "\n");
    fprintf(fp, "section .data");
    fprintf(fp, "section .text\n");
    while (globalHead != NULL)
    {
        switch (globalHead->op)
        {
        case OP_ASSIGN:

            break;
        case LABEL:
        {
            if (strcmp(globalHead->operand1, "driver") == 0)
            {
                fprintf(fp, "main:\n");
            }
            globalHead = globalHead->next;
        }
        default:
            break;
        }
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
    genCode(fp1);
    while (globalHead != NULL)
    {
        printf("%20s %20s %20s %20s\n", EnumToOperatorString(globalHead->op), globalHead->operand1, globalHead->operand2, globalHead->resultant);
        globalHead = globalHead->next;
    }
}