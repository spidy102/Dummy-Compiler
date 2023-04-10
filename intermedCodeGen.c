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

int labelCount = 0;
int tempCount = 0;

quadruple *globalHead = NULL;

int newTemp()
{
    return ++tempCount;
}

int newLabel()
{
    return ++labelCount;
}

quadruple *stmtsCodeGen(astNode *stmts, SymTablePointer *symTable);

quadruple *initQuadruple()
{
    quadruple *qp = malloc(sizeof(quadruple));
    qp->next = NULL;
    memset(qp->operand1, '\0', sizeof(qp->operand1));
    memset(qp->operand2, '\0', sizeof(qp->operand2));
    memset(qp->resultant, '\0', sizeof(qp->resultant));
    qp->op = -1;
}

// void getAttributeCodeExpressions(astNode *node, SymTablePointer *symTable)
// {
//     switch (node->label)
//     {
//     case AST_PLUS:
//     case AST_MINUS:
//     case AST_DIV:
//     case AST_MUL:
//     {
//         int temp = newTemp();
//         snprintf(node->name, "0temp%d", temp);
//         // symbol table??
//         getAttributeCodeExpressions(node->leftChild, symTable);
//         getAttributeCodeExpressions(node->leftChild->nextSibling, symTable);
//         // append the codes together
//         node->leftChild->code->next = node->leftChild->nextSibling->code;
//         quadruple *qp = initQuadruple();
//         strcpy(qp->operand1, node->leftChild->name);
//         strcpy(qp->operand2, node->leftChild->nextSibling->name);
//         strcpy(qp->resultant, node->name);
//         node->leftChild->nextSibling->code->next = qp;
//         qp->next = NULL;
//         if (node->label == AST_PLUS)
//         {
//             qp->op = OP_PLUS;
//         }
//         else if (node->label == AST_MINUS)
//         {
//             qp->op = OP_MINUS;
//         }
//         else if (node->label == AST_MUL)
//         {
//             qp->op = OP_MUL;
//         }
//         else
//         {
//             qp->op = OP_DIV;
//         }
//         return;
//     }
//     case AST_ID:
//     {
//         strcpy(node->name, node->tk->str);
//         return;
//     }
//     case AST_SIGNED:
//     {
//         getAttributeCodeExpressions(node->leftChild->nextSibling);
//         if (node->leftChild->tk->token == MINUS)
//         {
//             int temp = newTemp();
//             snprintf(node->name, "0temp%d", temp);
//             quadruple *qp = initQuadruple();
//             qp->op = U_MINUS;
//             strcpy(qp->operand1, node->leftChild->nextSibling->name);
//             strcpy(qp->resultant, node->name);
//             node->leftChild->nextSibling->code->next = qp;
//             qp->next = NULL;
//             node->code = node->leftChild->nextSibling->code;
//         }
//         else
//         {
//             strcpy(node->name, node->leftChild->nextSibling->name);
//             node->code = node->leftChild->nextSibling->code;
//         }
//         return;
//     }
//     case AST_NUM:
//     {
//         snprintf(node->name, "%d", node->tk->integer);
//         return;
//     }
//     case AST_RNUM:
//     {
//         snprintf(node->name, "%ld", node->tk->rnum);
//         return;
//     }
//     case AST_BOOL:
//     {
//         sprintf(node->name, "%s", node->tk->str);
//         return;
//     }
//     }
// }

quadruple *appendAtEnd(quadruple *head, quadruple *append)
{
    if (head == NULL)
    {
        return append;
    }
    quadruple *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = append;
    return head;
}

SymTablePointer *getModulesSymTable(SymTablePointer *symTable)
{
    while (symTable->typeST != MODULEST)
    {
        symTable = symTable->parentHashTable;
    }
    return symTable;
}

int updateOffsets(char *str, SymTablePointer *symTable, types type)
{
    SymTablePointer *ptrnewT = initSymTablePointer();
    strcpy(ptrnewT->str, str);
    SymTablePointer *modulesSymTable = getModulesSymTable(symTable);
    int offset = modulesSymTable->activationRecordSize;
    ptrnewT->offset = offset;
    insertSymTable(symTable->corrHashtable, ptrnewT);
    if (type == TYPE_INTEGER)
    {
        ptrnewT->width = INT_WIDTH;
    }
    else if (type == TYPE_BOOLEAN)
    {
        ptrnewT->width = BOOL_WIDTH;
    }
    else if (type == TYPE_REAL)
    {
        ptrnewT->width = REAL_WIDTH;
    }
    modulesSymTable->activationRecordSize += ptrnewT->width;
    return ptrnewT->offset;
}

void getExpressionsCode(astNode *expr, SymTablePointer *symTable)
{
    switch (expr->label)
    {
    case AST_OR:
    {
        astNode *b1 = expr->leftChild;
        astNode *b2 = expr->leftChild->nextSibling;
        b1->trueCase = expr->trueCase;
        b1->falseCase = newLabel();
        b2->trueCase = expr->trueCase;
        b2->falseCase = expr->falseCase;
        getExpressionsCode(b1);
        getExpressionsCode(b2);
        expr->code = b1->code;
        quadruple *qp = initQuadruple();
        qp->op = LABEL;
        sprintf(qp->operand1, "label%d", b1->falseCase);
        expr->code = appendAtEnd(expr->code, qp);
        qp->next = b2->code;
        return;
    }
    case AST_AND:
    {
        astNode *b1 = expr->leftChild;
        astNode *b2 = b1->nextSibling;
        b1->trueCase = newLabel();
        b1->falseCase = expr->falseCase;
        b2->trueCase = expr->trueCase;
        b2->falseCase = expr->falseCase;
        getExpressionsCode(b1);
        getExpressionsCode(b2);
        expr->code = b1->code;
        quadruple *qp = initQuadruple();
        qp->op = LABEL;
        sprintf(qp->operand1, "label%d", b1->trueCase);
        expr->code = appendAtEnd(expr->code, qp);
        qp->next = b2->code;
        return;
    }
    case AST_GE:
    case AST_LE:
    case AST_LT:
    case AST_GT:
    case AST_NE:
    case AST_EQ:
    {
        astNode *e1 = expr->leftChild;
        astNode *e2 = e1->nextSibling;
        getExpressionsCode(expr->leftChild);
        getExpressionsCode(expr->leftChild->nextSibling);
        expr->code = appendAtEnd(expr->code, expr->leftChild->code);
        expr->code = appendAtEnd(expr->code, expr->leftChild->nextSibling->code);
        int temp = newTemp();
        quadruple *qp = initQuadruple();
        sprintf(qp->resultant, "temp%d", temp);
        sprintf(qp->operand1, "%s", e1->name);
        sprintf(qp->operand2, "%s", e2->name);
        if (expr->label == AST_EQ)
        {
            qp->op = OP_EQ;
        }
        else if (expr->label == AST_NE)
        {
            qp->op = OP_NE;
        }
        else if (expr->label == AST_GE)
        {
            qp->op = OP_GE;
        }
        else if (expr->label == AST_GT)
        {
            qp->op = OP_GT;
        }
        else if (expr->label == AST_LE)
        {
            qp->op = OP_LE;
        }
        else if (expr->label == AST_LT)
        {
            qp->op = OP_LT;
        }
        expr->code = appendAtEnd(expr->code, qp);
        // e2->code->next = qp;
        quadruple *qp1 = initQuadruple();
        qp1->op = JUMPIFTRUE;
        sprintf(qp1->operand1, "temp%d", temp);
        sprintf(qp1->operand2, "label%d", expr->trueCase);
        // qp->next = qp1;
        printf("hemlo??%d\n", qp1->op);

        expr->code = appendAtEnd(expr->code, qp1);
        quadruple *qp2 = initQuadruple();
        qp2->op = JUMP;
        sprintf(qp2->operand1, "label%d", expr->falseCase);
        // qp1->next = qp2;
        expr->code = appendAtEnd(expr->code, qp2);
        return;
    }
    case AST_BOOL:
    {
        expr->code = initQuadruple();
        expr->code->op = JUMP;
        if (expr->tk->token == TRUE)
        {
            sprintf(expr->code->operand1, "label%d", expr->trueCase);
        }
        else
        {
            sprintf(expr->code->operand1, "label%d", expr->falseCase);
        }
        return;
    }
    case AST_ID:
    {
        strcpy(expr->name, expr->tk->str);
        return;
    }
    case AST_NUM:
    {
        sprintf(expr->name, "%d", expr->tk->integer);
        return;
    }
    case AST_PLUS:
    case AST_MINUS:
    case AST_MUL:
    case AST_DIV:
    {
        int newT = newTemp();
        sprintf(expr->name, "temp%d", newT);
        int offset = updateOffsets(expr->name, symTable, expr->type);
        astNode *e1 = expr->leftChild;
        astNode *e2 = e1->nextSibling;
        getExpressionsCode(e1, symTable);
        getExpressionsCode(e2, symTable);
        expr->code = appendAtEnd(expr->code, e1->code);
        expr->code = appendAtEnd(expr->code, e2->code);
        quadruple *qp = initQuadruple();
        if (expr->label == AST_PLUS)
            qp->op = OP_PLUS;
        else if (expr->label == AST_MINUS)
            qp->op = OP_MINUS;
        else if (expr->label == AST_MUL)
            qp->op = OP_MUL;
        else
            qp->op = OP_DIV;
        strcpy(qp->resultant, expr->name);
        qp->offsetRes = offset;
        strcpy(qp->operand1, e1->name);
        if (atoi(qp->operand1) == 0)
        {
            qp->offsetOperand1 = getFromSymTable(symTable->corrHashtable, qp->operand1);
        }
        strcpy(qp->operand2, e2->name);
        if (atoi(qp->operand1) == 0)
        {
            qp->offsetOperand1 = getFromSymTable(symTable->corrHashtable, qp->operand1);
        }
        expr->code = appendAtEnd(expr->code, qp);
        return;
    }
    }
}

quadruple *generateWhileLoopCode(astNode *stmts)
{
    quadruple *head = initQuadruple();
    int label1 = newLabel();
    head->op = LABEL;
    sprintf(head->operand1, "label%d", label1);
    astNode *exprOnTop = stmts->leftChild;
    int label2 = newLabel();
    int label3 = newLabel();
    exprOnTop->trueCase = label2;
    exprOnTop->falseCase = label3;
    getExpressionsCode(exprOnTop);
    head->next = exprOnTop->code;
    quadruple *qp1 = initQuadruple();
    qp1->op = LABEL;
    sprintf(qp1->operand1, "label%d", label2);
    head = appendAtEnd(head, qp1);
    // stmts here
    quadruple *qp2 = initQuadruple();
    qp2->op = JUMP;
    sprintf(qp2->operand1, "label%d", label1);
    head = appendAtEnd(head, qp2);
    quadruple *qp3 = initQuadruple();
    qp3->op = LABEL;
    sprintf(qp3->operand1, "label%d", label3);
    head = appendAtEnd(head, qp3);
    return head;
}

quadruple *generateForLoopCode(astNode *stmts)
{
    quadruple *head = initQuadruple();
    SymTablePointer *symTable = stmts->symTable;
    astNode *loopVar = stmts->leftChild;
    astNode *range = stmts->leftChild->nextSibling;
    head->op = OP_ASSIGN;
    strcpy(head->resultant, loopVar->tk->str);
    // if signed??
    if (range->leftChild->isNegative)
    {
        snprintf(head->operand1, 25, "-%d", range->leftChild->tk->integer);
    }
    else
    {
        snprintf(head->operand1, 25, "%d", range->leftChild->tk->integer);
    }
    int label1 = newLabel();
    quadruple *tempQ1 = initQuadruple();
    tempQ1->op = LABEL;
    snprintf(tempQ1->operand1, 25, "label%d", label1);
    head = appendAtEnd(head, tempQ1);
    int temp0 = newTemp();
    quadruple *tempQ = initQuadruple();
    tempQ->op = OP_LE;
    strcpy(tempQ->operand1, loopVar->tk->str);
    if (range->leftChild->nextSibling->isNegative)
    {
        snprintf(tempQ->operand2, 25, "-%d", range->leftChild->nextSibling->tk->integer);
    }
    else
    {
        snprintf(tempQ->operand2, 25, "%d", range->leftChild->nextSibling->tk->integer);
    }
    snprintf(tempQ->resultant, 25, "temp%d", temp0);
    // tempQ1->next = tempQ;
    head = appendAtEnd(head, tempQ);
    quadruple *tempQ2 = initQuadruple();
    tempQ2->op = JUMPIFTRUE;
    int label2 = newLabel();
    snprintf(tempQ2->operand1, 25, "temp%d", temp0);
    snprintf(tempQ2->operand2, 25, "label%d", label2);
    // tempQ->next = tempQ2;
    head = appendAtEnd(head, tempQ2);
    quadruple *tempQ3 = initQuadruple();
    tempQ3->op = JUMP;
    int label3 = newLabel();
    snprintf(tempQ3->operand1, 25, "label%d", label3);
    // tempQ2->next = tempQ3;
    head = appendAtEnd(head, tempQ3);
    quadruple *tempQ4 = initQuadruple();
    tempQ4->op = LABEL;
    snprintf(tempQ4->operand1, 25, "label%d", label2);
    // tempQ3->next = tempQ4;
    head = appendAtEnd(head, tempQ4);
    // get ready for statements
    astNode *forLoopStmts = stmts->leftChild->nextSibling->nextSibling->leftChild;
    // get code for stmts
    quadruple *stmtsHead = stmtsCodeGen(forLoopStmts, symTable);
    appendAtEnd(head, stmtsHead);
    quadruple *tempQ5 = initQuadruple();
    strcpy(tempQ5->resultant, loopVar->tk->str);
    strcpy(tempQ5->operand1, loopVar->tk->str);
    tempQ5->op = OP_PLUS;
    snprintf(tempQ5->operand2, 25, "%d", 1);
    appendAtEnd(head, tempQ5);
    quadruple *tempQ6 = initQuadruple();
    tempQ6->op = JUMP;
    snprintf(tempQ6->operand1, 25, "label%d", label1);
    appendAtEnd(head, tempQ6);
    quadruple *tempQ7 = initQuadruple();
    tempQ7->op = LABEL;
    snprintf(tempQ7->operand1, 25, "label%d", label3);
    appendAtEnd(head, tempQ7);
    return head;
}

void getArrayCode(astNode *node, SymTablePointer *symTable)
{
    SymTablePointer *ptr = getFromAnySymTable(symTable, node->leftChild->tk->str);
    types type = ptr->typeIfArray.type;
    int width = -1;
    if (type == TYPE_INTEGER)
    {
        width = INT_WIDTH;
    }
    else if (type == TYPE_REAL)
    {
        width = REAL_WIDTH;
    }
    else
    {
        width = BOOL_WIDTH;
    }
    int temp = newTemp();
    sprintf(node->name, "temp%d", temp);
    // get expression code
    astNode *expr = node->leftChild->nextSibling;
    getExpressionsCode(expr);
    quadruple *qp = initQuadruple();
    qp->op = OP_MUL;
    strcpy(qp->resultant, node->name);
    strcpy(qp->operand1, expr->name);
    sprintf(qp->operand2, "%d", width);
    expr->code = appendAtEnd(expr->code, qp);
    node->code = expr->code;
    return;
}

void getGen(astNode *node, SymTablePointer *symTable)
{
    if (node->label == AST_ID)
    {
        strcpy(node->name, node->tk->str);
        return;
    }
    else if (node->label == AST_NUM)
    {
        sprintf(node->name, "%d", node->tk->integer);
        return;
    }
    else if (node->label == AST_RNUM)
    {
        sprintf(node->name, "%lf", node->tk->rnum);
        return;
    }
    else if (node->label == AST_BOOL)
    {
        strcpy(node->name, node->tk->str);
        return;
    }
    else
    {
        getArrayCode(node, symTable);
        // globalHead = appendAtEnd(globalHead, node->code);
        quadruple *qp = initQuadruple();
        qp->op = LW;
        int temp = newTemp();
        sprintf(qp->resultant, "temp%d", temp);
        strcpy(qp->operand1, node->leftChild->tk->str);
        strcpy(qp->operand2, node->name); // offset
        sprintf(node->name, "temp%d", temp);
        node->code = appendAtEnd(node->code, qp);
        return;
    }
}

quadruple *generateSwitchCaseCode(astNode *stmts)
{

    quadruple *head = initQuadruple();
    SymTablePointer *symTable = stmts->symTable;
    head->op = JUMP;
    int label0 = newLabel();
    snprintf(head->operand1, 25, "label%d", label0);

    astNode *switchExpr = stmts->leftChild;
    astNode *caseStmts = switchExpr->nextSibling;
    // get code for case statements
    astNode *case1 = caseStmts->leftChild;
    astNode *def = caseStmts->nextSibling;
    // check if boolean or integer switch
    if (def->leftChild == NULL)
    {
        bool *casevalues = malloc(sizeof(bool) * 2);
        int *caselabels = malloc(sizeof(int) * 2);
        int labelexit = newLabel();
        quadruple *tempQ1 = initQuadruple();
        int label1 = newLabel();
        tempQ1->op = LABEL;
        snprintf(tempQ1->operand1, 25, "label%d", label1);
        caselabels[0] = label1;
        casevalues[0] = true;
        head = appendAtEnd(head, tempQ1);

        quadruple *stmtsHead = stmtsCodeGen(case1->leftChild->nextSibling, symTable);
        head = appendAtEnd(head, stmtsHead);
        quadruple *tempQ2 = initQuadruple();
        tempQ2->op = JUMP;
        snprintf(tempQ2->operand1, 25, "label%d", labelexit);
        head = appendAtEnd(head, tempQ2);

        quadruple *tempQ3 = initQuadruple();
        int label2 = newLabel();
        tempQ3->op = LABEL;
        snprintf(tempQ3->operand1, 25, "label%d", label2);
        caselabels[1] = label2;
        casevalues[1] = false;
        head = appendAtEnd(head, tempQ3);

        stmtsHead = stmtsCodeGen(case1->leftChild->nextSibling->nextSibling, symTable);
        head = appendAtEnd(head, stmtsHead);
        quadruple *tempQ4 = initQuadruple();
        tempQ4->op = JUMP;
        snprintf(tempQ4->operand1, 25, "label%d", labelexit);
        head = appendAtEnd(head, tempQ4);

        quadruple *tempQ5 = initQuadruple();
        tempQ5->op = LABEL;
        snprintf(tempQ5->operand1, 25, "label%d", label0);
        head = appendAtEnd(head, tempQ5);

        quadruple *tempQ6 = initQuadruple();
        tempQ6->op = JUMPIFTRUE;
        strcpy(tempQ6->operand1, switchExpr->tk->str);
        snprintf(tempQ6->operand2, 25, "label%d", label1);
        head = appendAtEnd(head, tempQ6);

        quadruple *tempQ7 = initQuadruple();
        tempQ7->op = JUMP;
        snprintf(tempQ7->operand1, 25, "label%d", label2);
        head = appendAtEnd(head, tempQ7);

        quadruple *tempQ8 = initQuadruple();
        tempQ8->op = LABEL;
        snprintf(tempQ8->operand1, 25, "label%d", labelexit);
        head = appendAtEnd(head, tempQ8);

        free(casevalues);
        free(caselabels);
    }
    else
    {
        int *casevalues = malloc(sizeof(int) * 25);
        int *caselabels = malloc(sizeof(int) * 25);
        int size = 25;
        int i = 0; // tracks number of cases
        int labelexit = newLabel();
        while (case1 != NULL)
        {
            quadruple *tempQ1 = initQuadruple();
            // create a label for each case
            int label1 = newLabel();
            tempQ1->op = LABEL;
            snprintf(tempQ1->operand1, 25, "label%d", label1);
            caselabels[i] = label1;
            casevalues[i] = case1->leftChild->tk->integer;
            head = appendAtEnd(head, tempQ1);
            quadruple *stmtsHead = stmtsCodeGen(case1->leftChild->nextSibling, symTable);
            head = appendAtEnd(head, stmtsHead);
            quadruple *tempQ2 = initQuadruple();
            tempQ2->op = JUMP;
            snprintf(tempQ2->operand1, 25, "label%d", labelexit);
            head = appendAtEnd(head, tempQ2);
            case1 = case1->nextSibling;
            i++;

            // keep reallocating whenever we reach half of the allocated space
            if (i >= size / 2)
            {
                size = size * 2;
                casevalues = realloc(casevalues, sizeof(int) * size);
                caselabels = realloc(caselabels, sizeof(int) * size);
            }
        }
        // note the default checks are redundant, but I'm keeping them for now
        int labeldef = newLabel();
        if (def->leftChild != NULL)
        {
            quadruple *tempQ3 = initQuadruple();
            tempQ3->op = LABEL;
            snprintf(tempQ3->operand1, 25, "label%d", labeldef);
            head = appendAtEnd(head, tempQ3);
            quadruple *stmtsHead = stmtsCodeGen(def->leftChild, symTable);
            head = appendAtEnd(head, stmtsHead);
            quadruple *tempQ4 = initQuadruple();
            tempQ4->op = JUMP;
            snprintf(tempQ4->operand1, 25, "label%d", labelexit);
            head = appendAtEnd(head, tempQ4);
        }
        quadruple *tempQ5 = initQuadruple();
        tempQ5->op = LABEL;
        snprintf(tempQ5->operand1, 25, "label%d", label0);
        head = appendAtEnd(head, tempQ5);
        for (int j = 0; j < i; j++)
        {
            quadruple *tempQ6 = initQuadruple();
            int tempcmp = newTemp();
            tempQ6->op = OP_EQ;
            strcpy(tempQ6->operand1, switchExpr->tk->str);
            snprintf(tempQ6->operand2, 25, "%d", casevalues[j]);
            snprintf(tempQ6->resultant, 25, "temp%d", tempcmp);
            head = appendAtEnd(head, tempQ6);

            quadruple *tempQ7 = initQuadruple();
            tempQ7->op = JUMPIFTRUE;
            snprintf(tempQ7->operand1, 25, "temp%d", tempcmp);
            snprintf(tempQ7->operand2, 25, "label%d", caselabels[j]);
            head = appendAtEnd(head, tempQ7);
        }
        if (labeldef != -1)
        {
            quadruple *tempQ7 = initQuadruple();
            tempQ7->op = JUMP;
            snprintf(tempQ7->operand1, 25, "label%d", labeldef);
            head = appendAtEnd(head, tempQ7);
        }
        quadruple *tempQ8 = initQuadruple();
        tempQ8->op = LABEL;
        snprintf(tempQ8->operand1, 25, "label%d", labelexit);
        head = appendAtEnd(head, tempQ8);
        free(casevalues);
        free(caselabels);
    }
    return head;
}

quadruple *stmtsCodeGen(astNode *stmts, SymTablePointer *symTable)
{
    quadruple *tempHead = NULL;
    while (stmts != NULL)
    {
        if (stmts->label == AST_FOR)
        {
            quadruple *head1 = generateForLoopCode(stmts);
            tempHead = appendAtEnd(tempHead, head1);
            // stmts->code = head1;
        }
        else if (stmts->label == AST_WHILE)
        {
            quadruple *head1 = generateWhileLoopCode(stmts);
            tempHead = appendAtEnd(tempHead, head1);
        }
        else if (stmts->label == AST_SWITCH_CASE)
        {
            quadruple *head1 = generateSwitchCaseCode(stmts);
            tempHead = appendAtEnd(tempHead, head1);
        }
        else if (stmts->label == AST_PRINT)
        {
            quadruple *head = initQuadruple();
            head->op = OP_PRINT;
            getGen(stmts->leftChild, symTable);
            tempHead = appendAtEnd(tempHead, stmts->leftChild->code);
            strcpy(head->operand1, stmts->leftChild->name);
            tempHead = appendAtEnd(tempHead, head);
        }
        else if (stmts->label == AST_GETVALUE)
        {
            quadruple *head = initQuadruple();
            head->op = OP_GETVALUE;
            getGen(stmts->leftChild, symTable);
            strcpy(head->operand1, stmts->leftChild->name);
            tempHead = appendAtEnd(tempHead, head);
        }
        else if (stmts->label == AST_ASSIGNOP)
        {
            quadruple *head = initQuadruple();
            head->op = OP_ASSIGN;
            getExpressionsCode(stmts->leftChild->nextSibling, symTable);
            tempHead = appendAtEnd(tempHead, stmts->leftChild->nextSibling->code);
            strcpy(head->operand1, stmts->leftChild->nextSibling->name);
            if (stmts->leftChild->label == AST_ID)
            {
                strcpy(head->resultant, stmts->leftChild->tk->str);
            }
            tempHead = appendAtEnd(tempHead, head);
        }
        stmts = stmts->nextSibling;
    }
    return tempHead;
}

void startIntermCodeGen(astNode *root)
{
    astNode *mdls = root->leftChild;

    while (mdls != NULL)
    {

        switch (mdls->label)
        {
        case AST_DRIVERMODULE:
        {
            SymTablePointer *symTable = mdls->symTable;
            astNode *stmts = mdls->leftChild->leftChild->leftChild;
            quadruple *qp = stmtsCodeGen(stmts, symTable);
            quadruple *qp1 = initQuadruple();
            qp1->op = LABEL;
            strcpy(qp1->operand1, "driver");
            globalHead = appendAtEnd(globalHead, qp1);
            globalHead = appendAtEnd(globalHead, qp);
            mdls = mdls->nextSibling;
            break;
        }
        case AST_OTHERMODULES:
        {
            astNode *separateMdls = mdls->leftChild;
            // while (separateMdls != NULL)
            // {
            //     SymTablePointer *symTable = separateMdls->symTable;
            //     astNode *stmts = separateMdls->leftChild->nextSibling->leftChild;
            //     //
            //     while (stmts != NULL)
            //     {
            //         if (stmts->label == AST_FOR)
            //         {
            //             generateForLoopCode(stmts);
            //         }
            //     }
            //     separateMdls = separateMdls->nextSibling;
            // }

            mdls = mdls->nextSibling;
        }
        }
    }
}

char *EnumToOperatorString(operators nt)
{
    int i = 0;
    FILE *fp = fopen("operators.txt", "r");
    char *buffer = malloc(sizeof(char) * 100);
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 3] = '\0';
        // printf("%s\n", buffer);

        if (nt == i)
            return buffer;
        i++;
    }
    fclose(fp);
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
//     inorder_ast(astRoot);
//     populateGlobalSymbolTable(globalSymbolTable, astRoot, 0);
//     // if (semanticallyCorrect)
//     typeCheck(astRoot);
//     if (semanticallyCorrect && semanticRulesPassed)
//     {
//         startIntermCodeGen(astRoot);
//     }
//     while (globalHead != NULL)
//     {
//         printf("%20s %20s %20s %20s\n", EnumToOperatorString(globalHead->op), globalHead->operand1, globalHead->operand2, globalHead->resultant);
//         globalHead = globalHead->next;
//     }
// }