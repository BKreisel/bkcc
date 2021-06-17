#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stdio.h>

#define ASCII_NUM_START 0x30

typedef struct SCANNER {
    FILE* fd;
    char lastChar;
    bool putback;
    unsigned int lineNum;
} SCANNER, *PSCANNER;

typedef enum TOKENTYPE {
    TOKEN_UNKNOWN = 0,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_INTEGER
} TOKENTYPE, *PTOKENTYPE;


typedef struct TOKEN {
    TOKENTYPE type;
    int value;
} TOKEN, *PTOKEN;

// Functions --------------------------------------------------------------------------------------

// Initialize the Scanner with a file descriptor
bool init_scanner(PSCANNER pScanner, FILE* fd);

// Return tokens until EOF
bool scan(PSCANNER pScanner, PTOKEN pToken);

// Get the name of the current token
const char* token_name(PTOKEN pToken);


#endif /* SCANNER_H */