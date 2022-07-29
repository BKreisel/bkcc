#include <stdlib.h>
#include "error.h"
#include "parser.h"

// Prototypes -------------------------------------------------------------------------------------
PASTNode build_ast(PSCANNER pScanner);
PASTNode _build_ast(PSCANNER pScanner);
int interpret_ast(PASTNode root);
PASTNode build_node(AST_OP op, PASTNode left, PASTNode right, int value);
void free_ast(PASTNode root);
AST_OP ast_op_fromtoken(TOKENTYPE token);
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
    
    clear_error();
    n = _build_ast(pScanner);
    if(ERR_OK != get_error())
    {
        return NULL;
    }
    return n;
}
// ------------------------------------------------------------------------------------------------
PASTNode _build_ast(PSCANNER pScanner)
{
    PASTNode left, right;
    TOKEN token;
    AST_OP op;
    
    // Left
    scan(pScanner, &token);

    if(TOKEN_EOF == token.type) {
        return NULL;
    }
    op = ast_op_fromtoken(token.type);
    if(AST_INTEGER != op) {
        set_error(ERR_AST_OP, "Unexpected AST Operator: %s (%d). Wanted INT", ast_name(op), token.value);
        return NULL;
    }

    left = build_node(op, NULL, NULL, token.value);
    
    // Current
    scan(pScanner, &token);
    if(TOKEN_EOF == token.type) {
        return left;
    }
    op = ast_op_fromtoken(token.type);
    if(AST_UNKNOWN == op || AST_INTEGER == op) {
        set_error(ERR_AST_OP, "Bad AST Operator: %s (%d)", ast_name(op), token.value);
        return NULL;
    }

    //Right
    right = _build_ast(pScanner);
    
    return build_node(op, left, right, token.value);
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
