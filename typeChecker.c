#include <stdio.h>
#include "lexerDef.h"
#include "lexer.h"
#include "tree.h"
#include "parserDef.h"
#include "parser.h"
#include "ast.h"
#include "astDef.h"
#include "symbolTable.h"
#include "symbolTableDef.h"
#include "symTableUtil.h"

void getAttributeType(astNode *node, SymTablePointer *symTable)
{
    switch (node->label)
    {
    case AST_PLUS:
    case AST_MINUS:
    case AST_MUL:
    {

        getAttributeType(node->leftChild, symTable);
        getAttributeType(node->leftChild->nextSibling, symTable);
        if (node->leftChild->type == TYPE_INTEGER && node->leftChild->nextSibling->type == TYPE_INTEGER)
        {
            node->type = TYPE_INTEGER;
        }
        else if (node->leftChild->type == TYPE_REAL && node->leftChild->nextSibling->type == TYPE_REAL)
        {
            node->type = TYPE_REAL;
        }
        else
        {
            node->type = TYPE_ERROR;
        }
        break;
    }
    case AST_DIV:
    {
        getAttributeType(node->leftChild, symTable);
        getAttributeType(node->leftChild->nextSibling, symTable);
        if (node->leftChild->type == TYPE_INTEGER && node->leftChild->nextSibling->type == TYPE_INTEGER)
        {
            node->type = TYPE_REAL;
        }
        else if (node->leftChild->type == TYPE_REAL && node->leftChild->nextSibling->type == TYPE_REAL)
        {
            node->type = TYPE_REAL;
        }
        else if ((node->leftChild->type == TYPE_REAL && node->leftChild->nextSibling->type == TYPE_INTEGER) || (node->leftChild->type == TYPE_INTEGER && node->leftChild->nextSibling->type == TYPE_REAL))
        {
            node->type = TYPE_REAL;
        }
        else
        {
            node->type = TYPE_ERROR;
        }
        break;
    }
    case AST_GT:
    case AST_GE:
    case AST_LT:
    case AST_LE:
    case AST_EQ:
    case AST_NE:
    {
        getAttributeType(node->leftChild, symTable);
        getAttributeType(node->leftChild->nextSibling, symTable);
        if (node->leftChild->type == TYPE_INTEGER && node->leftChild->nextSibling->type == TYPE_INTEGER)
        {
            node->type = TYPE_BOOLEAN;
        }
        else if (node->leftChild->type == TYPE_REAL && node->leftChild->nextSibling->type == TYPE_REAL)
        {
            node->type = TYPE_BOOLEAN;
        }
        else
        {
            node->type = TYPE_ERROR;
        }
        break;
    }
    case AST_AND:
    case AST_OR:
    {
        getAttributeType(node->leftChild, symTable);
        getAttributeType(node->leftChild->nextSibling, symTable);
        if (node->leftChild->type == TYPE_BOOLEAN && node->leftChild->nextSibling->type == TYPE_BOOLEAN)
        {
            node->type = TYPE_BOOLEAN;
        }
        else
        {
            node->type = TYPE_ERROR;
        }
        break;
    }
    case AST_NUM:
    {

        node->type = TYPE_INTEGER;
        BREAK;
    }
    case AST_RNUM:
    {
        node->type = TYPE_REAL;
        break;
    }
    case AST_BOOL:
    {
        node->type = TYPE_BOOLEAN;
        break;
    }
    case AST_ID:
    {
        SymTablePointer *pointer = getFromSymTable(symTable->corrHashtable, node->tk->str);
        node->type = pointer->typeIfNotArray;
        break;
    }
    }
}

void checkTypesForModule(SymTablePointer *symTable, astNode *stmts)
{
    while (stmts != NULL)
    {

        switch (stmts->label)
        {
        case AST_ASSIGNOP:
        {

            getAttributeType(stmts->leftChild, symTable);
            getAttributeType(stmts->leftChild->nextSibling, symTable);

            if (stmts->leftChild->nextSibling->type == TYPE_ERROR || stmts->leftChild->type != stmts->leftChild->nextSibling->type)
            {
                printf("Error: type mismatch at line number %d\n", stmts->leftChild->tk->line_num);
            }
            stmts = stmts->nextSibling;
            break;
        }
        default:
        {
            stmts = stmts->nextSibling;
            break;
        }
        }
    }
}

void typeCheck(astNode *root)
{
    astNode *modules = root->leftChild;
    while (modules != NULL)
    {
        switch (modules->label)
        {
        case AST_OTHERMODULES:
        {

            astNode *separateModules = modules->leftChild;
            while (separateModules != NULL)
            {
                SymTablePointer *symTableForCurrentModule = separateModules->symTable;
                astNode *stmtsForCurrentModule = separateModules->leftChild->nextSibling->nextSibling->nextSibling->leftChild->leftChild;

                checkTypesForModule(symTableForCurrentModule, stmtsForCurrentModule);
                separateModules = separateModules->nextSibling;
            }
            modules = modules->nextSibling;
            break;
        }
        default:
        {
            modules = modules->nextSibling;
            break;
        }
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
    FILE *fp = fopen("random1.txt", "r");
    twinbuffer *tb = twinbuffer_init(fp, 256);
    fill_grammar(fopen("Grammar.txt", "r"));
    hashtable ht = initHashtable();
    populate_hashtable(&ht);
    populateParseTable();
    treenode *root = parseInputSourceCode(fp, tb, ht);
    astNode *astRoot = constructAST(root);
    inorder_ast(astRoot);
    populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
    typeCheck(astRoot);
}