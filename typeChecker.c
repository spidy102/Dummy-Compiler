#include <stdio.h>
#include <string.h>
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
#include "typeCheckerDef.h"

bool semanticRulesPassed = true;

void checkBounds(astNode *arr_ele, SymTablePointer *ptr, SymTablePointer *parent, bool printErrors);

void getAttributeType(astNode *node, SymTablePointer *symTable, bool printErrors)
{
    switch (node->label)
    {
    case AST_PLUS:
    case AST_MINUS:
    case AST_MUL:
    {
        getAttributeType(node->leftChild, symTable, printErrors);
        getAttributeType(node->leftChild->nextSibling, symTable, printErrors);
        if (node->leftChild->type == TYPE_INTEGER && node->leftChild->nextSibling->type == TYPE_INTEGER)
        {
            node->type = TYPE_INTEGER;
        }
        else if (node->leftChild->type == TYPE_REAL && node->leftChild->nextSibling->type == TYPE_REAL)
        {
            node->type = TYPE_REAL;
        }
        else if (node->leftChild->type == TYPE_MISSING || node->leftChild->nextSibling->type == TYPE_MISSING)
        {
            node->type = TYPE_MISSING;
        }
        else
        {
            node->type = TYPE_ERROR;
        }
        break;
    }
    case AST_SIGNED:
    {
        getAttributeType(node->leftChild->nextSibling, symTable, printErrors);
        node->type = node->leftChild->nextSibling->type;
        break;
    }
    case AST_DIV:
    {

        getAttributeType(node->leftChild, symTable, printErrors);
        getAttributeType(node->leftChild->nextSibling, symTable, printErrors);
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
        else if (node->leftChild->type == TYPE_MISSING || node->leftChild->nextSibling->type == TYPE_MISSING)
        {
            node->type = TYPE_MISSING;
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
        getAttributeType(node->leftChild, symTable, printErrors);
        getAttributeType(node->leftChild->nextSibling, symTable, printErrors);
        if (node->leftChild->type == TYPE_INTEGER && node->leftChild->nextSibling->type == TYPE_INTEGER)
        {
            node->type = TYPE_BOOLEAN;
        }
        else if (node->leftChild->type == TYPE_REAL && node->leftChild->nextSibling->type == TYPE_REAL)
        {
            node->type = TYPE_BOOLEAN;
        }
        else if (node->leftChild->type == TYPE_MISSING || node->leftChild->nextSibling->type == TYPE_MISSING)
        {
            node->type = TYPE_MISSING;
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
        getAttributeType(node->leftChild, symTable, printErrors);
        getAttributeType(node->leftChild->nextSibling, symTable, printErrors);
        if (node->leftChild->type == TYPE_BOOLEAN && node->leftChild->nextSibling->type == TYPE_BOOLEAN)
        {
            node->type = TYPE_BOOLEAN;
        }
        else if (node->leftChild->type == TYPE_MISSING || node->leftChild->nextSibling->type == TYPE_MISSING)
        {
            node->type = TYPE_MISSING;
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
        if (pointer == NULL)
        {
            // skip this construct entirely
            node->type = TYPE_MISSING;
            break;
        }
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
        if (pointer == NULL)
        {
            node->type = TYPE_MISSING;
            break;
        }
        // check bounds;
        checkBounds(node, pointer, symTable, printErrors);
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
    else if (type == TYPE_ARR_BOOL)
        return "array of boolean";
    else if (type == TYPE_ARR_INT)
        return "array of integer";
    else if (type == TYPE_ARR_REAL)
        return "array of real";
    return "boolean";
}

void checkBounds(astNode *arr_ele, SymTablePointer *ptr, SymTablePointer *parent, bool printErrors)
{
    // printf("str:%s\n\n", ptr->str);

    astNode *bound = arr_ele->leftChild->nextSibling;
    if (bound->label == AST_NUM)
    {
        // check bounds
        if (!ptr->typeIfArray.high_ && !ptr->typeIfArray.low_)
        {
            if (!(bound->tk->integer >= ptr->typeIfArray.low && bound->tk->integer <= ptr->typeIfArray.high))
            {
                if (printErrors)
                {
                    printf("Line %d: Error: array index out of bounds for array variable %s\n", bound->tk->line_num, arr_ele->leftChild->tk->str);
                }
                semanticRulesPassed = false;
                // type_error?
            }
        }
        bound->type = TYPE_INTEGER;
        arr_ele->type = ptr->typeIfArray.type;
    }
    else if (bound->label == AST_RNUM)
    {
        if (printErrors)
        {
            printf("Line %d: Error: array index is expected to be integral, received real\n", bound->tk->line_num);
        }
        semanticRulesPassed = false;
        arr_ele->type = TYPE_ERROR;
    }
    else if (bound->label == AST_SIGNED)
    {
        getAttributeType(bound->leftChild->nextSibling, parent, printErrors);
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
                        if (printErrors)
                        {
                            printf("Line %d: Error: array index out of bounds for array variable %s\n", bound->leftChild->nextSibling->tk->line_num, arr_ele->leftChild->tk->str);
                        }
                        semanticRulesPassed = false;
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
                        if (printErrors)
                        {
                            printf("Line %d: Error: array index out of bounds for array variable %s\n", bound->leftChild->nextSibling->tk->line_num, arr_ele->leftChild->tk->str);
                        }
                        semanticRulesPassed = false;
                    }
                }
            }
            arr_ele->type = ptr->typeIfArray.type;
        }
    }
    else
    {
        getAttributeType(bound, parent, printErrors);

        if (bound->type != TYPE_INTEGER)
        {

            if (printErrors)
            {
                printf("Line %d: Error: array index is expected to be of integer\n", arr_ele->leftChild->tk->line_num);
            }

            semanticRulesPassed = false;
            arr_ele->type = TYPE_MISSING;
        }
        else
        {
            arr_ele->type = ptr->typeIfArray.type;
        }
        // delay bound checking until runtime
    }
}

bool compareForArrayMatch1(SymTablePointer *ptr1, SymTablePointer *ptr2, int line)
{

    if (!ptr1->typeIfArray.high_ && !ptr2->typeIfArray.low_)
    {
        if (!(ptr1->typeIfArray.low == ptr2->typeIfArray.low && ptr1->typeIfArray.high == ptr2->typeIfArray.high && ptr1->typeIfArray.type == ptr2->typeIfArray.type))
        {
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
            return true;
        }
        if (ptr1->typeIfArray.low_ && ptr2->typeIfArray.low_ && ptr1->typeIfArray.isNegLow && ptr2->typeIfArray.isNegLow && strcmp(ptr1->typeIfArray.lowLexeme, ptr2->typeIfArray.lowLexeme) == 0)
        {
        }
        else
        {
            return true;
        }
    }
    return false;
}

void compareActualAndFormalParams(list *ipl, astNode *actual_params, SymTablePointer *symTable, bool printErrors)
{
    int i = 0;
    astNode *temp = actual_params;
    int line;
    if (temp->label == AST_ARRAY_ELEMENT)
    {
        line = temp->leftChild->tk->line_num;
    }
    else
    {
        line = temp->tk->line_num;
    }
    while (actual_params != NULL && ipl != NULL)
    {
        switch (actual_params->label)
        {
        case AST_NUM:
        case AST_RNUM:
        case AST_BOOL:
        case AST_ID:
        case AST_ARRAY_ELEMENT:
        {
            getAttributeType(actual_params, symTable, printErrors);
            if (ipl->isArray)
            {
                if (actual_params->type != TYPE_ARR_BOOL && actual_params->type != TYPE_ARR_REAL && actual_params->type != TYPE_ARR_INT)
                {
                    if (actual_params->type != TYPE_MISSING)
                    {
                        if (printErrors)
                        {
                            printf("Line %d: Error: type mismatch in parameter %d, received %s, required array\n", actual_params->tk->line_num, i + 1, EnumToTypeString(actual_params->type));
                        }
                        semanticRulesPassed = false;
                    }
                }
                else
                {
                    if (actual_params->type != TYPE_MISSING)
                    {
                        if (compareForArrayMatch1(getPointerFromList(ipl), getFromAnySymTable(symTable, actual_params->tk->str), actual_params->tk->line_num))
                        {
                            if (printErrors)
                            {
                                printf("Line %d: Error: type mismatch in parameter %d, received %s, required %s\n", actual_params->tk->line_num, i + 1, EnumToTypeString(actual_params->type), ipl->typeIfArray.type == TYPE_ARR_INT ? "array of integer" : (ipl->typeIfArray.type == TYPE_ARR_BOOL ? "array of boolean" : "array of real"));
                            }
                            semanticRulesPassed = false;
                        }
                    }
                }
            }
            else if (ipl->typeIfNotArray != actual_params->type)
            {
                if (ipl->typeIfNotArray != TYPE_MISSING && actual_params->type != TYPE_MISSING)
                {
                    if (printErrors)
                    {
                        printf("Line %d: Error: type mismatch in parameter %d, received %s, required %s\n", actual_params->tk->line_num, i + 1, EnumToTypeString(actual_params->type), EnumToTypeString(ipl->typeIfNotArray));
                    }
                    semanticRulesPassed = false;
                }
            }
            actual_params = actual_params->nextSibling;
            ipl = ipl->next;
            break;
        }
        }
        i++;
    }
    if (ipl != NULL)
    {
        if (printErrors)
        {
            printf("Line %d: Error: Received too few input arguments for function call\n", line);
        }
        semanticRulesPassed = false;
    }
    if (actual_params != NULL)
    {
        if (printErrors)
        {
            printf("Line %d: Error: received too many input arguments for function call\n", line);
        }
        semanticRulesPassed = false;
    }
}

void compareRetParams(list *opl, astNode *retParams, SymTablePointer *symTable, int line, bool printErrors)
{
    int i = 0;
    while (retParams != NULL && opl != NULL)
    {
        switch (retParams->label)
        {
        case AST_NUM:
        case AST_RNUM:
        case AST_BOOL:
        case AST_ID:
        case AST_ARRAY_ELEMENT:
        {

            getAttributeType(retParams, symTable, printErrors);
            if (opl->isArray)
            {
                if (retParams->type != TYPE_ARR_BOOL && retParams->type != TYPE_ARR_REAL && retParams->type != TYPE_ARR_INT)
                {
                    if (retParams->type != TYPE_MISSING)
                    {
                        if (printErrors)
                        {
                            printf("Line %d: Error: type mismatch in output parameter %d, received %s, required array\n", retParams->tk->line_num, i + 1, EnumToTypeString(retParams->type));
                        }
                        semanticRulesPassed = false;
                    }
                }
            }
            else if (opl->typeIfNotArray != retParams->type)
            {
                if (opl->typeIfNotArray != TYPE_MISSING && retParams->type != TYPE_MISSING)
                {
                    semanticRulesPassed = false;
                    if (printErrors)
                    {
                        printf("Line %d: Error: type mismatch in output parameter %d, received %s, required %s\n", retParams->tk->line_num, i + 1, EnumToTypeString(retParams->type), EnumToTypeString(opl->typeIfNotArray));
                    }
                }
            }
            retParams = retParams->nextSibling;
            opl = opl->next;
            break;
        }
        }
        i++;
    }
    if (opl != NULL)
    {
        semanticRulesPassed = false;
        if (printErrors)
        {
            printf("Line %d: Error: too few output parameters for function call\n", line);
        }
    }
    if (retParams != NULL)
    {
        semanticRulesPassed = false;
        if (printErrors)
        {
            printf("Line %d: Error: too many output parameters for function call\n", line);
        }
    }
}

void checkIfLoopStmtsDoNotRedefineLoopVariable(astNode *stmts, astNode *loopVar, bool printErrors)
{
    while (stmts != NULL)
    {
        if (stmts->label == AST_ASSIGNOP)
        {
            if (stmts->leftChild->label == AST_ID && strcmp(stmts->leftChild->tk->str, loopVar->tk->str) == 0)
            {
                if (printErrors)
                {
                    printf("Line %d: Error: for loop variable assigned a value\n", stmts->leftChild->tk->line_num);
                }
                semanticRulesPassed = false;
            }
        }
        else if (stmts->label == AST_MODULE_REUSE)
        {
            astNode *optionalNode = stmts->leftChild;
            if (optionalNode->leftChild == NULL)
            {
                stmts = stmts->nextSibling;
            }
            else
            {
                astNode *idList = optionalNode->leftChild->leftChild;
                while (idList != NULL)
                {
                    if (strcmp(idList->tk->str, loopVar->tk->str) == 0)
                    {
                        if (printErrors)
                        {
                            printf("Line %d: Error: for loop variable assigned a value\n", idList->tk->line_num);
                        }
                        semanticRulesPassed = false;
                    }
                    idList = idList->nextSibling;
                }
            }
        }
        else if (stmts->label == AST_WHILE)
        {
            checkIfLoopStmtsDoNotRedefineLoopVariable(stmts->leftChild->nextSibling->leftChild, loopVar, printErrors);
        }
        else if (stmts->label == AST_SWITCH_CASE)
        {
            astNode *cases = stmts->leftChild->nextSibling->leftChild;
            while (cases != NULL)
            {
                astNode *stmts1 = cases->leftChild->nextSibling->leftChild;
                checkIfLoopStmtsDoNotRedefineLoopVariable(stmts1, loopVar, printErrors);
                cases = cases->nextSibling;
            }
            astNode *def = stmts->leftChild->nextSibling->nextSibling->leftChild->leftChild;
            checkIfLoopStmtsDoNotRedefineLoopVariable(def, loopVar, printErrors);
        }
        else if (stmts->label == AST_FOR)
        {
            checkIfLoopStmtsDoNotRedefineLoopVariable(stmts->leftChild->nextSibling->nextSibling->leftChild, loopVar, printErrors);
        }
        stmts = stmts->nextSibling;
    }
}

bool checkIfOnLHS(astNode *temp, astNode *expr)
{
    while (temp != NULL)
    {
        switch (temp->label)
        {
        case AST_ASSIGNOP:
        {
            if (temp->leftChild->label == AST_ID)
            {
                if (strcmp(expr->tk->str, temp->leftChild->tk->str) == 0)
                {
                    return true;
                }
            }
            temp = temp->nextSibling;
            break;
        }
        case AST_MODULE_REUSE:
        {
            astNode *optionalNode = temp->leftChild;
            if (optionalNode->leftChild == NULL)
            {
                temp = temp->nextSibling;
            }
            else
            {
                astNode *idList = optionalNode->leftChild->leftChild;
                while (idList != NULL)
                {
                    if (strcmp(idList->tk->str, expr->tk->str) == 0)
                    {
                        return true;
                    }
                    else
                    {
                        idList = idList->nextSibling;
                    }
                }
                temp = temp->nextSibling;
            }
            break;
        }
        case AST_WHILE:
        {
            if (checkIfOnLHS(temp->leftChild->nextSibling->leftChild, expr))
                return true;
            temp = temp->nextSibling;
            break;
        }
        case AST_SWITCH_CASE:
        {
            astNode *cases = temp->leftChild->nextSibling->leftChild;
            while (cases != NULL)
            {
                astNode *stmts = cases->leftChild->nextSibling->leftChild;
                if (checkIfOnLHS(stmts, expr))
                {
                    return true;
                }
                cases = cases->nextSibling;
            }
            astNode *def = temp->leftChild->nextSibling->nextSibling;
            if (def->leftChild != NULL)
            {
                if (checkIfOnLHS(def->leftChild->leftChild, expr))
                    return true;
            }

            temp = temp->nextSibling;
            break;
        }
        case AST_FOR:
        {
            if (checkIfOnLHS(temp->leftChild->nextSibling->nextSibling->leftChild, expr))
                return true;
            temp = temp->nextSibling;
            break;
        }
        default:
        {
            temp = temp->nextSibling;
            break;
        }
        }
    }
}

bool checkWhileLoopExpressionAssigned(astNode *stmts, astNode *expr)
{
    if (expr == NULL)
        return false;
    if (checkWhileLoopExpressionAssigned(stmts, expr->leftChild))
        return true;
    if (expr->label == AST_ID)
    {
        astNode *temp = stmts;
        if (checkIfOnLHS(temp, expr))
            return true;
    }
    if (expr->leftChild != NULL)
    {
        astNode *leftOut = expr->leftChild->nextSibling;
        while (leftOut != NULL)
        {
            if (checkWhileLoopExpressionAssigned(stmts, leftOut))
                return true;
            leftOut = leftOut->nextSibling;
        }
    }
}

void checkTypesForModule(SymTablePointer *symTable, astNode *stmts, bool printErrors)
{
    while (stmts != NULL)
    {

        switch (stmts->label)
        {
        // case AST_ASSIGNOP:
        // {
        //     getAttributeType(stmts->leftChild, symTable);
        //     getAttributeType(stmts->leftChild->nextSibling, symTable);
        //     if (stmts->leftChild->type == TYPE_MISSING || stmts->leftChild->nextSibling->type == TYPE_MISSING)
        //     {
        //         // SKIP THIS
        //         stmts = stmts->nextSibling;
        //         break;
        //     }
        //     if (stmts->leftChild->nextSibling->type == TYPE_ERROR || stmts->leftChild->type == TYPE_ERROR)
        //     {
        //         int line;
        //         if (stmts->leftChild->tk != NULL)
        //         {
        //             line = stmts->leftChild->tk->line_num;
        //         }
        //         else
        //         {
        //             line = stmts->leftChild->leftChild->tk->line_num;
        //         }
        //         printf("Error: Invalid operands for operation on line number %d\n", line);
        //         semanticRulesPassed = false;
        //     }
        //     else if (stmts->leftChild->type != stmts->leftChild->nextSibling->type)
        //     {

        //         int line;
        //         if (stmts->leftChild->tk != NULL)
        //         {
        //             line = stmts->leftChild->tk->line_num;
        //         }
        //         else
        //         {
        //             line = stmts->leftChild->leftChild->tk->line_num;
        //         }
        //         printf("Error: type mismatch at line number %d, cannot assign %s type value to %s\n", line, EnumToTypeString(stmts->leftChild->nextSibling->type), EnumToTypeString(stmts->leftChild->type));
        //         semanticRulesPassed = false;
        //     }
        //     stmts = stmts->nextSibling;
        //     break;
        // }
        case AST_FOR:
        {
            SymTablePointer *symTableInThisScope = stmts->symTable;
            astNode *stmtsNode = stmts->leftChild->nextSibling->nextSibling->leftChild;
            // check if the statements inside for loop do not redefine the loop variable
            checkIfLoopStmtsDoNotRedefineLoopVariable(stmtsNode, stmts->leftChild, printErrors);
            checkTypesForModule(symTableInThisScope, stmtsNode, printErrors);
            stmts = stmts->nextSibling;
            break;
        }
        case AST_WHILE:
        {
            // while's condition??????
            getAttributeType(stmts->leftChild, stmts->symTable, printErrors);
            if (stmts->leftChild->type != TYPE_BOOLEAN)
            {
                if (printErrors)
                {
                    printf("Line %d: Error: while's condition should only evaluate to boolean\n", stmts->tk->line_num);
                }
                semanticRulesPassed = false;
                // check this once
            }
            SymTablePointer *symTableInThisScope = stmts->symTable;
            astNode *stmtsNode = stmts->leftChild->nextSibling->leftChild;
            checkTypesForModule(symTableInThisScope, stmtsNode, printErrors);
            if (!checkWhileLoopExpressionAssigned(stmtsNode, stmts->leftChild))
            {
                if (printErrors)
                {
                    printf("Line %d: Error: while loop expression variables are not assigned any value\n", stmts->tk->line_num);
                }
                semanticRulesPassed = false;
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_MODULE_REUSE:
        {
            astNode *params = stmts->leftChild;
            SymTablePointer *ptr = getFromSymTable(globalSymbolTable->corrHashtable, params->nextSibling->tk->str);

            while (params != NULL)
            {

                switch (params->label)
                {
                case AST_ID:
                {

                    // ptr = getFromSymTable(globalSymbolTable->corrHashtable, params->tk->str);
                    SymTablePointer *currentScope = symTable;

                    while (currentScope->typeST != MODULEST)
                    {
                        currentScope = currentScope->parentHashTable;
                    }
                    if (strcmp(params->tk->str, currentScope->str) == 0)
                    {
                        if (printErrors)
                        {
                            printf("Line %d: Error: A function cannot be called recursively\n", params->tk->line_num);
                        }
                        semanticRulesPassed = false;
                    }
                    params = params->nextSibling;
                    break;
                }
                case AST_ACTUAL_PARA:
                {
                    astNode *actualParams = params->leftChild;
                    list *ipl = ptr->input_para_list;
                    compareActualAndFormalParams(ipl, actualParams, symTable, printErrors);
                    params = params->nextSibling;
                    break;
                }
                case AST_OPTIONAL:
                {
                    astNode *retParams = params->leftChild->leftChild;
                    list *opl = ptr->output_para_list;
                    compareRetParams(opl, retParams, symTable, params->tk->line_num, printErrors);
                    params = params->nextSibling;
                    break;
                }
                }
            }
            stmts = stmts->nextSibling;
            break;
        }
        case AST_SWITCH_CASE:
        {
            astNode *idNode = stmts->leftChild;
            SymTablePointer *ptr = getFromAnySymTable(symTable, idNode->tk->str);
            if (!ptr->isArray && ptr->typeIfNotArray == TYPE_INTEGER)
            {
                // need to check case conditions are integral or not
                // and default must be present
                astNode *cases = stmts->leftChild->nextSibling->leftChild;
                astNode *def = stmts->leftChild->nextSibling->nextSibling;
                if (def->leftChild == NULL)
                {
                    if (printErrors)
                    {
                        printf("Line %d: Error: Switch case statement must have a default statement associated when the identifier is integer, is absent for switch statement\n", idNode->tk->line_num);
                    }
                    semanticRulesPassed = false;
                }
                while (cases != NULL)
                {
                    astNode *caseLabel = cases->leftChild;
                    if (caseLabel->label == AST_BOOL)
                    {
                        if (printErrors)
                        {
                            printf("Line %d: Error: should have integer case labels for switch statement with integer identifier, received boolean\n", caseLabel->tk->line_num);
                        }
                        semanticRulesPassed = false;
                    }
                    cases = cases->nextSibling;
                }
            }
            else if (!ptr->isArray && ptr->typeIfNotArray == TYPE_BOOLEAN)
            {
                // need to check if case conditions are boolean
                // default should not be present
                astNode *cases = stmts->leftChild->nextSibling->leftChild;
                astNode *temp = cases;
                astNode *def = stmts->leftChild->nextSibling->nextSibling;
                if (def->leftChild != NULL)
                {
                    if (printErrors)
                    {
                        printf("Line %d: Error: Switch case statement must not have a default statement associated when the identifier is boolean\n", def->tk->line_num);
                    }
                    semanticRulesPassed = false;
                }
                while (temp != NULL)
                {
                    astNode *caseLabel = temp->leftChild;
                    if (caseLabel->label != AST_BOOL)
                    {
                        if (printErrors)
                        {
                            printf("Line %d: Error: case statement should have a boolean identifier\n", caseLabel->tk->line_num);
                        }
                        semanticRulesPassed = false;
                    }
                    temp = temp->nextSibling;
                }
            }
            else
            {
                //////what to do here?
                semanticRulesPassed = false;
                if (printErrors)
                {
                    printf("Line %d: Error: switch case statement is expected to have only an integer or boolean typed identifier, got %s \n", idNode->tk->line_num, (ptr->isArray ? "array" : "real"));
                }
            }
            // check types???

            astNode *cases = stmts->leftChild->nextSibling->leftChild;
            while (cases != NULL)
            {
                checkTypesForModule(cases->symTable, cases->leftChild->nextSibling->leftChild, printErrors);
                cases = cases->nextSibling;
            }
            astNode *def = stmts->leftChild->nextSibling->nextSibling;
            if (def->leftChild != NULL)
                checkTypesForModule(def->symTable, def->leftChild->leftChild, printErrors);
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
bool outputParamsCheckHelper(astNode *temp, list *opl)
{
    bool isPresent = false;
    while (temp != NULL)
    {

        if (temp->label == AST_ASSIGNOP)
        {

            astNode *lvalue = temp->leftChild;

            if (lvalue->label != AST_ID)
            {
                temp = temp->nextSibling;
            }
            else
            {
                if (strcmp(lvalue->tk->str, opl->tk->str) == 0)
                {
                    return true;
                }
                else
                {
                    temp = temp->nextSibling;
                }
            }
        }
        else if (temp->label == AST_MODULE_REUSE)
        {

            astNode *optionalNode = temp->leftChild;
            if (optionalNode->leftChild == NULL)
            {
                temp = temp->nextSibling;
            }
            else
            {
                astNode *idList = optionalNode->leftChild->leftChild;
                while (idList != NULL)
                {
                    if (strcmp(idList->tk->str, opl->tk->str) == 0)
                    {
                        return true;
                    }
                    else
                    {
                        idList = idList->nextSibling;
                    }
                }
                temp = temp->nextSibling;
            }
        }
        else if (temp->label == AST_FOR)
        {
            astNode *forStmts = temp->leftChild->nextSibling->nextSibling->leftChild;
            if (outputParamsCheckHelper(forStmts, opl))
                return true;
            temp = temp->nextSibling;
        }
        else if (temp->label == AST_WHILE)
        {
            astNode *whileStmts = temp->leftChild->nextSibling->leftChild;
            if (outputParamsCheckHelper(whileStmts, opl))
                return true;
            temp = temp->nextSibling;
        }
        else if (temp->label == AST_SWITCH_CASE)
        {
            astNode *cases = temp->leftChild->nextSibling->leftChild;
            while (cases != NULL)
            {
                if (outputParamsCheckHelper(cases->leftChild->nextSibling->leftChild, opl))
                    return true;
                cases = cases->nextSibling;
            }
            astNode *def = temp->leftChild->nextSibling->nextSibling;
            if (def->leftChild != NULL)
            {
                if (outputParamsCheckHelper(def->leftChild->leftChild, opl))
                    return true;
            }
            temp = temp->nextSibling;
        }
        else
        {
            temp = temp->nextSibling;
        }
    }
    return false;
}

void checkIfOutputParametersAreAssigned(astNode *stmts, SymTablePointer *module, bool printErrors)
{

    list *opl = module->output_para_list;

    astNode *temp = stmts;

    while (opl != NULL)
    {

        bool isPresent = outputParamsCheckHelper(temp, opl);
        if (!isPresent)
        {
            semanticRulesPassed = false;
            if (printErrors)
            {
                printf("Line %d: Error: In module %s output parameter %s is never assigned a value\n", module->line_end, module->str, opl->tk->str);
            }
        }
        isPresent = false;
        temp = stmts;
        opl = opl->next;
    }
}

void typeCheck(astNode *root, bool printErrors)
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
                if (symTableForCurrentModule == NULL)
                {
                    separateModules = separateModules->nextSibling;
                    continue;
                }
                astNode *stmtsForCurrentModule = separateModules->leftChild->nextSibling->nextSibling->nextSibling->leftChild;
                if (stmtsForCurrentModule != NULL)
                {
                    checkTypesForModule(symTableForCurrentModule, stmtsForCurrentModule->leftChild, printErrors);
                    checkIfOutputParametersAreAssigned(stmtsForCurrentModule->leftChild, separateModules->symTable, printErrors);
                }

                separateModules = separateModules->nextSibling;
            }
            modules = modules->nextSibling;

            break;
        }
        case AST_DRIVERMODULE:
        {
            checkTypesForModule(getFromSymTable(globalSymbolTable->corrHashtable, "driver"), modules->leftChild->leftChild->leftChild, printErrors);
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

// int main()
// {
//     globalSymbolTable = initSymTablePointer();
//     globalSymbolTable->typeST = GLOBALST;
//     globalSymbolTable->parentHashTable = NULL;
//     hashtable *ht1 = initHashtableForSymTable();
//     globalSymbolTable->corrHashtable = ht1;
//     FILE *fp = fopen("random4.txt", "r");
//     twinbuffer *tb = twinbuffer_init(fp, 256);
//     fill_grammar(fopen("Grammar.txt", "r"));
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
//     populateParseTable();
//     treenode *root = parseInputSourceCode(fp, tb, ht);

//     astNode *astRoot = constructAST(root);
//     // inorder_ast(astRoot);
//     populateGlobalSymbolTable(globalSymbolTable, astRoot, 0, true);
//     typeCheck(astRoot, true);
// }