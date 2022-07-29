#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

typedef enum AST_OP {
    AST_UNKNOWN = 0,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_ADD,
    AST_SUB,
    AST_INTEGER
} AST_OP, *PAST_OP;


typedef struct ASTNode ASTNode;
typedef struct ASTNode *PASTNode;

typedef struct ASTNode {
    AST_OP op;
    PASTNode lchild;
    PASTNode rchild;
    int value;
} ASTNode, *PASTNode;


// Functions --------------------------------------------------------------------------------------

PASTNode build_ast(PSCANNER pScanner);

int interpret_ast(PASTNode root);

void free_ast(PASTNode root);

AST_OP ast_op_fromtoken(TOKENTYPE token);

// Get the name of the current token
const char* ast_name(AST_OP op);

#endif /* PARSER_H */