#ifndef COMPILER_H
#define COMPILER_H

#include <stdbool.h>
#include "parser.h"

typedef enum ARCHITECTURE {
    AMD64
} ARCHITECTURE, *PARCHITECTURE;



// Functions --------------------------------------------------------------------------------------

// Compile an AST
bool compile(PASTNode root, const char* fname);

#endif /* COMPILER_H */