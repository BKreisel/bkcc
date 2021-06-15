#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "scanner.h"
#include "utils.h"

int interpret(PASTNode pAST) {
    int lval, rval = 0;
    
    if(NULL != pAST->left) {
        lval = interpret(pAST->left);
    }
    if(NULL != pAST->right) {
        rval = interpret(pAST->right);
    }

    switch(pAST->op) {
        case AST_MULTIPLY:
            DEBUG_PRINT("%d * %d", lval, rval);
            return (lval * rval);
        case AST_DIVIDE:
            DEBUG_PRINT("%d / %d", lval, rval);
            return (lval / rval);
        case AST_ADD:
            DEBUG_PRINT("%d + %d", lval, rval);
            return (lval + rval);
        case AST_SUB:
            DEBUG_PRINT("%d - %d", lval, rval);
            return (lval - rval);
        case AST_INTEGER:
            DEBUG_PRINT("INT %d", pAST->value);
            return pAST->value;
        case AST_UNKNOWN:
            printf("Failed to interpret AST.\n");
            exit(1);
    }

    //never reached.
    return 0;
}

// ------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    FILE* fd = NULL;
    int retval = RET_OK;
    SCANNER scanner = {};
    PAST pAST = NULL;
    int result = 0;

    if(argc != 2) {
        THROW("usage: main <infile>\n");
        retval = RET_ERR;
    }

    fd = fopen(argv[1], "r");
    if(NULL == fd) {
        THROW("Failed to open %s\n", argv[1]);
        retval = RET_ERR;
    }

    if(!init_scanner(&scanner, fd)) {
        THROW("Scanner initialization failed.");
        retval = RET_ERR;
    }

    printf("Parser:\n\n");

    pAST = build_ast(&scanner);
    if(NULL == pAST) {
        THROW("Failed to build AST");
        retval = RET_ERR;
    }

    result = interpret(pAST);

    printf("Result: %d\n", result);

    cleanup:
        if(NULL != fd) {
            fclose(fd);
            fd = NULL;
        }
        if(NULL != pAST) {
            free_ast(pAST);
            pAST = NULL;
        }
        return retval;
}