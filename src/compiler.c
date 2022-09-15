#include <stdbool.h>
#include <stdio.h>
#include "amd64.h"
#include "compiler.h"
#include "error.h"


// Prototypes -------------------------------------------------------------------------------------
bool compile(PASTNode root, const char* fname);

// ------------------------------------------------------------------------------------------------
bool compile(PASTNode root, const char* fname)
{
    ARCHITECTURE arch = AMD64;

    switch(arch) {
        case AMD64:
            return amd64_compile(root, fname);
        default:
            set_error(ERR_COMPILER, "Unknown Architecture: %d", arch);
            return false;
    }
}
