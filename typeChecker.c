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

void checkBounds(astNode *arr_ele, SymTablePointer *ptr, SymTablePointer *parent);

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
        break;
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
        SymTablePointer *pointer = getFromAnySymTable(symTable, node->tk->str);
        if (pointer->isArray)
        {
            types type = pointer->typeIfArray.type;
            if (type == TYPE_INTEGER)
            {
                node->type = TYPE_ARR_INT;
            }
            else if (type == TYPE_REAL)
            {
                node->type = TYPE_ARR_REAL;
            }
            else
            {
                node->type = TYPE_ARR_BOOL;
            }
        }
        else
        {
            node->type = pointer->typeIfNotArray;
        }
        break;
    }
    case AST_ARRAY_ELEMENT:
    {

        SymTablePointer *pointer = getFromAnySymTable(symTable, node->leftChild->tk->str);
        // check bounds;
        checkBounds(node, pointer, symTable);
        break;
    }
    }
}

char *EnumToTypeString(types type)
{
    if (type == TYPE_INTEGER)
        return "integer";
    else if (type == TYPE_REAL)
        return "real";
    return "boolean";
}

void checkBounds(astNode *arr_ele, SymTablePointer *ptr, SymTablePointer *parent)
{

    astNode *bound = arr_ele->leftChild->nextSibling;
    if (bound->label == AST_NUM)
    {
        // check bounds
        if (!ptr->typeIfArray.high_ && !ptr->typeIfArray.low_)
        {
            if (!(bound->tk->integer >= ptr->typeIfArray.low && bound->tk->integer <= ptr->typeIfArray.high))
            {
                printf("Error: array index out of bounds at line number %d\n", bound->tk->line_num);
                // type_error?
            }
        }
        bound->type = TYPE_INTEGER;
        arr_ele->type = ptr->typeIfArray.type;
    }
    else if (bound->label == AST_RNUM)
    {
        printf("Error: array index is expected to be of integer, received real at line number %d\n", bound->tk->line_num);
        arr_ele->type = TYPE_ERROR;
    }
    else if (bound->label == AST_SIGNED)
    {
        getAttributeType(bound->leftChild->nextSibling, parent);
        if (bound->leftChild->nextSibling->label == AST_NUM)
        {
            if (bound->leftChild->tk->token == MINUS)
            {
                // check bounds
                int signBound = -bound->leftChild->nextSibling->tk->integer;
                if (!ptr->typeIfArray.high_ && !ptr->typeIfArray.low_)
                {
                    if (!(signBound >= ptr->typeIfArray.low && signBound <= ptr->typeIfArray.high))
                    {
                        printf("Error: array index out of bounds at line number %d\n", bound->leftChild->nextSibling->tk->line_num);
                    }
                }
            }
            else
            {
                int signBound = bound->leftChild->nextSibling->tk->integer;
                if (!ptr->typeIfArray.high_ && !ptr->typeIfArray.low_)
                {
                    if (!(signBound >= ptr->typeIfArray.low && signBound <= ptr->typeIfArray.high))
                    {
                        printf("Error: array index out of bounds at line number %d\n", bound->leftChild->nextSibling->tk->line_num);
                    }
                }
            }
            arr_ele->type = ptr->typeIfArray.type;
        }
    }
    else
    {
        getAttributeType(bound, parent);
        if (bound->type != TYPE_INTEGER)
        {
            printf("Error: array index is expected to be of integer, received %s at line number %d\n", EnumToTypeString(bound->type), bound->tk->line_num);
            arr_ele->type = TYPE_ERROR;
        }
        else
        {
            arr_ele->type = ptr->typeIfArray.type;
        }
        // delay bound checking until runtime
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
            if (stmts->leftChild->nextSibling->type == TYPE_ERROR || stmts->leftChild->type == TYPE_ERROR)
            {
                int line;
                if (stmts->leftChild->tk != NULL)
                {
                    line = stmts->leftChild->tk->line_num;
                }
                else
                {
                    line = stmts->leftChild->leftChild->tk->line_num;
                }
                printf("Error: Invalid operands for operation on line number %d\n", line);
            }
            else if (stmts->leftChild->type != stmts->leftChild->nextSibling->type)
            {

                int line;
                if (stmts->leftChild->tk != NULL)
                {
                    line = stmts->leftChild->tk->line_num;
                }
                else
                {
                    line = stmts->leftChild->leftChild->tk->line_num;
                }

                printf("Error: type mismatch at line number %d, cannot assign %s type value to %s\n", line, EnumToTypeString(stmts->leftChild->nextSibling->type), EnumToTypeString(stmts->leftChild->type));
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_FOR:
        {
            SymTablePointer *symTableInThisScope = stmts->symTable;
            astNode *stmtsNode = stmts->leftChild->nextSibling->nextSibling->leftChild;
            checkTypesForModule(symTableInThisScope, stmtsNode);
            stmts = stmts->nextSibling;
            break;
        }
        case AST_WHILE:
        {
            SymTablePointer *symTableInThisScope = stmts->symTable;
            astNode *stmtsNode = stmts->leftChild->nextSibling;
            checkTypesForModule(symTableInThisScope, stmtsNode);
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
    if (semanticallyCorrect)
        typeCheck(astRoot);
}