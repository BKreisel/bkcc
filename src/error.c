#include <stdarg.h>
#include <stdio.h>
#include "error.h"

#define ERR_BUFSIZE 512

// Internal Globals -------------------------------------------------------------------------------
ERROR _err = ERR_OK;
char* _errmsg[ERR_BUFSIZE] = {0};

// Prototypes -------------------------------------------------------------------------------------
unsigned int get_error();
void set_error(ERROR err, char* fmt, ...);
void clear_error();
void print_error();
const char* error_name(ERROR err);

// ------------------------------------------------------------------------------------------------
unsigned int get_error()
{
    return _err;
}
// ------------------------------------------------------------------------------------------------
void set_error(ERROR err, char* fmt, ...)
{  
    _err = err;
    va_list args;
    va_start(args, fmt);
    vsnprintf((char*)_errmsg, ERR_BUFSIZE, fmt, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void clear_error()
{  
    _err = ERR_OK;
}

// ------------------------------------------------------------------------------------------------
void print_error()
{
    char* fmt = "Error %s: %s\n";
    fprintf(stderr, fmt, error_name(_err), _errmsg);
}

// ------------------------------------------------------------------------------------------------
const char* error_name(ERROR err)
{
    switch(_err)
    {
        case ERR_ALLOC:
            return "Memory Allocation";
        case ERR_TOKEN:
            return "Bad Token";
        case ERR_AST_OP:
            return "Bad AST Operand";
    }
    return "Unknown Error type";
}
