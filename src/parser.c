#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "scanner.h"
#include "utils.h"

const char *ASTOperatorStrings[] = {
    "Unknown",
    "Multiply",
    "Divide",
    "Add",
    "Subtract",
    "Integer"
};

// Prototypes -------------------------------------------------------------------------------------
const char* ast_op_name(PAST pAST);
PAST build_ast(PSCANNER pScanner);
void free_ast(PAST pAST);
PASTNode build_node(AST_OP op, PASTNode left, PASTNode right, int value);
AST_OP op_fromtoken(TOKENTYPE type);

// ------------------------------------------------------------------------------------------------
const char* ast_op_name(PAST pAST) {
    return ASTOperatorStrings[pAST->op];
}

// ------------------------------------------------------------------------------------------------
PAST build_ast(PSCANNER pScanner) {
    PASTNode left, right = NULL;
    AST_OP op = AST_UNKNOWN;
    int rootVal = 0;
    TOKEN token = {};

    if(!scan(pScanner, &token)) {
        THROW("Syntax Error on line %d: Unexpected EOF\n", pScanner->lineNum);
    }

    if(TOKEN_INTEGER == token.type) {
        left = build_node(op_fromtoken(token.type), NULL, NULL, token.value);
    } else {
        THROW("Syntax Error on line %d: expression must start with integer.", pScanner->lineNum);
    }

    if(!scan(pScanner, &token)) {
        return left;
    }

    op = op_fromtoken(token.type);
    rootVal = token.value;

    if(AST_UNKNOWN == op) {
        THROW("Syntax Error on line %d: Unexpected AST Operator: %c", pScanner->lineNum, (char)token.value);
    }

    right = build_ast(pScanner);

    return build_node(op, left, right, rootVal);

    cleanup:
        exit(1);
}

// ------------------------------------------------------------------------------------------------
AST_OP op_fromtoken(TOKENTYPE type) {
    switch(type) {
        case TOKEN_STAR:
            return AST_MULTIPLY;
        case TOKEN_SLASH:
            return AST_DIVIDE;
        case TOKEN_PLUS:
            return AST_ADD;
        case TOKEN_MINUS:
            return AST_SUB;
        case TOKEN_INTEGER:
            return AST_INTEGER;
        default:
            return AST_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
PASTNode build_node(AST_OP op, PASTNode left, PASTNode right, int value) {
    PASTNode pNode = calloc(1, sizeof(ASTNode));
    DEBUG_PRINT("Allocated AST Node: [ADDR: 0x%p] [OP: %s] [LEFT: 0x%p] [RIGHT: 0x%p] [VALUE: %d]",
                pNode, ASTOperatorStrings[op], left, right, value);
    if(NULL == pNode) {
        DEBUG_PRINT("Memory Allocation Failure\n");
        return NULL;
    }
    pNode->op = op;
    pNode->left = left;
    pNode->right = right;
    pNode->value = value;

    return pNode;
}

// ------------------------------------------------------------------------------------------------
void free_ast(PAST pAST) {
    DEBUG_PRINT("Freed AST Node: [ADDR: 0x%p] [OP: %s] [VALUE: %d]",
                pAST, ast_op_name(pAST), pAST->value);

    if(NULL != pAST->left) {
        free_ast(pAST->left);
    }
    if(NULL != pAST->right) {
        free_ast(pAST->right);
    }
    free(pAST);
    return;
}