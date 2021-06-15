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
typedef struct ASTNode *PAST;

struct ASTNode {
    AST_OP op;
    PASTNode left;
    PASTNode right;
    int value;
};

// Functions --------------------------------------------------------------------------------------

//Get the name of the current AST Operator
const char* ast_op_name(PAST pAST);

// Build an Abstract Styntax Tree
PAST build_ast(PSCANNER pScanner);

// Free an Abstract Styntax Tree
void free_ast(PAST pAST);

#endif /* PARSER_H */