#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "parserDef.h"
#include "parser.h"
#include "hashmap.h"
#include "ast.h"
#include "astDef.h"
#include "symTableUtil.h"
#include "symbolTable.h"

SymTablePointer *globalSymbolTable;

SymTablePointer *append_scope_pointer(SymTablePointer *head, SymTablePointer *append)
{
    if (head == NULL)
    {
        return append;
    }
    SymTablePointer *temp = head;
    while (temp->siblingHashTable != NULL)
    {
        temp = temp->siblingHashTable;
    }
    temp->siblingHashTable = append;
    return head;
}

types getType(astNode *type)
{
    if (type->tk->token == INTEGER)
    {
        return TYPE_INTEGER;
    }
    else if (type->tk->token == REAL)
    {
        return TYPE_REAL;
    }
    else if (type->tk->token == BOOLEAN)
    {
        return TYPE_BOOLEAN;
    }
}

list *getListFromAST(astNode *pl, int *offset)
{
    list *head = malloc(sizeof(list));
    list *ret = head;
    while (pl != NULL)
    {
        if (pl->leftChild->label != AST_ARRAY_DATATYPE)
        {
            head->typeIfNotArray = getType(pl->leftChild);
            if (head->typeIfNotArray == TYPE_INTEGER)
            {
                head->width = *offset + INT_WIDTH;
                *offset += 4;
            }
            else if (head->typeIfNotArray == TYPE_BOOLEAN)
            {
                head->width = *offset + BOOL_WIDTH;
                *offset += 1;
            }
            else
            {
                head->width = *offset + REAL_WIDTH;
            }
            head->tk = pl->leftChild->nextSibling->tk;
        }
        else
        {

            astNode *arr_type = pl->leftChild;
            // HAVE TO INCORPORATE SIGNED CASES TOO
            head->typeIfArray.low = arr_type->leftChild->nextSibling->leftChild->tk->integer;
            head->typeIfArray.high = arr_type->leftChild->nextSibling->leftChild->nextSibling->tk->integer;
            head->typeIfArray.type = getType(arr_type->leftChild);
            // range checks?
            head->tk = pl->leftChild->nextSibling->tk;
            head->width = (head->typeIfArray.high - head->typeIfArray.low + 1) * (head->typeIfArray.type == TYPE_INTEGER ? INT_WIDTH : (head->typeIfArray.type == TYPE_BOOLEAN ? (BOOL_WIDTH) : (REAL_WIDTH)));
        }
        pl = pl->nextSibling;
        if (pl != NULL)
        {
            head->next = malloc(sizeof(list));
            head = head->next;
        }
        else
        {
            head->next = NULL;
        }
    }
    return ret;
}

SymTablePointer *initSymTablePointer()
{
    SymTablePointer *pointer = malloc(sizeof(SymTablePointer));
    pointer->corrHashtable = NULL;
    pointer->isAwaited = false;
    // pointer->typeExpression = NULL;
    pointer->str = NULL;
}

void traverse_ast(astNode *root, SymTablePointer *st)
{
    if (root == NULL)
    {
        return;
    }
    traverse_ast(root->leftChild, st);
    if (root->label == AST_ID)
    {
        if (!existsInAnySymTable(st, root->tk->str))
        {
            printf("Error: variable %s at line number %d has not been declared in this scope\n", root->tk->str, root->tk->line_num);
        }
    }
    if (root->leftChild != NULL)
    {
        astNode *leftOut = root->leftChild->nextSibling;
        while (leftOut != NULL)
        {
            traverse_ast(leftOut, st);
            leftOut = leftOut->nextSibling;
        }
    }
}

void populateStmtsSymTable(SymTablePointer *module, astNode *stmts, int *offset)
{
    hashtable *ht = module->corrHashtable;

    while (stmts != NULL)
    {
        switch (stmts->label)
        {
        case AST_DECLARE:
        {
            astNode *idList = stmts->leftChild->leftChild;
            while (idList != NULL)
            {

                if (existsInAnySymTable(module, idList->tk->str))
                {

                    printf("Error: Redeclaration of variable %s at line number %d\n", idList->tk->str, idList->tk->line_num);
                }
                else
                {
                    SymTablePointer *pointer = initSymTablePointer();
                    // pointer->typeExpression = stmts->leftChild->nextSibling;
                    if (stmts->leftChild->nextSibling->label == AST_ARRAY_DATATYPE)
                    {
                        astNode *arr_type = stmts->leftChild->nextSibling;
                        pointer->typeIfArray.low = arr_type->leftChild->nextSibling->leftChild->tk->integer;
                        pointer->typeIfArray.high = arr_type->leftChild->nextSibling->leftChild->nextSibling->tk->integer;
                        pointer->typeIfArray.type = getType(arr_type->leftChild);
                        *offset += *offset + (pointer->typeIfArray.high - pointer->typeIfArray.low + 1) * (pointer->typeIfArray.type == TYPE_INTEGER ? INT_WIDTH : (pointer->typeIfArray.type == TYPE_BOOLEAN ? (BOOL_WIDTH) : (REAL_WIDTH)));
                        pointer->offset = *offset;
                    }
                    else
                    {
                        pointer->typeIfNotArray = getType(stmts->leftChild->nextSibling);
                        *offset += *offset + (pointer->typeIfNotArray == TYPE_INTEGER ? INT_WIDTH : (pointer->typeIfNotArray == TYPE_BOOLEAN ? BOOL_WIDTH : REAL_WIDTH));
                    }
                    pointer->str = idList->tk->str;
                    pointer->tk = idList->tk;
                    insertSymTable(ht, pointer);
                }
                idList = idList->nextSibling;
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_IOSTMT:
        {
            if (stmts->leftChild->label == AST_ID)
            {
                if (!existsInAnySymTable(module, stmts->leftChild->tk->str))
                {
                    printf("Error: variable %s at line number %d is not defined in this scope\n", stmts->leftChild->tk->str, stmts->leftChild->tk->line_num);
                }
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_SWITCH_CASE:
        {

            astNode *idNode = stmts->leftChild;
            if (!existsInAnySymTable(module, idNode->tk->str))
            {
                printf("Error: variable %s at line number %d is not defined in this scope\n", idNode->tk->str, idNode->tk->line_num);
            }

            hashtable newScopeHashTable = initHashtable();
            SymTablePointer *newPointer = initSymTablePointer();
            newPointer->corrHashtable = &newScopeHashTable;
            newPointer->parentHashTable = module;

            if (module->childScopeTable == NULL)
            {
                module->childScopeTable = newPointer;
            }
            else
            {
                SymTablePointer *temp = module->childScopeTable;
                while (temp->siblingHashTable != NULL)
                {
                    temp = temp->siblingHashTable;
                }
                temp->siblingHashTable = newPointer;
            }
            astNode *cases = stmts->leftChild->nextSibling->leftChild;
            while (cases != NULL)
            {
                populateStmtsSymTable(newPointer, cases->leftChild->nextSibling->leftChild, offset);
                cases = cases->nextSibling;
            }

            stmts = stmts->nextSibling;
            break;
        }
        case AST_MODULE_REUSE:
        {
            astNode *children = stmts->leftChild;
            while (children != NULL)
            {
                switch (children->label)
                {
                case AST_OPTIONAL:
                {
                    astNode *idList = children->leftChild->leftChild;

                    while (idList != NULL)
                    {
                        if (!existsInAnySymTable(module, idList->tk->str))
                        {
                            printf("Error: Variable %s at line number %d is not declared in this scope\n", idList->tk->str, idList->tk->line_num);
                        }
                        idList = idList->nextSibling;
                    }
                    children = children->nextSibling;
                    break;
                }
                case AST_ID:
                {
                    if (!existsInSymTable(globalSymbolTable->corrHashtable, children->tk->str))
                    {
                        printf("Error: module %s at line number %d has not been defined\n", children->tk->str, children->tk->line_num);
                    }
                    children = children->nextSibling;
                    break;
                }
                case AST_ACTUAL_PARA:
                {
                    astNode *idList = children->leftChild;
                    while (idList != NULL)
                    {
                        if (idList->label == AST_SIGNED)
                        {
                            astNode *idNode = idList->leftChild->nextSibling;
                            if (!existsInAnySymTable(module, idNode->tk->str))
                            {
                                printf("Error: variable %s at line number %d has not been declared\n", idNode->tk->str, idNode->tk->line_num);
                            }
                        }
                        else
                        {
                            astNode *idNode = idList;
                            if (!existsInAnySymTable(module, idNode->tk->str))
                            {
                                printf("Error: variable %s at line number %d has not been declared\n", idNode->tk->str, idNode->tk->line_num);
                            }
                        }
                        idList = idList->nextSibling;
                    }
                    children = children->nextSibling;
                    break;
                }
                }
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_ASSIGNOP:
        {
            traverse_ast(stmts, module);
            stmts = stmts->nextSibling;
            break;
        }
        case AST_FOR:
        {
            SymTablePointer *forScope = initSymTablePointer();
            module->childScopeTable = append_scope_pointer(module->childScopeTable, forScope);
            forScope->parentHashTable = module;
            hashtable forScopeHashTable = initHashtable();
            forScope->corrHashtable = &forScopeHashTable;
            SymTablePointer *var = initSymTablePointer();
            var->tk = stmts->leftChild->tk;
            var->str = stmts->leftChild->tk->str;
            insertSymTable(forScope->corrHashtable, var);
            astNode *forStmts = stmts->leftChild->nextSibling->nextSibling->leftChild;

            populateStmtsSymTable(forScope, forStmts, offset);

            stmts = stmts->nextSibling;
            break;
        }
        case AST_WHILE:
        {
            SymTablePointer *whileScope = initSymTablePointer();
            module->childScopeTable = append_scope_pointer(module->childScopeTable, whileScope);
            whileScope->parentHashTable = module;
            hashtable whileScopeHashTable = initHashtable();
            whileScope->corrHashtable = &whileScopeHashTable;
            astNode *whileStmts = stmts->leftChild->nextSibling->leftChild;
            traverse_ast(stmts->leftChild, module);
            populateStmtsSymTable(whileScope, whileStmts, offset);

            stmts = stmts->nextSibling;
            break;
        }
        }
    }
}

void populateModuleSymbolTable(SymTablePointer *module, astNode *root)
{
    hashtable *moduleST = module->corrHashtable;
    astNode *mdlDef = root->leftChild->nextSibling->nextSibling->nextSibling;
    if (mdlDef->leftChild == NULL)
    {
        return;
    }
    astNode *stmts = mdlDef->leftChild->leftChild;
    int offset = 0;

    populateStmtsSymTable(module, stmts, &offset);
}

void populateGlobalSymbolTable(SymTablePointer *global, astNode *astRoot, int offset)
{
    hashtable *globalST = global->corrHashtable;
    astNode *root = astRoot->leftChild;
    while (root != NULL)
    {
        switch (root->label)
        {
        case AST_MODULE_DECLARATIONS:
        {
            astNode *mdcrls = root->leftChild;
            while (mdcrls != NULL)
            {
                astNode *corrID = mdcrls->leftChild;
                if (existsInSymTable(globalST, corrID->tk->str))
                {
                    fprintf(stderr, "Module %s at line number %d has been declared previously\n", corrID->tk->str, corrID->tk->line_num);
                }
                else
                {
                    SymTablePointer *pointer = initSymTablePointer();
                    pointer->isAwaited = true;
                    pointer->str = corrID->tk->str;
                    pointer->tk = corrID->tk;
                    pointer->parentHashTable = global;
                    insertSymTable(globalST, pointer);
                }
                mdcrls = mdcrls->nextSibling;
            }
            root = root->nextSibling;
            break;
        }
        case AST_OTHERMODULES:
        {
            astNode *mdls = root->leftChild;
            while (mdls != NULL)
            {
                astNode *corrID = mdls->leftChild;
                SymTablePointer *pointer = malloc(sizeof(SymTablePointer));
                pointer->str = corrID->tk->str;
                if (existsInSymTable(globalST, pointer->str) && !getFromSymTable(globalST, pointer->str)->isAwaited)
                {
                    printf("Error: Module with name %s at line number %d was defined previously!\n", corrID->tk->str, corrID->tk->line_num);
                }
                else
                {
                    if (existsInSymTable(globalST, pointer->str))
                    {
                        getFromSymTable(globalST, pointer->str)->isAwaited = false;
                    }
                    insertSymTable(globalST, pointer);
                    // populate type expression
                    astNode *iplNode = mdls->leftChild->nextSibling->leftChild;
                    astNode *oplNode = iplNode->nextSibling;
                    pointer->input_para_list = getListFromAST(iplNode, &offset);
                    pointer->output_para_list = getListFromAST(oplNode, &offset);
                    hashtable ht = initHashtable();
                    pointer->corrHashtable = &ht;
                    list *temp = pointer->input_para_list;
                    while (temp != NULL)
                    {

                        SymTablePointer *var = initSymTablePointer();
                        var->str = temp->tk->str;
                        printf("%d\n", iplNode->leftChild->nextSibling->tk->token);
                        var->tk = temp->tk;

                        insertSymTable(pointer->corrHashtable, var);
                        temp = temp->next;
                    }
                    pointer->typeST = MODULEST;
                    pointer->parentHashTable = global;
                    populateModuleSymbolTable(pointer, mdls);
                    temp = pointer->output_para_list;
                    // while (temp != NULL)
                    // {
                    //     if (!existsInSymTable(pointer->corrHashtable, temp->tk->str))
                    //     {
                    //         printf("Error: return variable %s is not defined in the module %s\n", temp->tk->str, pointer->str);

                    //     } // assuming should be defined in that scope
                    //     temp = temp->next;
                    // }
                }

                mdls = mdls->nextSibling;
            }
            root = root->nextSibling;

            break;
        }
        case AST_DRIVERMODULE:
        {
            SymTablePointer *pointer = malloc(sizeof(SymTablePointer));
            pointer->str = "driver";
            hashtable ht = initHashtable();
            pointer->corrHashtable = &ht;
            astNode *stmts = root->leftChild->leftChild->leftChild;
            populateStmtsSymTable(pointer, stmts, &offset);
            root = root->nextSibling;
            break;
        }
        }
    }
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (globalST->table[i] != NULL)
        {
            SymTablePointer *node = globalST->table[i]->bucket_ptr;
            while (node != NULL)
            {
                if (node->isAwaited)
                {
                    printf("Error: module %s declared at line number %d is never defined\n", node->str, node->tk->line_num);
                }
                node = node->next;
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
    FILE *fp = fopen("random.txt", "r");
    twinbuffer *tb = twinbuffer_init(fp, 256);
    fill_grammar(fopen("Grammar.txt", "r"));
    hashtable ht = initHashtable();
    populate_hashtable(&ht);
    populateParseTable();
    treenode *root = parseInputSourceCode(fp, tb, ht);
    astNode *astRoot = constructAST(root);
    inorder_ast(astRoot);
    populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
}