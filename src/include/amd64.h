#ifndef AMD64_H
#define AMD64_H

#include <stdbool.h>
#include "parser.h"

#define AMD64_GEN_REG_COUNT 13
typedef enum AMD64_GEN_REG {
    RAX = 0,
    RBX,
    RCX,
    RDX,
    RBP,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
} AMD64_GEN_REG, *PAMD64_GEN_REG;

typedef struct AMD64_CTX {
    FILE* fd;
    bool reg_state[AMD64_GEN_REG_COUNT];
} AMD64_CTX, *PAMD64_CTX;

// Functions --------------------------------------------------------------------------------------

// Compile an AST.
bool amd64_compile(PASTNode root, const char* fname);

#endif /* AMD64_H */