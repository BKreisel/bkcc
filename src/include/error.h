#ifndef ERROR_H
#define ERROR_H

typedef enum ERROR {
    ERR_OK = 0,

    // Generic
    ERR_ALLOC,

    // Scanner
    ERR_TOKEN,

    // Parser
    ERR_AST_OP,

} ERROR, *PERROR;

// Functions --------------------------------------------------------------------------------------
unsigned int get_error();

const char* error_name(ERROR err);

void set_error(ERROR err, char* fmt, ...);

void print_error();

#endif /* ERROR_H */