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
#include "typeCheckerDef.h"
#include <stdbool.h>
#include <string.h>

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
                        printf("Line %d: Error: variable %s is not declared\n", range1->tk->line_num, range1->tk->str);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range1->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Line %d: Error: received non integral array bounds\n", range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.low_ = true;
                    pointer->typeIfArray.lowLexeme = range1->tk->str;
                    pointer->typeIfArray.isNegLow = range1->isNegative;
                }
                if (range2->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range2->tk->str))
                    {
                        printf("Line %d: Error: variable %s is not declared\n", range2->tk->line_num, range2->tk->str);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range2->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Line %d: Error: received non integral array bounds\n", range2->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.high_ = true;
                    pointer->typeIfArray.highLexeme = range2->tk->str;
                    pointer->typeIfArray.isNegHigh = range2->isNegative;
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
                        printf("Line %d: Error: variable %s is not declared\n", range1->tk->line_num, range1->tk->str);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range1->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Line %d: Error: received non integral array bound\n", range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.low_ = true;
                    pointer->typeIfArray.lowLexeme = range1->tk->str;
                    pointer->typeIfArray.isNegLow = range1->isNegative;
                }
                if (range2->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range2->tk->str))
                    {
                        semanticallyCorrect = false;
                        printf("Line %d: Error: variable %s is not declared\n", range2->tk->line_num, range2->tk->str);
                    }
                    else if (getFromAnySymTable(parent, range2->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        semanticallyCorrect = false;
                        printf("Line %d: Error: received non integral array bounds\n", range2->tk->line_num);
                    }
                    pointer->typeIfArray.high_ = true;
                    pointer->typeIfArray.highLexeme = range2->tk->str;
                    pointer->typeIfArray.isNegHigh = range2->isNegative;
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
                        printf("Line %d: Error: variable %s in array bound for %s is not declared\n", range1->tk->line_num, range1->tk->str, pointer->str);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range1->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Line %d: Error: received non integral array bounds\n", range1->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.low_ = true;
                    pointer->typeIfArray.lowLexeme = range1->tk->str;
                    pointer->typeIfArray.isNegLow = range1->isNegative;
                }
                if (range2->label == AST_ID)
                {
                    if (!existsInAnySymTable(parent, range2->tk->str))
                    {
                        printf("Line %d: Error: variable %s is not declared", range2->tk->line_num, range2->tk->str);
                        semanticallyCorrect = false;
                    }
                    else if (getFromAnySymTable(parent, range2->tk->str)->typeIfNotArray != TYPE_INTEGER)
                    {
                        printf("Line %d: Error: received non integral array bounds\n", range2->tk->line_num);
                        semanticallyCorrect = false;
                    }
                    pointer->typeIfArray.high_ = true;
                    pointer->typeIfArray.highLexeme = range2->tk->str;
                    pointer->typeIfArray.isNegHigh = range2->isNegative;
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
            pointer->width = INT_WIDTH;
            int ret = *offset;
            *offset += INT_WIDTH;
            return ret;
        }
        else if (pointer->typeIfNotArray == TYPE_BOOLEAN)
        {
            pointer->width = BOOL_WIDTH;
            int ret = *offset;
            *offset += BOOL_WIDTH;
            return ret;
        }
        else
        {
            pointer->width = REAL_WIDTH;
            int ret = *offset;
            *offset += REAL_WIDTH;
            return ret;
        }
    }
    else
    {
        array_type type = pointer->typeIfArray;
        if (!type.low_ && !type.high_)
        {
            if (type.type == TYPE_INTEGER)
            {
                pointer->width = (type.high - type.low + 1) * INT_WIDTH + 1;
                int ret = *offset;
                *offset += pointer->width;
                return ret;
            }
            else if (type.type == TYPE_BOOLEAN)
            {
                pointer->width = (type.high - type.low) * BOOL_WIDTH + 1;
                int ret = *offset;
                *offset += pointer->width;
                return ret;
            }
            else
            {
                pointer->width = (type.high - type.low) * REAL_WIDTH + 1;
                int ret = *offset;
                *offset += pointer->width;
                return ret;
            }
        } // dynamic arrays treatment??
    }
}

SymTablePointer *append_scope_pointer(SymTablePointer *head, SymTablePointer *append)
{
    if (head == NULL)
    {
        append->siblingHashTable = NULL;
        return append;
    }
    SymTablePointer *temp = head;
    while (temp->siblingHashTable != NULL)
    {
        temp = temp->siblingHashTable;
    }
    temp->siblingHashTable = append;
    append->siblingHashTable = NULL;
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
                head->width = INT_WIDTH;
                head->offset = *offset;
                *offset += head->width;
            }
            else if (head->typeIfNotArray == TYPE_BOOLEAN)
            {
                head->width = BOOL_WIDTH;
                head->offset = *offset;
                *offset += head->width;
            }
            else
            {
                head->width = REAL_WIDTH;
                head->offset = *offset;
                *offset += head->width;
            }
            head->tk = pl->leftChild->nextSibling->tk;
        }
        else
        {
            // printf("%s\n\n\n\n", pl->leftChild->leftChild->nextSibling->nextSibling->tk->str);
            head->isArray = true;
            astNode *arr_type = pl->leftChild;
            // HAVE TO INCORPORATE SIGNED CASES TOO
            astNode *range1 = pl->leftChild->leftChild->nextSibling->leftChild;
            astNode *range2 = pl->leftChild->leftChild->nextSibling->leftChild->nextSibling;
            head->typeIfArray.type = getType(arr_type->leftChild);
            if (range1->label == AST_NUM && range2->label == AST_NUM)
            {
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
                // range checks?
                head->typeIfArray.high_ = false;
                head->typeIfArray.low_ = false;
                head->tk = pl->leftChild->nextSibling->tk;
                head->width = (head->typeIfArray.high - head->typeIfArray.low + 1) * (head->typeIfArray.type == TYPE_INTEGER ? INT_WIDTH : (head->typeIfArray.type == TYPE_BOOLEAN ? (BOOL_WIDTH) : (REAL_WIDTH))) + 1;
                head->offset = *offset;
                *offset += head->width;
            }
            else
            {
                if (range1->label == AST_ID)
                {
                    head->typeIfArray.low_ = true;
                    head->typeIfArray.lowLexeme = range1->tk->str;
                    head->typeIfArray.isNegLow = range1->isNegative;
                }
                if (range2->label == AST_ID)
                {
                    head->typeIfArray.high_ = true;
                    head->typeIfArray.highLexeme = range2->tk->str;
                    head->typeIfArray.isNegLow = range2->isNegative;
                }
            }
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
    pointer->isDeclared = false;
    pointer->isDefined = false;
    pointer->called = false;
    pointer->width = -1;
    pointer->offset = -1;
    pointer->line_start = -1;
    pointer->line_end = -1;
    pointer->childScopeTable = NULL;
    pointer->input_para_list = NULL;
    pointer->next = NULL;
    pointer->output_para_list = NULL;
    pointer->parentHashTable = NULL;
    pointer->tk = NULL;
    pointer->typeIfArray.low_ = false;
    pointer->typeIfArray.high_ = false;
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
            printf("Line %d: Error: variable %s has not been declared\n", root->tk->line_num, root->tk->str);
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

bool inInputParameters(SymTablePointer *module, char *str)
{
    while (module->typeST != MODULEST)
    {
        module = module->parentHashTable;
    }
    if (strcmp(module->str, "driver") != 0)
    {
        list *ipl = module->input_para_list;
        while (ipl != NULL)
        {
            if (strcmp(ipl->tk->str, str) == 0)
            {
                return true;
            }
            ipl = ipl->next;
        }
    }
    return false;
}

bool compareForArrayMatch(SymTablePointer *ptr1, SymTablePointer *ptr2, int line)
{

    if (!ptr1->typeIfArray.high_ && !ptr2->typeIfArray.low_)
    {
        if (!(ptr1->typeIfArray.low == ptr2->typeIfArray.low && ptr1->typeIfArray.high == ptr2->typeIfArray.high && ptr1->typeIfArray.type == ptr2->typeIfArray.type))
        {
            printf("Line %d: Error: array type mismatch\n", line);
            return true;
        }
    }
    else
    {
        // how to check here
        if (ptr1->typeIfArray.high_ && ptr2->typeIfArray.high_ && ptr1->typeIfArray.isNegHigh && ptr2->typeIfArray.isNegHigh && strcmp(ptr1->typeIfArray.highLexeme, ptr2->typeIfArray.highLexeme) == 0)
        {
        }
        else
        {
            printf("Line %d: Error: array type mismatch\n", line);
            return true;
        }
        if (ptr1->typeIfArray.low_ && ptr2->typeIfArray.low_ && ptr1->typeIfArray.isNegLow && ptr2->typeIfArray.isNegLow && strcmp(ptr1->typeIfArray.lowLexeme, ptr2->typeIfArray.lowLexeme) == 0)
        {
        }
        else
        {
            printf("Line %d: Error: array type mismatch\n", line);
            return true;
        }
    }
    return false;
}

void checkTypes(astNode *stmts, SymTablePointer *symTable)
{
    getAttributeType(stmts->leftChild, symTable);
    getAttributeType(stmts->leftChild->nextSibling, symTable);
    if (stmts->leftChild->type == TYPE_MISSING || stmts->leftChild->nextSibling->type == TYPE_MISSING)
    {
        // SKIP THIS
        stmts = stmts->nextSibling;
        return;
    }
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
        printf("Line %d: Error: Invalid operands for operation\n", line);
        semanticRulesPassed = false;
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
        printf("Line %d: Error: type mismatch, cannot assign %s type value to %s\n", line, EnumToTypeString(stmts->leftChild->nextSibling->type), EnumToTypeString(stmts->leftChild->type));
        semanticRulesPassed = false;
    }
    else if ((stmts->leftChild->type == TYPE_ARR_BOOL || stmts->leftChild->type == TYPE_ARR_INT || stmts->leftChild->type == TYPE_ARR_REAL) && (stmts->leftChild->nextSibling->type == TYPE_ARR_BOOL || stmts->leftChild->nextSibling->type == TYPE_ARR_INT || stmts->leftChild->nextSibling->type == TYPE_ARR_REAL))
    {
        SymTablePointer *ptr1 = getFromAnySymTable(symTable, stmts->leftChild->tk->str);
        SymTablePointer *ptr2 = getFromAnySymTable(symTable, stmts->leftChild->nextSibling->tk->str);
        compareForArrayMatch(ptr1, ptr2, stmts->leftChild->tk->line_num);
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
                    if (module->typeST != SCOPEST && inInputParameters(module, idList->tk->str))
                    {
                        SymTablePointer *pointer11 = initSymTablePointer();
                        pointer11->str = idList->tk->str;
                        populateTypeInformation(pointer11, stmts->leftChild->nextSibling, module);
                        pointer11->offset = getOffset(pointer11, offset);
                        pointer11->str = idList->tk->str;
                        pointer11->tk = idList->tk;
                        insertSymTable(ht, pointer11);
                    }
                    else
                    {
                        printf("Line %d: Error: Redeclaration of variable %s\n", idList->tk->line_num, idList->tk->str);
                        semanticallyCorrect = false;
                    }
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
        case AST_PRINT:
        {
            if (stmts->leftChild->label == AST_ID)
            {
                if (!existsInAnySymTable(module, stmts->leftChild->tk->str))
                {
                    printf("Line %d: Error: variable %s is not declared\n", stmts->leftChild->tk->line_num, stmts->leftChild->tk->str);
                    semanticallyCorrect = false;
                }
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_GETVALUE:
        {
            if (stmts->leftChild->label == AST_ID)
            {
                if (!existsInAnySymTable(module, stmts->leftChild->tk->str))
                {
                    printf("Line %d: Error: variable %s is not declared\n", stmts->leftChild->tk->line_num, stmts->leftChild->tk->str);
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
                printf("Line %d: Error: variable %s is not declared\n", idNode->tk->line_num, idNode->tk->str);
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
                // int offset1 = 0;
                SymTablePointer *casePtr = initSymTablePointer();
                casePtr->typeST = SCOPEST;
                hashtable *caseHashtable = initHashtableForSymTable();
                casePtr->corrHashtable = caseHashtable;
                casePtr->parentHashTable = newPointer;
                cases->symTable = casePtr;
                casePtr->line_start = cases->line_start;
                casePtr->line_end = cases->line_end;
                newPointer->childScopeTable = append_scope_pointer(newPointer->childScopeTable, casePtr);
                populateStmtsSymTable(casePtr, cases->leftChild->nextSibling->leftChild, offset);
                cases = cases->nextSibling;
            }

            astNode *def = stmts->leftChild->nextSibling->nextSibling; // default node
            if (def->leftChild != NULL)
            {
                SymTablePointer *defPtr = initSymTablePointer();
                defPtr->typeST = SCOPEST;
                hashtable *caseHashtable = initHashtableForSymTable();
                defPtr->corrHashtable = caseHashtable;
                defPtr->parentHashTable = newPointer;
                def->symTable = defPtr;
                defPtr->line_start = def->line_start;
                defPtr->line_end = def->line_end;
                newPointer->childScopeTable = append_scope_pointer(newPointer->childScopeTable, defPtr);
                populateStmtsSymTable(defPtr, def->leftChild->leftChild, offset);
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
                            printf("Line %d: Error: Variable %s is not declared\n", idList->tk->line_num, idList->tk->str);
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
                        printf("Line %d: Error: module %s has not been defined\n", children->tk->line_num, children->tk->str);
                        semanticallyCorrect = false;
                    }
                    else
                    {
                        SymTablePointer *ptr = getFromSymTable(globalSymbolTable->corrHashtable, children->tk->str);
                        ptr->called = true;
                        if (ptr->isDeclared && ptr->isDefined && !ptr->called)
                        {
                            printf("Line %d: Error: module %s's call has both a declaration and definition before this\n", children->tk->line_num, children->tk->str);
                        }
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
                                printf("Line %d: Error: variable %s has not been declared\n", idNode->tk->line_num, idNode->tk->str);
                                semanticallyCorrect = false;
                            }
                        }
                        else
                        {
                            astNode *idNode = idList;
                            if (!existsInAnySymTable(module, idNode->tk->str))
                            {
                                printf("Line %d: Error: variable %s has not been declared\n", idNode->tk->line_num, idNode->tk->str);
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
            checkTypes(stmts, module);
            stmts = stmts->nextSibling;
            break;
        }
        case AST_FOR:
        {
            SymTablePointer *forScope = initSymTablePointer();
            module->childScopeTable = append_scope_pointer(module->childScopeTable, forScope);
            forScope->parentHashTable = module;
            forScope->typeST = SCOPEST;
            forScope->line_start = stmts->line_start;
            forScope->line_end = stmts->line_end;
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
            // int offset1 = 0;
            populateStmtsSymTable(forScope, forStmts, offset);

            stmts = stmts->nextSibling;
            break;
        }
        case AST_WHILE:
        {
            SymTablePointer *whileScope = initSymTablePointer();
            module->childScopeTable = append_scope_pointer(module->childScopeTable, whileScope);
            whileScope->parentHashTable = module;
            whileScope->typeST = SCOPEST;
            whileScope->line_start = stmts->line_start;
            whileScope->line_end = stmts->line_end;
            stmts->symTable = whileScope;
            hashtable *whileScopeHashTable = initHashtableForSymTable();
            whileScope->corrHashtable = whileScopeHashTable;
            astNode *whileStmts = stmts->leftChild->nextSibling->leftChild;
            traverse_ast(stmts->leftChild, module);
            // int offset1 = 0;
            populateStmtsSymTable(whileScope, whileStmts, offset);
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

int staticOrDyn(SymTablePointer *ptr)
{
    if (ptr->typeIfArray.high_ || ptr->typeIfArray.low_)
    {
        return 1;
    }
    return 0;
}

void printSymbolTable(SymTablePointer *symTable)
{
    if (symTable == NULL)
        return;
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (symTable->corrHashtable->table[i]->bucket_ptr != NULL)
        {
            SymTablePointer *ptr = symTable->corrHashtable->table[i]->bucket_ptr;
            int line_start = symTable->line_start;
            int line_end = symTable->line_end;
            while (ptr != NULL)
            {
                // char *moduleName = ptr->str;
                hashtable *ht = ptr->corrHashtable;
                if (ptr->typeST == MODULEST)
                {
                    line_start = ptr->line_start;
                    line_end = ptr->line_end;
                    list *inp = ptr->input_para_list;
                    while (inp != NULL)
                    {
                        if (inp->isArray)
                        {
                            if (staticOrDyn(getPointerFromList(inp)) == 1)
                            {
                                printf("%10s %20s [%d-%d] %10s %10s %10s %10s %10s %10d\n", inp->tk->str, ptr->str, line_start, line_end, EnumToTypeString(inp->typeIfArray.type), "yes", "dynamic", "**", "**", inp->offset);
                            }
                            else
                            {
                                printf("%10s %20s [%d-%d] %10s %10s %10s [%3d,%3d] %10d %10d\n", inp->tk->str, ptr->str, line_start, line_end, EnumToTypeString(inp->typeIfArray.type), "yes", "static", inp->typeIfArray.low, inp->typeIfArray.high, inp->width, inp->offset);
                            }
                        }
                        else
                        {
                            printf("%10s %20s [%d-%d] %10s %10s %10s %10s %10d %10d\n", inp->tk->str, ptr->str, line_start, line_end, EnumToTypeString(inp->typeIfNotArray), "no", "**", "**", inp->width, inp->offset);
                        }
                        inp = inp->next;
                    }
                    list *opl = ptr->output_para_list;
                    while (opl != NULL)
                    {

                        printf("%10s %20s [%d-%d] %10s %10s %10s %10s %10d %10d\n", opl->tk->str, ptr->str, line_start, line_end, EnumToTypeString(opl->typeIfNotArray), "no", "**", "**", opl->width, opl->offset);
                        opl = opl->next;
                    }
                    printSymbolTable(ptr);
                }
                else
                {
                    SymTablePointer *ptr1 = symTable;
                    while (ptr1 != NULL && ptr1->typeST != MODULEST)
                    {
                        ptr1 = ptr1->parentHashTable;
                    }
                    if (ptr->isArray)
                    {
                        if (staticOrDyn(ptr) == 1)
                        {
                            printf("%10s %20s [%d-%d] %10s %10s %10s %10s %10s %10d\n", ptr->str, ptr1->str, line_start, line_end, EnumToTypeString(ptr->typeIfArray.type), "yes", "dynamic", "**", "**", ptr->offset);
                        }
                        else
                        {
                            printf("%10s %20s [%d-%d] %10s %10s %10s [%3d,%3d] %10d %10d\n", ptr->str, ptr1->str, line_start, line_end, EnumToTypeString(ptr->typeIfArray.type), "yes", "static", ptr->typeIfArray.low, ptr->typeIfArray.high, ptr->width, ptr->offset);
                        }
                    }
                    else
                    {
                        printf("%10s %20s [%d-%d] %10s %10s %10s %10s %10d %10d\n", ptr->str, ptr1->str, line_start, line_end, EnumToTypeString(ptr->typeIfNotArray), "no", "**", "**", ptr->width, ptr->offset);
                    }
                }
                ptr = ptr->next;
            }
        }
    }
    if (symTable->childScopeTable != NULL)
    {
        printSymbolTable(symTable->childScopeTable);
    }
    if (symTable->siblingHashTable != NULL)
    {
        printSymbolTable(symTable->siblingHashTable);
    }
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
                    pointer->isDeclared = true;
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
                astNode *modDef = corrID->nextSibling->nextSibling->nextSibling;
                pointer->line_start = modDef->line_start;
                pointer->line_end = modDef->line_end;
                pointer->str = corrID->tk->str;
                if (existsInSymTable(globalST, pointer->str) && !getFromSymTable(globalST, pointer->str)->isAwaited)
                {
                    printf("Line %d: Error: Module with name %s was defined previously!\n", corrID->tk->line_num, corrID->tk->str);
                    semanticallyCorrect = false;
                }
                else
                {
                    if (existsInSymTable(globalST, pointer->str))
                    {
                        SymTablePointer *temp = getFromSymTable(globalST, pointer->str);
                        temp->isDefined = true;
                        if (temp->isDeclared && temp->isDefined && !temp->called)
                        {
                            printf("Line %d: Error: module %s has both definition and declaration before its call\n", corrID->tk->line_num, pointer->str);
                        }
                        temp->isAwaited = false;
                        pointer = temp;
                        pointer->line_start = modDef->line_start;
                        pointer->line_end = modDef->line_end;
                    }
                    else
                    {
                        insertSymTable(globalST, pointer);
                    }
                    // populate type expression
                    astNode *iplNode = mdls->leftChild->nextSibling->leftChild;
                    astNode *oplNode = mdls->leftChild->nextSibling->nextSibling->leftChild;
                    pointer->input_para_list = NULL;
                    pointer->output_para_list = NULL;
                    int offset1 = 0;
                    if (iplNode != NULL)
                        pointer->input_para_list = getListFromAST(iplNode, &offset1);
                    if (oplNode != NULL)
                        pointer->output_para_list = getListFromAST(oplNode, &offset1);

                    list *ptr = pointer->input_para_list;
                    if (strcmp(pointer->str, "function2") == 0)
                    {
                        while (ptr != NULL)
                        {
                            ptr = ptr->next;
                        }
                    }
                    hashtable *ht = initHashtableForSymTable();
                    pointer->corrHashtable = ht;
                    astNode *temp;
                    // int offset = 0;
                    // while (temp != NULL)
                    // {
                    //     SymTablePointer *pointer1 = initSymTablePointer();

                    //     populateTypeInformation(pointer1, temp->leftChild, pointer);
                    //     pointer1->offset = getOffset(pointer1, &offset);
                    //     pointer1->str = temp->leftChild->nextSibling->tk->str;
                    //     insertSymTable(pointer->corrHashtable, pointer1);
                    //     temp = temp->nextSibling;
                    // }
                    temp = oplNode;

                    // while (temp != NULL)
                    // {
                    //     SymTablePointer *pointer1 = initSymTablePointer();

                    //     populateTypeInformation(pointer1, temp->leftChild, pointer);
                    //     // offset here:
                    //     pointer1->offset = getOffset(pointer1, &offset1);
                    //     pointer1->str = temp->leftChild->nextSibling->tk->str;
                    //     insertSymTable(pointer->corrHashtable, pointer1);
                    //     temp = temp->nextSibling;
                    // }

                    pointer->typeST = MODULEST;
                    pointer->parentHashTable = global;
                    populateModuleSymbolTable(pointer, mdls, &offset1);

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
            pointer->line_start = root->leftChild->line_start;
            pointer->line_end = root->leftChild->line_end;
            hashtable *ht = initHashtableForSymTable();
            root->symTable = pointer;
            pointer->corrHashtable = ht;
            pointer->typeST = MODULEST;
            pointer->parentHashTable = global;
            astNode *stmts = root->leftChild->leftChild->leftChild;
            int offset1 = 0;
            populateStmtsSymTable(pointer, stmts, &offset1);
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
                    printf("Line %d: Error: module %s is never defined\n", node->tk->line_num, node->str);
                    semanticallyCorrect = false;
                }
                node = node->next;
            }
        }
    }
}

int getActModule(SymTablePointer *symTable)
{
    if (symTable == NULL)
        return 0;
    int ans = 0;
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (symTable->corrHashtable->table[i]->bucket_ptr != NULL)
        {
            SymTablePointer *ptr = symTable->corrHashtable->table[i]->bucket_ptr;
            while (ptr != NULL)
            {
                // char *moduleName = ptr->str;
                if (ptr->width != -1)
                    ans += ptr->width;
                ptr = ptr->next;
            }
        }
    }
    if (symTable->childScopeTable != NULL)
    {
        ans += getActModule(symTable->childScopeTable);
    }
    if (symTable->siblingHashTable != NULL)
    {
        ans += getActModule(symTable->siblingHashTable);
    }
    return ans;
}

void getActivationRecords()
{
    hashtable *ht = globalSymbolTable->corrHashtable;
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (ht->table[i]->bucket_ptr != NULL)
        {
            SymTablePointer *ptr = ht->table[i]->bucket_ptr;
            while (ptr != NULL)
            {
                ptr->activationRecordSize = getActModule(ptr);
                list *inp = ptr->input_para_list;
                while (inp != NULL)
                {
                    ptr->activationRecordSize += inp->width;
                    inp = inp->next;
                }
                inp = ptr->output_para_list;
                while (inp != NULL)
                {
                    ptr->activationRecordSize += inp->width;
                    inp = inp->next;
                }
                ptr = ptr->next;
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
//     getActivationRecords();
//     printf("%d\n", getFromSymTable(globalSymbolTable->corrHashtable, "one")->activationRecordSize);
//     printSymbolTable(globalSymbolTable);
// }
