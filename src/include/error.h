#ifndef ERROR_H
#define ERROR_H

typedef enum ERROR {
    ERR_OK = 0,

    // Generic
    ERR_ALLOC,
    ERR_FOPEN,

    // Scanner
    ERR_TOKEN,

    // Parser
    ERR_AST_OP,

    // Compiler
    ERR_COMPILER,

} ERROR, *PERROR;

// Functions --------------------------------------------------------------------------------------

// Get the last set error code
unsigned int get_error();

// Get the string name of an error code
const char* error_name(ERROR err);

// Set the error code with optional message
void set_error(ERROR err, char* fmt, ...);

// Reset the Error var to OK.
void clear_error();

// Print the current formatted error to stderr. 
void print_error();

#endif /* ERROR_H */