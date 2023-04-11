/*
Group - 29

Shaurya Marwah - 2019B3A70459P
Hari Sankar - 2019B3A70564P
Ruchir Kumbhare - 2019B5A70650P
Ashwin Murali - 2019B2A70957P
Dilip Venkatesh - 2020A7PS1203P

*/

#include <stdlib.h>
#include <string.h>
#include "parserDef.h"
#include "parser.h"
#include "ast.h"

astNode *append_at_end(astNode *head, astNode *append)
{
    astNode *temp = head;
    if (head == NULL)
    {
        astNode *new_head = append;
        append->nextSibling = NULL;
        return new_head;
    }
    while (head->nextSibling != NULL)
    {
        head = head->nextSibling;
    }
    head->nextSibling = append;
    append->nextSibling = NULL;
    return temp;
}

astNode *initASTNode(astNodeLabel label, astNode *leftChild)
{
    astNode *node = malloc(sizeof(astNode));
    node->label = label;
    node->isNegative = false;
    node->leftChild = leftChild;
    node->nextSibling = NULL; // need to check;
    node->symTable = NULL;
    node->tk = NULL;
    return node;
}

char *EnumToASTString(astNodeLabel nt)
{
    int i = 0;
    FILE *fp = fopen("astLabels.txt", "r");
    char *buffer = malloc(sizeof(char) * 100);
    while (fgets(buffer, 100, fp) != NULL)
    {
        buffer[strlen(buffer) - 2] = '\0';
        // printf("%s\n", buffer);

        if (nt == i)
            return buffer;
        i++;
    }
    fclose(fp);
}

void freeRHSList(treenode *root)
{
    treenode *children = root->child;
    while (children != NULL)
    {
        treenode *temp = children->nextSibling;
        free(children);
        children = temp;
    }
}

treenode *getNodeWithSymbol(nonTerminal nt, treenode *rootPass)
{
    treenode *root = rootPass->child;
    while (root != NULL)
    {
        // if (root->node.isTerminal)
        //     printf("heret: %s\n", EnumToTString(root->node.t));
        // else
        //     printf("here: %s\n", EnumToNTString(root->node.nt));
        if (!root->node.isTerminal && root->node.nt == nt)
        {
            return root;
        }
        root = root->nextSibling;
    }
    // should not reach here
    return NULL;
}

astNode *constructAST(treenode *root)
{
    switch (root->rule_No)
    {
    case 0: // program moduleDeclarations otherModules(2) driverModule otherModules(1)
        treenode *mds = getNodeWithSymbol(moduleDeclarations, root);
        treenode *oms2 = getNodeWithSymbol(otherModules, root);
        treenode *drimod = getNodeWithSymbol(driverModule, root);
        treenode *oms1 = root->child->nextSibling->nextSibling->nextSibling;
        oms2->inh = NULL;
        oms1->inh = NULL;
        mds->inh = NULL;

        astNode *mdsNode = constructAST(mds);

        astNode *oms2Node = constructAST(oms2);

        astNode *oms1Node = constructAST(oms1);

        astNode *driModNode = constructAST(drimod);

        // make linkedlist of these nodes
        astNode *programNode = initASTNode(AST_PROGRAM, NULL);
        if (mdsNode != NULL)
        {
            programNode->leftChild = append_at_end(programNode->leftChild, mdsNode);
        }
        if (oms2Node != NULL)
        {
            programNode->leftChild = append_at_end(programNode->leftChild, oms2Node);
        }
        else
        {
            programNode->leftChild = append_at_end(programNode->leftChild, initASTNode(AST_OTHERMODULES, NULL));
        }

        programNode->leftChild = append_at_end(programNode->leftChild, driModNode);

        if (oms1Node != NULL)
        {
            programNode->leftChild = append_at_end(programNode->leftChild, oms1Node);
        }
        else
        {
            programNode->leftChild = append_at_end(programNode->leftChild, initASTNode(AST_OTHERMODULES, NULL));
        }

        freeRHSList(root);
        return programNode;
    case 1: // moduleDeclarations moduleDeclaration moduleDeclarations(1)
    {
        treenode *md = getNodeWithSymbol(moduleDeclaration, root);
        treenode *mds1 = getNodeWithSymbol(moduleDeclarations, root);

        astNode *mdNode = constructAST(md);

        mds1->inh = append_at_end(root->inh, mdNode);

        constructAST(mds1);

        root->syn = mds1->syn;
        freeRHSList(root);
        return initASTNode(AST_MODULE_DECLARATIONS, root->syn);
    }
    case 2: // moduleDeclarations EPSILON
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 3:
    {
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->nextSibling->nextSibling->tk;
        freeRHSList(root);
        return initASTNode(AST_MODULE_DECLARATION, idNode);
    }
    case 4: // otherModules module otherModules(1)
    {
        treenode *mdl = getNodeWithSymbol(module, root);
        treenode *oms1 = getNodeWithSymbol(otherModules, root);

        astNode *mdlNode = constructAST(mdl);

        oms1->inh = append_at_end(root->inh, mdlNode);
        constructAST(oms1);
        root->syn = oms1->syn;
        freeRHSList(root);
        return initASTNode(AST_OTHERMODULES, root->syn);
    }
    case 5:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 6: // driverModule DRIVERDEF DRIVER PROGRAM DRIVERENDDEF module

    {
        treenode *modDef = getNodeWithSymbol(moduleDef, root);
        astNode *modDefNode = constructAST(modDef);
        freeRHSList(root);
        return initASTNode(AST_DRIVERMODULE, modDefNode);
    }
    case 7:
    {
        treenode *inputParaList = getNodeWithSymbol(input_plist, root);
        treenode *retP = getNodeWithSymbol(ret, root);
        treenode *modDef = getNodeWithSymbol(moduleDef, root);

        astNode *retNode = constructAST(retP);

        astNode *modDefNode = constructAST(modDef);

        astNode *inputParaListNode = constructAST(inputParaList);

        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->nextSibling->nextSibling->tk;

        inputParaList->inh = NULL;

        astNode *moduleNode = initASTNode(AST_MODULE, NULL);
        moduleNode->leftChild = append_at_end(moduleNode->leftChild, idNode);
        if (inputParaListNode != NULL)
        {
            moduleNode->leftChild = append_at_end(moduleNode->leftChild, inputParaListNode);
        } // need to add this node here..(if null)
        else
        {
            moduleNode->leftChild = append_at_end(moduleNode->leftChild, initASTNode(AST_INPUT_PARA_LIST, NULL));
        }
        if (retNode != NULL)
        {
            moduleNode->leftChild = append_at_end(moduleNode->leftChild, retNode);
        }
        else
        {
            moduleNode->leftChild = append_at_end(moduleNode->leftChild, initASTNode(AST_OUTPUT_PARA_LIST, NULL));
        }

        if (modDefNode != NULL)
        {
            moduleNode->leftChild = append_at_end(moduleNode->leftChild, modDefNode);
        }

        freeRHSList(root);
        return moduleNode;
    }
    case 8:
    {
        treenode *outputParaList = getNodeWithSymbol(output_plist, root);
        astNode *outputParaListNode = constructAST(outputParaList);
        freeRHSList(root);
        return outputParaListNode;
        // return initASTNode(AST_RET, outputParaListNode);
    }
    case 9:
    {
        freeRHSList(root);
        return initASTNode(AST_RET, NULL);
    }
    case 10:
    {
        treenode *dt = getNodeWithSymbol(dataType, root);
        treenode *n_1 = getNodeWithSymbol(n1, root);

        astNode *dtNode = constructAST(dt);
        astNode *idNode = initASTNode(AST_ID, NULL);

        idNode->tk = root->child->tk;
        dtNode->nextSibling = idNode;
        astNode *input_para_1 = initASTNode(AST_INPUT_PARA, dtNode);

        n_1->inh = append_at_end(root->inh, input_para_1);

        constructAST(n_1);

        root->syn = n_1->syn;
        astNode *temp = n_1->syn;
        freeRHSList(root);
        return initASTNode(AST_INPUT_PARA_LIST, root->syn);
    }
    case 11:
    {
        treenode *dt = getNodeWithSymbol(dataType, root);
        treenode *n_1 = getNodeWithSymbol(n1, root);
        astNode *dtNode = constructAST(dt);

        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->nextSibling->tk;
        dtNode->nextSibling = idNode;
        astNode *input_para_1 = initASTNode(AST_INPUT_PARA, dtNode);
        n_1->inh = append_at_end(root->inh, input_para_1);
        constructAST(n_1);
        root->syn = n_1->syn;
        freeRHSList(root);
        return NULL;
    }
    case 12:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 13:
    {
        treenode *dt = getNodeWithSymbol(type, root);
        treenode *n_1 = getNodeWithSymbol(n2, root);
        astNode *dtNode = constructAST(dt);

        astNode *idNode = initASTNode(AST_ID, NULL);

        idNode->tk = root->child->tk;
        dtNode->nextSibling = idNode;
        astNode *output_para_1 = initASTNode(AST_OUTPUT_PARA, dtNode);

        n_1->inh = append_at_end(root->inh, output_para_1);

        constructAST(n_1);

        root->syn = n_1->syn;
        freeRHSList(root);
        return initASTNode(AST_OUTPUT_PARA_LIST, root->syn);
    }
    case 14:
    {
        treenode *dt = getNodeWithSymbol(type, root);
        treenode *n_1 = getNodeWithSymbol(n2, root);

        astNode *dtNode = constructAST(dt);

        astNode *idNode = initASTNode(AST_ID, NULL);

        idNode->tk = root->child->nextSibling->tk;
        dtNode->nextSibling = idNode;
        astNode *input_para_1 = initASTNode(AST_INPUT_PARA, dtNode);
        n_1->inh = append_at_end(root->inh, input_para_1);
        constructAST(n_1);

        root->syn = n_1->syn;
        freeRHSList(root);
        return NULL;
    }
    case 15:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 16 ... 18:
    {
        astNode *dt = initASTNode(AST_DATATYPE, NULL);
        dt->tk = root->child->tk;
        freeRHSList(root);
        return dt;
    }
    case 19:
    {
        astNode *dt = initASTNode(AST_ARRAY_DATATYPE, NULL);
        treenode *range_arr = getNodeWithSymbol(range_arrays, root);
        treenode *type1 = getNodeWithSymbol(type, root);
        astNode *dt1 = constructAST(type1);
        astNode *range_arrNode = constructAST(range_arr);
        dt1->nextSibling = range_arrNode;
        dt->leftChild = dt1;
        freeRHSList(root);
        return dt;
    }
    case 20:
    {
        treenode *index_arr1 = getNodeWithSymbol(index_arr, root);
        treenode *index_arr2 = index_arr1->nextSibling->nextSibling;
        astNode *index_arr1Node = constructAST(index_arr1);
        astNode *index_arr2Node = constructAST(index_arr2);

        index_arr1Node->nextSibling = index_arr2Node;
        freeRHSList(root);
        return initASTNode(AST_RANGE, index_arr1Node);
    }
    case 21 ... 23:
    {
        astNode *dt = initASTNode(AST_DATATYPE, NULL);
        dt->tk = root->child->tk;
        freeRHSList(root);
        return dt;
    }

    case 24:
    {
        treenode *stmts = getNodeWithSymbol(statements, root);
        stmts->inh = NULL;

        constructAST(stmts);

        root->syn = stmts->addr;

        astNode *node = initASTNode(AST_MODULEDEF, root->syn);
        node->line_start = root->child->tk->line_num;
        node->line_end = root->child->nextSibling->nextSibling->tk->line_num;
        freeRHSList(root);
        return node;
    }
    case 25: // statements(2) statement statements(1)
    {
        treenode *stmt = getNodeWithSymbol(statement, root);
        treenode *stmts1 = getNodeWithSymbol(statements, root);
        astNode *stmtNode = constructAST(stmt);
        stmts1->inh = append_at_end(root->inh, stmt->addr);
        constructAST(stmts1);
        root->syn = stmts1->syn;

        freeRHSList(root);
        root->addr = initASTNode(AST_STATEMENTS, root->syn);
        astNode *temp = root->syn;
        return root->addr;
    }
    case 26:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 27:
    {

        astNode *ioStmtNode = constructAST(root->child);
        root->addr = ioStmtNode;
        freeRHSList(root);
        return ioStmtNode;
    }
    case 28 ... 31:
    {
        constructAST(root->child);
        root->addr = root->child->addr;
        freeRHSList(root);
        return root->addr;
    }
    case 32:
    {
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->nextSibling->nextSibling->tk;
        freeRHSList(root);
        return initASTNode(AST_GETVALUE, idNode);
    }
    case 33: // ioStmt PRINT BO var_print BC SEMICOL
    {
        astNode *varPrintNode = constructAST(getNodeWithSymbol(var_print, root));
        freeRHSList(root);
        return initASTNode(AST_PRINT, varPrintNode);
    }
    case 34 ... 35:
    {
        astNode *boolNode = initASTNode(AST_BOOL, NULL);
        boolNode->tk = root->child->tk;
        freeRHSList(root);
        return boolNode;
    }
    case 36:
    {
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->tk;
        freeRHSList(root);
        return idNode;
    }
    case 37:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        freeRHSList(root);
        return numNode;
    }
    case 38:
    {
        astNode *rnumNode = initASTNode(AST_RNUM, NULL);
        rnumNode->tk = root->child->tk;
        freeRHSList(root);
        return rnumNode;
    }
    case 39:
    {
        treenode *varPrintDash = getNodeWithSymbol(var_print_dash, root);
        astNode *varPrintDashNode = constructAST(varPrintDash);
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->tk;
        if (varPrintDashNode == NULL)
        {
            freeRHSList(root);
            return idNode;
        }
        else
        {
            astNode *arr_ele = initASTNode(AST_ARRAY_ELEMENT, NULL);
            arr_ele->leftChild = idNode;
            idNode->nextSibling = varPrintDashNode;
            freeRHSList(root);
            return arr_ele;
        }
    }
    case 40:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        freeRHSList(root);
        return numNode;
    }
    case 41:
    {
        astNode *rnumNode = initASTNode(AST_RNUM, NULL);
        rnumNode->tk = root->child->tk;
        freeRHSList(root);
        return rnumNode;
    }
    case 42:
    {
        astNode *boolNode = constructAST(root->child);
        freeRHSList(root);
        return boolNode;
    }
    case 43:
    {
        freeRHSList(root);
        return NULL;
    }
    case 44:
    {
        treenode *indexArr = getNodeWithSymbol(index_arr, root);
        astNode *indexArrNode = constructAST(indexArr);
        freeRHSList(root);
        return indexArrNode;
    }
    case 45 ... 46:
    {
        astNode *child = constructAST(root->child);
        root->addr = child;
        freeRHSList(root);

        return child;
    }
    case 47:
    {
        treenode *whichSt = getNodeWithSymbol(whichStmt, root);
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->tk;
        whichSt->inh = idNode;
        constructAST(whichSt);
        astNode *assignNode = initASTNode(AST_ASSIGNOP, NULL);
        assignNode->leftChild = append_at_end(assignNode->leftChild, whichSt->syn);
        assignNode->leftChild = append_at_end(assignNode->leftChild, whichSt->addr);
        root->addr = assignNode;
        freeRHSList(root);
        return assignNode;
    }
    case 48:
    {
        treenode *lvalueIDStmt1 = getNodeWithSymbol(lvalueIDStmt, root);
        astNode *lvalueIDStmt1Node = constructAST(lvalueIDStmt1);

        root->addr = lvalueIDStmt1->addr;
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 49:
    {
        treenode *lvalueARRStmt1 = getNodeWithSymbol(lvalueARRStmt, root);
        lvalueARRStmt1->inh = root->inh;
        astNode *node = constructAST(lvalueARRStmt1);
        root->syn = lvalueARRStmt1->syn;
        root->addr = lvalueARRStmt1->addr;
        freeRHSList(root);
        return NULL;
    }
    case 50:
    {
        treenode *expression1 = getNodeWithSymbol(expression, root);
        constructAST(expression1);
        root->addr = expression1->addr;
        freeRHSList(root);
        return NULL;
    }
    case 51:
    {
        treenode *element_index_with_expressions1 = getNodeWithSymbol(element_index_with_expressions, root);
        astNode *node = initASTNode(AST_ARRAY_ELEMENT, NULL);
        node->leftChild = append_at_end(node->leftChild, root->inh);
        constructAST(element_index_with_expressions1);

        node->leftChild = append_at_end(node->leftChild, element_index_with_expressions1->addr);
        treenode *expression1 = getNodeWithSymbol(expression, root);
        constructAST(expression1);
        root->addr = expression1->addr;
        root->syn = node;
        freeRHSList(root);
        return NULL;
    }
    case 52:
    {
        treenode *sign1 = getNodeWithSymbol(sign, root);
        treenode *new_index1 = getNodeWithSymbol(new_index, root);
        astNode *signNode = constructAST(sign1);
        astNode *new_indexNode = constructAST(new_index1);
        if (signNode == NULL)
        {
            freeRHSList(root);
            return new_indexNode;
        }
        else
        {
            if (signNode->tk->token == MINUS)
            {
                new_indexNode->isNegative = true;
            }
            freeRHSList(root);
            return new_indexNode;
        }
    }
    case 53:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        freeRHSList(root);
        root->addr = numNode;
        return numNode;
    }
    case 54:
    {
        astNode *numNode = initASTNode(AST_ID, NULL);
        numNode->tk = root->child->tk;
        freeRHSList(root);
        root->addr = numNode;
        return numNode;
    }
    case 55 ... 56:
    {
        astNode *node = initASTNode(AST_SIGN, NULL);
        node->tk = root->child->tk;
        freeRHSList(root);
        return node;
    }
    case 57:
    {
        freeRHSList(root);
        return NULL;
    }
    case 58:
    {
        treenode *optionalNode = root->child;
        treenode *id = optionalNode->nextSibling->nextSibling->nextSibling;
        treenode *actualPara = getNodeWithSymbol(actual_para_list, root);
        actualPara->inh = NULL;
        astNode *optionalNode1 = constructAST(optionalNode);
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = id->tk;
        astNode *actualParaNode = constructAST(actualPara);
        astNode *node = initASTNode(AST_MODULE_REUSE, NULL);
        if (optionalNode->addr != NULL)
        {
            node->leftChild = append_at_end(node->leftChild, optionalNode->addr);
        }
        else
        {
            astNode *newNode = initASTNode(AST_OPTIONAL, NULL);
            newNode->leftChild = initASTNode(AST_IDLIST, NULL);
            newNode->tk = root->child->nextSibling->tk;
            node->leftChild = append_at_end(node->leftChild, newNode);
        }
        node->leftChild = append_at_end(node->leftChild, idNode);
        if (actualPara->addr != NULL)
        {
            astNode *newNode = initASTNode(AST_ACTUAL_PARA, NULL);
            newNode->leftChild = actualPara->addr;
            node->leftChild = append_at_end(node->leftChild, newNode);
        }
        freeRHSList(root);
        return node;
    }
    case 59:
    {
        treenode *signNode = root->child;
        treenode *knode = signNode->nextSibling;
        treenode *rootDash = knode->nextSibling;
        astNode *signNode1 = constructAST(signNode);
        constructAST(knode);
        if (signNode1 == NULL)
            rootDash->inh = append_at_end(rootDash->inh, knode->addr);
        else
        {
            if (signNode1->tk->token == MINUS)
            {
                knode->addr->isNegative = true;
            }
            // astNode *node = initASTNode(AST_SIGNED, NULL);
            // node->leftChild = signNode1;
            // signNode1->nextSibling = knode->addr;
            rootDash->inh = append_at_end(rootDash->inh, knode->addr);
        }
        constructAST(rootDash);
        root->addr = rootDash->syn;
        freeRHSList(root);
        return NULL;
    }
    case 60:
    {
        treenode *knode = root->child;
        treenode *rootDash = knode->nextSibling;
        astNode *boolNode = constructAST(knode);
        rootDash->inh = append_at_end(root->inh, boolNode);
        constructAST(rootDash);
        root->addr = rootDash->syn;
        freeRHSList(root);
        return NULL;
    }
    case 61:
    {
        treenode *kboolNode = root->child->nextSibling;
        treenode *root1 = kboolNode->nextSibling;
        astNode *kboolAstNode = constructAST(kboolNode);
        root1->inh = append_at_end(root->inh, kboolNode->addr);

        constructAST(root1);

        root->syn = root1->syn;
        freeRHSList(root);
        return NULL;
    }
    case 62:
    {
        treenode *signNode = root->child;
        treenode *knode = signNode->nextSibling;
        astNode *signASTNode = constructAST(signNode);
        constructAST(knode);
        if (signASTNode == NULL)
        {
            root->addr = knode->addr;
        }
        else
        {
            if (signASTNode->tk->token == MINUS)
            {
                knode->addr->isNegative = true;
            }
            // astNode *newSignNode = initASTNode(AST_SIGNED, NULL);
            // newSignNode->leftChild = signASTNode;
            // signASTNode->nextSibling = knode->addr;
            root->addr = knode->addr;
        }
        freeRHSList(root);
        return NULL;
    }
    case 63:
    {
        root->addr = constructAST(root->child);
        freeRHSList(root);
        return NULL;
    }
    case 64:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 65:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 66:
    {
        astNode *numNode = initASTNode(AST_RNUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 67:
    {
        treenode *idNode = root->child;
        treenode *n11Node = idNode->nextSibling;
        constructAST(n11Node);
        astNode *idAstNode = initASTNode(AST_ID, NULL);
        idAstNode->tk = idNode->tk;
        if (n11Node->addr == NULL)
        {
            root->addr = idAstNode;
        }
        else
        {
            root->addr = initASTNode(AST_ARRAY_ELEMENT, idAstNode);
            root->addr->leftChild = append_at_end(root->addr->leftChild, n11Node->addr);
        }
        freeRHSList(root);
        return NULL;
    }
    case 68:
    {
        constructAST(root->child->nextSibling);
        root->addr = root->child->nextSibling->addr;
        freeRHSList(root);
        return NULL;
    }
    case 69:
    {
        root->addr = NULL;
        freeRHSList(root);
        return NULL;
    }
    case 70:
    {
        constructAST(root->child->nextSibling);
        root->addr = initASTNode(AST_OPTIONAL, root->child->nextSibling->syn);
        root->addr->tk = root->child->tk;
        freeRHSList(root);
        return NULL;
    }
    case 71:
    {
        root->addr = NULL;
        freeRHSList(root);
        return NULL;
    }
    case 72:
    {
        treenode *idNode = root->child;
        treenode *n3Node = idNode->nextSibling;
        astNode *idNodeAST = initASTNode(AST_ID, NULL);
        idNodeAST->tk = idNode->tk;
        n3Node->inh = append_at_end(root->inh, idNodeAST);
        constructAST(n3Node);
        astNode *node = initASTNode(AST_IDLIST, n3Node->syn);
        root->syn = node;
        freeRHSList(root);
        return NULL;
    }
    case 73:
    {
        treenode *idNode = root->child->nextSibling;
        treenode *n3Node = idNode->nextSibling;
        astNode *idNodeAST = initASTNode(AST_ID, NULL);
        idNodeAST->tk = idNode->tk;
        n3Node->inh = append_at_end(root->inh, idNodeAST);
        constructAST(n3Node);

        root->syn = n3Node->syn;
        freeRHSList(root);
        return NULL;
    }
    case 74:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 75:
    {
        constructAST(root->child);
        root->addr = root->child->addr;
        freeRHSList(root);
        return NULL;
    }
    case 76:
    {
        constructAST(root->child);
        root->addr = root->child->addr;
        freeRHSList(root);
        return NULL;
    }
    case 77:
    {
        astNode *node = initASTNode(AST_SIGNED, NULL);
        treenode *unary = getNodeWithSymbol(unary_op, root);
        constructAST(unary);
        constructAST(root->child->nextSibling);
        node->leftChild = append_at_end(node->leftChild, unary->addr);
        node->leftChild = append_at_end(node->leftChild, root->child->nextSibling->addr);
        root->addr = node;
        freeRHSList(root);
        return NULL;
    }
    case 78:
    {
        constructAST(root->child->nextSibling);
        root->addr = root->child->nextSibling->addr;
        freeRHSList(root);
        return NULL;
    }
    case 79:
    {
        constructAST(root->child);
        root->addr = root->child->addr;
        freeRHSList(root);
        return NULL;
    }
    case 80:
    {
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->tk;
        root->addr = idNode;
        freeRHSList(root);
        return NULL;
    }
    case 81:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 82:
    {
        astNode *rnumNode = initASTNode(AST_RNUM, NULL);
        rnumNode->tk = root->child->tk;
        root->addr = rnumNode;
        freeRHSList(root);
        return NULL;
    }
    case 83 ... 84:
    {
        astNode *signNode = initASTNode(AST_SIGN, NULL);
        signNode->tk = root->child->tk;
        root->addr = signNode;
        freeRHSList(root);
        return NULL;
    }
    case 85:
    {
        treenode *n71 = getNodeWithSymbol(n7, root);
        treenode *anyTerm1 = getNodeWithSymbol(anyTerm, root);
        constructAST(anyTerm1);
        n71->inh = anyTerm1->addr;
        constructAST(n71);

        root->addr = n71->addr;
        freeRHSList(root);
        return NULL;
    }
    case 86:
    {
        treenode *logOp = getNodeWithSymbol(logicalOp, root);
        treenode *anyTerm1 = getNodeWithSymbol(anyTerm, root);
        treenode *n71 = getNodeWithSymbol(n7, root);
        constructAST(anyTerm1);
        n71->inh = anyTerm1->addr;
        constructAST(n71);
        constructAST(logOp);
        if (logOp->addr->tk->token == AND)
        {
            astNode *node = initASTNode(AST_AND, NULL);

            node->leftChild = append_at_end(node->leftChild, root->inh);

            node->leftChild = append_at_end(node->leftChild, n71->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
        else if (logOp->addr->tk->token == OR)
        {
            astNode *node = initASTNode(AST_OR, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, n71->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
    }
    case 87:
    {
        root->addr = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 88:
    {
        treenode *arithmeticExpr1 = getNodeWithSymbol(arithmeticExpr, root);
        treenode *n81 = getNodeWithSymbol(n8, root);
        constructAST(arithmeticExpr1);
        n81->inh = arithmeticExpr1->addr;
        constructAST(n81);
        root->addr = n81->addr;
        freeRHSList(root);
        return NULL;
    }
    case 89:
    {
        root->addr = constructAST(root->child);
        freeRHSList(root);
        return NULL;
    }
    case 90:
    {
        treenode *relOp = getNodeWithSymbol(relationalOp, root);
        treenode *arithmeticExpr1 = getNodeWithSymbol(arithmeticExpr, root);
        constructAST(relOp);
        constructAST(arithmeticExpr1);
        if (relOp->addr->tk->token == LT)
        {
            astNode *node = initASTNode(AST_LT, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, arithmeticExpr1->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
        else if (relOp->addr->tk->token == LE)
        {
            astNode *node = initASTNode(AST_LE, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, arithmeticExpr1->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
        else if (relOp->addr->tk->token == GT)
        {
            astNode *node = initASTNode(AST_GT, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, arithmeticExpr1->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
        else if (relOp->addr->tk->token == GE)
        {
            astNode *node = initASTNode(AST_GE, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, arithmeticExpr1->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
        else if (relOp->addr->tk->token == EQ)
        {
            astNode *node = initASTNode(AST_EQ, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, arithmeticExpr1->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
        else if (relOp->addr->tk->token == NE)
        {
            astNode *node = initASTNode(AST_NE, NULL);
            node->leftChild = append_at_end(node->leftChild, root->inh);
            node->leftChild = append_at_end(node->leftChild, arithmeticExpr1->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
    }
    case 91:
    {
        root->addr = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 92:
    {
        treenode *termNode = getNodeWithSymbol(term, root);
        treenode *n41 = termNode->nextSibling;
        constructAST(termNode);
        n41->inh = termNode->addr;
        constructAST(n41);
        if (n41->addr != n41->inh)
        {
            root->addr = n41->addr;
        }
        else
        {
            root->addr = termNode->addr;
        }
        freeRHSList(root);
        return NULL;
    }
    case 93:
    {
        treenode *op1Node = root->child;
        treenode *term1 = op1Node->nextSibling;
        treenode *n41 = term1->nextSibling;
        constructAST(term1);
        n41->inh = term1->addr;
        constructAST(n41);
        constructAST(op1Node);

        if (n41->inh != n41->addr)
        {

            if (op1Node->addr->tk->token == PLUS)
            {

                root->addr = initASTNode(AST_PLUS, NULL);
                root->addr->tk = op1Node->tk;
                root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
                root->addr->leftChild = append_at_end(root->addr->leftChild, n41->addr);
            }
            else
            {
                root->addr = initASTNode(AST_MINUS, NULL);
                root->addr->tk = op1Node->tk;
                root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
                root->addr->leftChild = append_at_end(root->addr->leftChild, n41->addr);
            }
        }
        else
        {

            if (op1Node->addr->tk->token == PLUS)
            {

                root->addr = initASTNode(AST_PLUS, NULL);
                root->addr->tk = op1Node->tk;
                root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);

                root->addr->leftChild = append_at_end(root->addr->leftChild, term1->addr);
            }
            else
            {
                root->addr = initASTNode(AST_MINUS, NULL);
                root->addr->tk = op1Node->tk;
                root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
                root->addr->leftChild = append_at_end(root->addr->leftChild, term1->addr);
            }
        }
        freeRHSList(root);
        return NULL;
    }
    case 94:
    {
        root->addr = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 95:
    {
        treenode *factorNode = root->child;
        treenode *n51 = factorNode->nextSibling;
        constructAST(factorNode);
        n51->inh = factorNode->addr;
        constructAST(n51);
        if (n51->addr != n51->inh)
        {
            root->addr = n51->addr;
        }
        else
        {
            root->addr = factorNode->addr;
        }
        freeRHSList(root);
        return NULL;
    }
    case 96:
    {
        treenode *op2Node = root->child;
        treenode *factorNode = op2Node->nextSibling;
        treenode *n51 = factorNode->nextSibling;
        constructAST(factorNode);
        n51->inh = factorNode->addr;
        constructAST(n51);
        constructAST(op2Node);
        if (op2Node->addr->tk->token == MUL)
        {

            root->addr = initASTNode(AST_MUL, NULL);
            root->addr->tk = op2Node->tk;
            root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
            root->addr->leftChild = append_at_end(root->addr->leftChild, n51->addr);
        }
        else
        {
            root->addr = initASTNode(AST_DIV, NULL);
            root->addr->tk = op2Node->tk;
            root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
            root->addr->leftChild = append_at_end(root->addr->leftChild, n51->addr);
        }
        freeRHSList(root);
        return NULL;
    }
    case 97:
    {
        root->addr = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 98:
    {
        treenode *node = root->child->nextSibling;
        constructAST(node);
        root->addr = node->addr;
        freeRHSList(root);
        return NULL;
    }
    //
    case 99:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 100:
    {
        astNode *numNode = initASTNode(AST_RNUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    // case 101:
    // {
    //     astNode *node = constructAST(root->child);
    //     root->addr = node;
    //     return NULL;
    // }//omit this rule
    case 101:
    {
        treenode *arr_ele = root->child->nextSibling;
        constructAST(arr_ele);
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->tk;
        if (arr_ele->addr == NULL)
        {

            root->addr = idNode;
            freeRHSList(root);
            return NULL;
        }
        else
        {
            astNode *node = initASTNode(AST_ARRAY_ELEMENT, NULL);
            node->leftChild = append_at_end(node->leftChild, idNode);
            node->leftChild = append_at_end(node->leftChild, arr_ele->addr);
            root->addr = node;
            freeRHSList(root);
            return NULL;
        }
    }
    case 102:
    {
        treenode *index_with_exp = root->child->nextSibling;
        constructAST(index_with_exp);
        root->addr = index_with_exp->addr;
        freeRHSList(root);
        return NULL;
    }
    case 103:
    {
        root->addr = NULL;
        freeRHSList(root);
        return NULL;
    }
    case 104:
    {
        treenode *signNode = root->child;
        treenode *n10 = signNode->nextSibling;
        astNode *signNode1 = constructAST(signNode);
        constructAST(n10);
        astNode *node = initASTNode(AST_SIGNED, NULL);
        node->leftChild = append_at_end(node->leftChild, signNode1);
        node->leftChild = append_at_end(node->leftChild, n10->addr);
        root->addr = node;
        freeRHSList(root);
        return NULL;
    }
    case 105:
    {
        treenode *arrE = root->child;
        constructAST(arrE);
        root->addr = arrE->addr;
        freeRHSList(root);
        return NULL;
    }
    case 106:
    {
        astNode *boolNode = constructAST(root->child);
        root->addr = boolNode;
        freeRHSList(root);
        return NULL;
    }
    case 107:
    {
        constructAST(root->child);
        root->addr = root->child->addr;
        freeRHSList(root);
        return NULL;
    }
    case 108:
    {
        constructAST(root->child->nextSibling);
        root->addr = root->child->nextSibling->addr;
        freeRHSList(root);
        return NULL;
    }
    case 109:
    {
        treenode *arrT = root->child;
        treenode *arrN4 = arrT->nextSibling;
        constructAST(arrT);
        arrN4->inh = arrT->addr;
        constructAST(arrN4);
        root->addr = arrN4->addr;
        freeRHSList(root);
        return NULL;
    }
    case 110:
    {
        astNode *boolNode = constructAST(root->child);
        root->addr = boolNode;
        freeRHSList(root);
        return NULL;
    }
    case 111:
    {
        treenode *op1Node = root->child;
        treenode *arrT = op1Node->nextSibling;
        treenode *arrN4 = arrT->nextSibling;
        constructAST(arrT);
        arrN4->inh = arrT->addr;
        constructAST(arrN4);
        constructAST(op1Node);
        if (op1Node->addr->tk->token == PLUS)
        {

            root->addr = initASTNode(AST_PLUS, NULL);
            root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
            root->addr->leftChild = append_at_end(root->addr->leftChild, arrN4->addr);
        }
        else
        {

            root->addr = initASTNode(AST_MINUS, NULL);
            root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
            root->addr->leftChild = append_at_end(root->addr->leftChild, arrN4->addr);
        }
        freeRHSList(root);
        return NULL;
    }
    case 112:
    {
        root->addr = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 113:
    {
        treenode *factorNode = root->child;
        treenode *n51 = factorNode->nextSibling;
        constructAST(factorNode);
        n51->inh = factorNode->addr;
        constructAST(n51);
        if (n51->addr != n51->inh)
        {
            root->addr = n51->addr;
        }
        else
        {
            root->addr = factorNode->addr;
        }
        freeRHSList(root);
        return NULL;
    }
    case 114:
    {
        treenode *op2Node = root->child;
        treenode *factorNode = op2Node->nextSibling;
        treenode *n51 = factorNode->nextSibling;
        constructAST(factorNode);
        n51->inh = factorNode->addr;
        constructAST(n51);
        constructAST(op2Node);
        if (op2Node->addr->tk->token == MUL)
        {

            root->addr = initASTNode(AST_MUL, NULL);
            root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
            root->addr->leftChild = append_at_end(root->addr->leftChild, n51->addr);
        }
        else
        {
            root->addr = initASTNode(AST_DIV, NULL);
            root->addr->leftChild = append_at_end(root->addr->leftChild, root->inh);
            root->addr->leftChild = append_at_end(root->addr->leftChild, n51->addr);
        }
        freeRHSList(root);
        return NULL;
    }
    case 115:
    {
        root->addr = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 116:
    {
        astNode *idNode = initASTNode(AST_ID, NULL);
        idNode->tk = root->child->tk;
        root->addr = idNode;
        freeRHSList(root);
        return NULL;
    }
    case 117:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 118:
    {
        treenode *expr = root->child->nextSibling;
        constructAST(expr);
        root->addr = expr->addr;
        freeRHSList(root);
        return NULL;
    }
    case 119 ... 130:
    {
        root->addr = initASTNode(AST_ID, NULL);

        root->addr->tk = root->child->tk;
        freeRHSList(root);
        return NULL;
    }
    case 131:
    {
        treenode *idListNode = root->child->nextSibling;
        treenode *dtNode = idListNode->nextSibling->nextSibling;
        astNode *dt = constructAST(dtNode);
        idListNode->inh = NULL;
        constructAST(idListNode);
        root->syn = idListNode->syn;

        root->addr = initASTNode(AST_DECLARE, NULL);
        root->addr->leftChild = root->syn;
        root->addr->leftChild->nextSibling = dt;
        freeRHSList(root);
        return NULL;
    }
    case 132:
    {
        treenode *idNode = root->child->nextSibling->nextSibling;
        astNode *idAST = initASTNode(AST_ID, NULL);
        idAST->tk = idNode->tk;
        treenode *caseStmtsNode = getNodeWithSymbol(caseStmts, root);
        treenode *defNode = caseStmtsNode->nextSibling;
        caseStmtsNode->inh = NULL;
        constructAST(caseStmtsNode);
        constructAST(defNode);
        astNode *node = initASTNode(AST_SWITCH_CASE, NULL);
        node->leftChild = idAST;
        if (caseStmtsNode->syn != NULL)
        {
            astNode *newNode = initASTNode(AST_CASES, NULL);
            newNode->leftChild = caseStmtsNode->syn;
            node->leftChild = append_at_end(node->leftChild, newNode);
        }
        if (defNode->addr != NULL)
        {
            node->leftChild = append_at_end(node->leftChild, defNode->addr);
        }
        else
        {
            node->leftChild = append_at_end(node->leftChild, initASTNode(AST_DEFAULT, NULL));
        }
        root->addr = node;
        freeRHSList(root);
        return NULL;
    }
    case 133:
    {
        treenode *valueNode = root->child->nextSibling;
        treenode *stmts = valueNode->nextSibling->nextSibling;
        treenode *n9Node = stmts->nextSibling->nextSibling->nextSibling;
        constructAST(valueNode);
        astNode *stmtsNode = constructAST(stmts);
        astNode *caseNode = initASTNode(AST_CASE, NULL);
        caseNode->line_start = root->child->tk->line_num;
        caseNode->line_end = stmts->nextSibling->nextSibling->tk->line_num;

        caseNode->leftChild = append_at_end(caseNode->leftChild, valueNode->addr);
        if (stmts->addr != NULL)
        {

            caseNode->leftChild = append_at_end(caseNode->leftChild, stmts->addr);
        }
        else
        {
            caseNode->leftChild = append_at_end(caseNode->leftChild, initASTNode(AST_STATEMENTS, NULL));
        }
        n9Node->inh = append_at_end(root->inh, caseNode);
        constructAST(n9Node);
        root->syn = n9Node->syn;
        freeRHSList(root);
        return NULL;
    }
    case 134:
    {
        treenode *valueNode = root->child->nextSibling;
        treenode *stmts = valueNode->nextSibling->nextSibling;
        treenode *n9Node = stmts->nextSibling->nextSibling->nextSibling;
        constructAST(valueNode);
        astNode *stmtsNode = constructAST(stmts);
        astNode *caseNode = initASTNode(AST_CASE, NULL);
        caseNode->line_start = root->child->tk->line_num;
        caseNode->line_end = stmts->nextSibling->nextSibling->tk->line_num;
        caseNode->leftChild = valueNode->addr;
        if (stmts->addr != NULL)
        {
            caseNode->leftChild = append_at_end(caseNode->leftChild, stmts->addr);
        }
        else
        {
            caseNode->leftChild = append_at_end(caseNode->leftChild, initASTNode(AST_STATEMENTS, NULL));
        }
        n9Node->inh = append_at_end(root->inh, caseNode);
        constructAST(n9Node);
        root->syn = n9Node->syn;
        freeRHSList(root);
        return NULL;
    }
    case 135:
    {
        root->syn = root->inh;
        freeRHSList(root);
        return NULL;
    }
    case 136:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 137 ... 138:
    {
        astNode *numNode = initASTNode(AST_BOOL, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 139:
    {
        treenode *stmtsNode = root->child->nextSibling->nextSibling;
        astNode *stmtsAST = constructAST(stmtsNode);
        astNode *newNode = initASTNode(AST_DEFAULT, NULL);
        newNode->line_start = root->child->tk->line_num;
        newNode->line_end = stmtsNode->nextSibling->nextSibling->tk->line_num;
        newNode->tk = root->child->tk;
        newNode->leftChild = stmtsAST;
        root->addr = newNode;
        freeRHSList(root);
        return NULL;
    }
    case 140:
    {
        root->addr = NULL;
        freeRHSList(root);
        return NULL;
    }
    case 141:
    {
        treenode *idNode = root->child->nextSibling->nextSibling;
        astNode *idAST = initASTNode(AST_ID, NULL);
        idAST->tk = idNode->tk;
        treenode *rangeFor = idNode->nextSibling->nextSibling;
        constructAST(rangeFor);
        treenode *stmts = rangeFor->nextSibling->nextSibling->nextSibling;
        constructAST(stmts);
        root->addr = initASTNode(AST_FOR, NULL);
        root->addr->leftChild = append_at_end(root->addr->leftChild, idAST);
        root->addr->leftChild = append_at_end(root->addr->leftChild, rangeFor->addr);
        root->addr->leftChild = append_at_end(root->addr->leftChild, stmts->addr);
        treenode *st = rangeFor->nextSibling->nextSibling;
        root->addr->line_start = st->tk->line_num;
        root->addr->line_end = st->nextSibling->nextSibling->tk->line_num;
        freeRHSList(root);
        return NULL;
    }
    case 142:
    {
        treenode *arOrB = getNodeWithSymbol(arithmeticOrBooleanExpr, root);
        treenode *stmts = arOrB->nextSibling->nextSibling->nextSibling;
        constructAST(arOrB);
        constructAST(stmts);
        root->addr = initASTNode(AST_WHILE, NULL);
        root->addr->tk = root->child->tk;
        root->addr->leftChild = append_at_end(root->addr->leftChild, arOrB->addr);
        root->addr->leftChild = append_at_end(root->addr->leftChild, stmts->addr);
        root->addr->line_start = arOrB->nextSibling->nextSibling->tk->line_num;
        root->addr->line_end = stmts->nextSibling->tk->line_num;
        freeRHSList(root);
        return NULL;
    }
    case 143:
    {
        treenode *signNode = root->child;
        treenode *indexFor = signNode->nextSibling;
        constructAST(signNode);
        constructAST(indexFor);
        if (signNode->addr == NULL || signNode->addr->tk->token == PLUS)
        {
            root->addr = indexFor->addr;
        }
        else
        {
            indexFor->addr->isNegative = true;
            root->addr = indexFor->addr;
            // astNode *signNode1 = initASTNode(AST_SIGNED, NULL);
            // signNode1->leftChild = append_at_end(signNode1->leftChild, signNode->addr);
            // signNode1->leftChild = append_at_end(signNode1->leftChild, indexFor->addr);
        }
        freeRHSList(root);
        return NULL;
    }
    case 144:
    {
        astNode *numNode = initASTNode(AST_NUM, NULL);
        numNode->tk = root->child->tk;
        root->addr = numNode;
        freeRHSList(root);
        return NULL;
    }
    case 145 ... 146:
    {
        astNode *node = initASTNode(AST_SIGN, NULL);
        node->tk = root->child->tk;
        root->addr = node;
        freeRHSList(root);
        return NULL;
    }
    case 147:
    {
        freeRHSList(root);
        return NULL;
    }
    case 148:
    {
        treenode *index1 = root->child;
        treenode *index2 = index1->nextSibling->nextSibling;
        constructAST(index1);
        constructAST(index2);
        astNode *node = initASTNode(AST_RANGE, NULL);
        node->leftChild = append_at_end(node->leftChild, index1->addr);
        node->leftChild = append_at_end(node->leftChild, index2->addr);
        root->addr = node;
        freeRHSList(root);
        return NULL;
    }
    }
}

void inorder_ast(astNode *root)
{
    if (root == NULL)
        return;
    inorder_ast(root->leftChild);
    if (root->label == AST_ID)
    {
        printf("str: %s", root->tk->str);
        // printf("line number at this token %d\n", root->tk->line_num);
    }
    printf("%s\n", EnumToASTString(root->label));
    if (root->leftChild != NULL)
    {
        astNode *leftOut = root->leftChild->nextSibling;
        while (leftOut != NULL)
        {
            inorder_ast(leftOut);
            leftOut = leftOut->nextSibling;
        }
    }
}

int countASTnodes(astNode *root)
{
    if (root == NULL)
        return 0;
    int count = 1;
    astNode *leftOut = root->leftChild;
    while (leftOut != NULL)
    {
        count += countASTnodes(leftOut);
        leftOut = leftOut->nextSibling;
    }
    return count;
}


// int main()
// {
//     FILE *fp = fopen("random.txt", "r");
//     twinbuffer *tb = twinbuffer_init(fp, 256);
//     fill_grammar(fopen("Grammar.txt", "r"));
//     hashtable ht = initHashtable();
//     populate_hashtable(&ht);
//     populateParseTable();
//     treenode *root = parseInputSourceCode(fp, tb, ht);
//     astNode *astRoot = constructAST(root);
//     inorder_ast(astRoot);
// }