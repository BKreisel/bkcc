#include <stdlib.h>
#include "error.h"
#include "parser.h"

// Prototypes -------------------------------------------------------------------------------------
PASTNode _build_ast(PSCANNER pScanner, PTOKEN pToken, int prev_precedence);
PASTNode build_ast(PSCANNER pScanner);
int interpret_ast(PASTNode root);
PASTNode build_node(AST_OP op, PASTNode left, PASTNode right, int value);
void free_ast(PASTNode root);
AST_OP ast_op_fromtoken(TOKENTYPE token);
int ast_op_precedence(AST_OP op);
const char* ast_name(AST_OP op);

// ------------------------------------------------------------------------------------------------
PASTNode build_node(AST_OP op, PASTNode left, PASTNode right, int value)
{
    PASTNode node = calloc(1, sizeof(ASTNode));
    if(NULL != node)
    {
        node->op = op;
        node->lchild = left;
        node->rchild = right;
        node->value = value;
    } else {
        set_error(ERR_ALLOC, "calloc for an AST node failed");
        return NULL;
    }
    return node;
}

// ------------------------------------------------------------------------------------------------
void free_ast(PASTNode root)
{
    if(NULL == root)
    {
        return;
    }
    if(NULL != root->lchild)
    {
        free_ast(root->lchild);
    }
    if(NULL != root->rchild)
    {
        free_ast(root->rchild);
    }
    free(root);
}

// ------------------------------------------------------------------------------------------------
PASTNode build_ast(PSCANNER pScanner)
{
    PASTNode n;
    TOKEN token;
    scan(pScanner, &token);
    
    clear_error();
    n = _build_ast(pScanner, &token, 0);
    if(ERR_OK != get_error())
    {
        return NULL;
    }
    return n;
}
// ------------------------------------------------------------------------------------------------
PASTNode _build_ast(PSCANNER pScanner, PTOKEN pToken, int prev_precedence)
{
    PASTNode left, right;
    AST_OP op;
    int precedence;
    
    op = ast_op_fromtoken(pToken->type);
    if(AST_INTEGER != op) {
        set_error(ERR_AST_OP, "Unexpected AST operator: %s (%d). Wanted INT", ast_name(op), pToken->value);
        return NULL;
    }
    left = build_node(op, NULL, NULL, pToken->value);
    
    scan(pScanner, pToken);
    if(TOKEN_EOF == pToken->type) {
        return left;
    }

    op = ast_op_fromtoken(pToken->type);
    precedence = ast_op_precedence(op);
    if (0 == precedence)
    {
        set_error(ERR_AST_OP, "Unexpected AST operator: %s (%d).", ast_name(op), pToken->value);
        return NULL; 
    }

    while(precedence > prev_precedence) {
        scan(pScanner, pToken);

        right = _build_ast(pScanner, pToken, precedence);
        left = build_node(op, left, right, 0);

        op = ast_op_fromtoken(pToken->type);
        precedence = ast_op_precedence(op);
        if(TOKEN_EOF == pToken->type) {
           return left;
        }
        if (0 == precedence)
        {
            set_error(ERR_AST_OP, "Unexpected AST operator: %s (%d).", ast_name(op), pToken->value);
            return NULL; 
        }    
    }

    return left;
}

// ------------------------------------------------------------------------------------------------
int interpret_ast(PASTNode root)
{
     int left, right;

    if(NULL != root->lchild) {
        left = interpret_ast(root->lchild);
    }

    if(NULL != root->rchild) {
        right = interpret_ast(root->rchild);
    }

    switch (root->op) {
        case AST_MULTIPLY:
            return left * right;
        case AST_DIVIDE:
            return left / right;
        case AST_ADD:
            return left + right;
        case AST_SUB:
            return left - right;
        case AST_INTEGER:
            return root->value;
        
        default:
            set_error(ERR_AST_OP, "Bad AST Operator: %s (%d)", 
                      ast_name(root->op), root->value);
            return 0;
    }
}

// ------------------------------------------------------------------------------------------------
AST_OP ast_op_fromtoken(TOKENTYPE token)
{
    switch(token)
    {
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
    }
    return AST_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
int ast_op_precedence(AST_OP op)
{
    switch(op)
    {
        case AST_ADD:
        case AST_SUB:
            return 10;
        case AST_MULTIPLY:
        case AST_DIVIDE:
            return 20;
        default:
            return 0;
    }
}

// ------------------------------------------------------------------------------------------------
const char* ast_name(AST_OP op)
{
    switch(op)
    {
        case AST_MULTIPLY:
            return "AST_Multiply";
        case AST_DIVIDE:
            return "AST_Divide";
        case AST_ADD:
            return "AST_Add";
        case AST_SUB:
            return "AST_Subtract";
        case AST_INTEGER:
            return "AST_Integer";

    }
    return "Unknown";
}
