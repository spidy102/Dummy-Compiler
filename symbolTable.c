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
#include "symbolTableDef.h"
#include <stdbool.h>

bool semanticallyCorrect = true;

SymTablePointer *globalSymbolTable;

void populateTypeInformation(SymTablePointer *pointer, astNode *temp, SymTablePointer *parent)
{
    if (temp->label == AST_ARRAY_DATATYPE)
    {
        pointer->isArray = true;
        if (temp->leftChild->tk->token == INTEGER)
        {
            astNode *rangeNode = temp->leftChild->nextSibling;
            astNode *range1 = rangeNode->leftChild;
            astNode *range2 = range1->nextSibling;
            if (range1->label == AST_NUM && range2->label == AST_NUM)
            {
                pointer->typeIfArray.low = range1->tk->integer;
                if (range1->isNegative)
                {
                    pointer->typeIfArray.low = -range1->tk->integer;
                }
                pointer->typeIfArray.high = range2->tk->integer;
                if (range2->isNegative)
                {
                    pointer->typeIfArray.high = -range2->tk->integer;
                }
            }
            else
            {
                if (range1->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range1->tk->str))
                    {
                        printf("Error: variable %s at line number %d is not declared\n", range1->tk->str, range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range1->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Error: received non integral array bounds at line number %d\n", range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.low_ = true;
                }
                if (range2->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range2->tk->str))
                    {
                        printf("Error: variable %s at line number %d is not declared\n", range2->tk->str, range2->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range2->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Error: received non integral array bounds at line number %d\n", range2->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.high_ = true;
                }
            }
            pointer->typeIfArray.type = TYPE_INTEGER;
        }
        else if (temp->leftChild->tk->token == BOOLEAN)
        {
            astNode *rangeNode = temp->leftChild->nextSibling;
            astNode *range1 = rangeNode->leftChild;
            astNode *range2 = range1->nextSibling;
            if (range1->label == AST_NUM && range2->label == AST_NUM)
            {
                pointer->typeIfArray.low = range1->tk->integer;
                if (range1->isNegative)
                {
                    pointer->typeIfArray.low = -range1->tk->integer;
                }
                pointer->typeIfArray.high = range2->tk->integer;
                if (range2->isNegative)
                {
                    pointer->typeIfArray.high = -range2->tk->integer;
                }
            }
            else
            {
                if (range1->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range1->tk->str))
                    {
                        printf("Error: variable %s at line number %d is not declared\n", range1->tk->str, range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range1->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Error: received non integral array bounds at line number %d\n", range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.low_ = true;
                }
                if (range2->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range2->tk->str))
                    {
                        printf("Error: variable %s at line number %d is not declared\n", range2->tk->str, range2->tk->line_num);
                    }
                    else if (getFromAnySymTable(parent, range2->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Error: received non integral array bounds at line number %d\n", range2->tk->line_num);
                    }
                    pointer->typeIfArray.high_ = true;
                }
            }
            pointer->typeIfArray.type = TYPE_BOOLEAN;
        }
        else if (temp->leftChild->tk->token == REAL)
        {
            astNode *rangeNode = temp->leftChild->nextSibling;
            astNode *range1 = rangeNode->leftChild;
            astNode *range2 = range1->nextSibling;
            if (range1->label == AST_NUM && range2->label == AST_NUM)
            {
                pointer->typeIfArray.low = range1->tk->integer;
                if (range1->isNegative)
                {
                    pointer->typeIfArray.low = -range1->tk->integer;
                }
                pointer->typeIfArray.high = range2->tk->integer;
                if (range2->isNegative)
                {
                    pointer->typeIfArray.high = -range2->tk->integer;
                }
            }
            else
            {
                if (range1->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range1->tk->str))
                    {
                        printf("Error: variable %s in array bound for %s at line number %d is not declared\n", range1->tk->str, pointer->str, range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range1->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Error: received non integral array bounds at line number %d\n", range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.low_ = true;
                }
                if (range2->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range2->tk->str))
                    {
                        printf("Error: variable %s at line number %d is not declared", range2->tk->str, range2->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range2->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Error: received non integral array bounds at line number %d\n", range2->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.high_ = true;
                }
            }
            pointer->typeIfArray.type = TYPE_REAL;
        }
    }
    else
    {
        pointer->isArray = false;

        if (temp->tk->token == INTEGER)
        {

            pointer->typeIfNotArray = TYPE_INTEGER;
        }
        else if (temp->tk->token == BOOLEAN)
        {
            pointer->typeIfNotArray = TYPE_BOOLEAN;
        }
        else
        {
            pointer->typeIfNotArray = TYPE_REAL;
        }
    }
}

int getOffset(SymTablePointer *pointer, int *offset)
{
    if (!pointer->isArray)
    {
        if (pointer->typeIfNotArray == TYPE_INTEGER)
        {
            int ret = *offset;
            *offset += INT_WIDTH;
            return ret;
        }
        else if (pointer->typeIfNotArray == TYPE_BOOLEAN)
        {
            int ret = *offset;
            *offset += BOOL_WIDTH;
            return ret;
        }
        else
        {
            int ret = *offset;
            *offset += REAL_WIDTH;
            return ret;
        }
    }
    else
    {
        array_type type = pointer->typeIfArray;
        if (type.type == TYPE_INTEGER)
        {
            int ret = *offset;
            *offset += (type.high - type.low + 1) * INT_WIDTH;
            return ret;
        }
        else if (type.type == TYPE_BOOLEAN)
        {
            int ret = *offset;
            *offset += (type.high - type.low + 1) * BOOL_WIDTH;
            return ret;
        }
        else
        {
            int ret = *offset;
            *offset += (type.high - type.low + 1) * TYPE_REAL;
            return ret;
        }
    }
}

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
            head->isArray = false;
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
            // printf("%s\n\n\n\n", pl->leftChild->leftChild->nextSibling->nextSibling->tk->str);
            head->isArray = true;
            astNode *arr_type = pl->leftChild;
            // HAVE TO INCORPORATE SIGNED CASES TOO
            head->typeIfArray.low = arr_type->leftChild->nextSibling->leftChild->tk->integer;
            if (arr_type->leftChild->nextSibling->leftChild->isNegative)
            {
                head->typeIfArray.low = -head->typeIfArray.low;
            }
            head->typeIfArray.high = arr_type->leftChild->nextSibling->leftChild->nextSibling->tk->integer;
            if (arr_type->leftChild->nextSibling->leftChild->nextSibling->isNegative)
            {
                head->typeIfArray.high = -head->typeIfArray.high;
            }
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
            semanticallyCorrect = false;
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

                if (existsInSymTable(module->corrHashtable, idList->tk->str))
                {
                    // if (module->typeST==MODULEST) {

                    // }
                    printf("Error: Redeclaration of variable %s at line number %d\n", idList->tk->str, idList->tk->line_num);
                    semanticallyCorrect = false;
                }
                else
                {

                    SymTablePointer *pointer = initSymTablePointer();
                    // pointer->typeExpression = stmts->leftChild->nextSibling;
                    pointer->str = idList->tk->str;
                    populateTypeInformation(pointer, stmts->leftChild->nextSibling, module);
                    pointer->offset = getOffset(pointer, offset);
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
                    semanticallyCorrect = false;
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
                semanticallyCorrect = false;
            }

            hashtable *newScopeHashTable = initHashtableForSymTable();
            SymTablePointer *newPointer = initSymTablePointer();
            newPointer->typeST = SCOPEST;
            newPointer->corrHashtable = newScopeHashTable;
            newPointer->parentHashTable = module;

            stmts->symTable = newPointer; // attach sym table with ast node;

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
                int offset1 = 0;
                populateStmtsSymTable(newPointer, cases->leftChild->nextSibling->leftChild, &offset1);
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
                    if (children->leftChild == NULL)
                    {
                        children = children->nextSibling;
                        break;
                    }
                    astNode *idList = children->leftChild->leftChild;

                    while (idList != NULL)
                    {
                        if (!existsInAnySymTable(module, idList->tk->str))
                        {
                            printf("Error: Variable %s at line number %d is not declared in this scope\n", idList->tk->str, idList->tk->line_num);
                            semanticallyCorrect = false;
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
                        semanticallyCorrect = false;
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
                                semanticallyCorrect = false;
                            }
                        }
                        else
                        {
                            astNode *idNode = idList;
                            if (!existsInAnySymTable(module, idNode->tk->str))
                            {
                                printf("Error: variable %s at line number %d has not been declared\n", idNode->tk->str, idNode->tk->line_num);
                                semanticallyCorrect = false;
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
            forScope->typeST = SCOPEST;
            stmts->symTable = forScope; // attach sym table with for's new scope;
            hashtable *forScopeHashTable = initHashtableForSymTable();
            forScope->corrHashtable = forScopeHashTable;
            SymTablePointer *var = initSymTablePointer();
            var->typeIfNotArray = TYPE_INTEGER;
            var->isArray = false;
            var->offset = getOffset(var, offset);
            var->tk = stmts->leftChild->tk;
            var->str = stmts->leftChild->tk->str;
            insertSymTable(forScope->corrHashtable, var);
            astNode *forStmts = stmts->leftChild->nextSibling->nextSibling->leftChild;
            int offset1 = 0;
            populateStmtsSymTable(forScope, forStmts, &offset1);

            stmts = stmts->nextSibling;
            break;
        }
        case AST_WHILE:
        {
            SymTablePointer *whileScope = initSymTablePointer();
            module->childScopeTable = append_scope_pointer(module->childScopeTable, whileScope);
            whileScope->parentHashTable = module;
            whileScope->typeST = SCOPEST;
            stmts->symTable = whileScope;
            hashtable *whileScopeHashTable = initHashtableForSymTable();
            whileScope->corrHashtable = whileScopeHashTable;
            astNode *whileStmts = stmts->leftChild->nextSibling->leftChild;
            traverse_ast(stmts->leftChild, module);
            int offset1 = 0;
            populateStmtsSymTable(whileScope, whileStmts, &offset1);

            stmts = stmts->nextSibling;
            break;
        }
        }
    }
}

void populateModuleSymbolTable(SymTablePointer *module, astNode *root, int *offset)
{
    hashtable *moduleST = module->corrHashtable;

    astNode *mdlDef = root->leftChild->nextSibling->nextSibling->nextSibling;
    if (mdlDef->leftChild == NULL)
    {
        return;
    }
    astNode *stmts = mdlDef->leftChild->leftChild;

    populateStmtsSymTable(module, stmts, offset);
}

void populateGlobalSymbolTable(SymTablePointer *global, astNode *astRoot, int offset)
{
    hashtable *globalST = global->corrHashtable;
    astRoot->symTable = global;

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
                SymTablePointer *pointer = initSymTablePointer();

                pointer->str = corrID->tk->str;
                if (existsInSymTable(globalST, pointer->str) && !getFromSymTable(globalST, pointer->str)->isAwaited)
                {
                    printf("Error: Module with name %s at line number %d was defined previously!\n", corrID->tk->str, corrID->tk->line_num);
                    semanticallyCorrect = false;
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
                    astNode *oplNode = mdls->leftChild->nextSibling->nextSibling->leftChild;
                    pointer->input_para_list = NULL;
                    pointer->output_para_list = NULL;
                    if (iplNode != NULL)
                        pointer->input_para_list = getListFromAST(iplNode, &offset);
                    if (oplNode != NULL)
                        pointer->output_para_list = getListFromAST(oplNode, &offset);
                    hashtable *ht = initHashtableForSymTable();
                    pointer->corrHashtable = ht;
                    astNode *temp = iplNode;
                    int offset = 0;
                    while (temp != NULL)
                    {
                        SymTablePointer *pointer1 = initSymTablePointer();

                        populateTypeInformation(pointer1, temp->leftChild, pointer);
                        pointer1->offset = getOffset(pointer1, &offset);
                        pointer1->str = temp->leftChild->nextSibling->tk->str;
                        insertSymTable(pointer->corrHashtable, pointer1);
                        temp = temp->nextSibling;
                    }
                    temp = oplNode;
                    while (temp != NULL)
                    {
                        SymTablePointer *pointer1 = initSymTablePointer();

                        populateTypeInformation(pointer1, temp->leftChild, pointer);
                        pointer1->offset = getOffset(pointer1, &offset);
                        pointer1->str = temp->leftChild->nextSibling->tk->str;
                        insertSymTable(pointer->corrHashtable, pointer1);
                        temp = temp->nextSibling;
                    }
                    pointer->typeST = MODULEST;
                    pointer->parentHashTable = global;
                    populateModuleSymbolTable(pointer, mdls, &offset);
                    mdls->symTable = pointer;
                    // if (existsInSymTable(pointer->corrHashtable, "ty"))
                    // {
                    //     printf("yes exists\n\n");
                    // }

                    // temp = pointer->output_para_list;
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
            SymTablePointer *pointer = initSymTablePointer();
            pointer->str = "driver";
            hashtable *ht = initHashtableForSymTable();
            root->symTable = pointer;
            pointer->corrHashtable = ht;
            pointer->typeST = MODULEST;
            pointer->parentHashTable = global;
            astNode *stmts = root->leftChild->leftChild->leftChild;
            populateStmtsSymTable(pointer, stmts, &offset);
            root = root->nextSibling;
            insertSymTable(globalST, pointer);
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
                    semanticallyCorrect = false;
                }
                node = node->next;
            }
        }
    }
}

// int main()
// {
//     globalSymbolTable = initSymTablePointer();
//     globalSymbolTable->typeST = GLOBALST;
//     globalSymbolTable->parentHashTable = NULL;
//     hashtable *ht1 = initHashtableForSymTable();
//     globalSymbolTable->corrHashtable = ht1;
//     FILE *fp = fopen("random1.txt", "r");
//     twinbuffer *tb = twinbuffer_init(fp, 256);
//     fill_grammar(fopen("Grammar.txt", "r"));
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
//     populateParseTable();
//     treenode *root = parseInputSourceCode(fp, tb, ht);

//     astNode *astRoot = constructAST(root);
//     inorder_ast(astRoot);
//     populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
// }