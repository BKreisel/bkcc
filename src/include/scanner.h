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
    unsigned int colNum;
} SCANNER, *PSCANNER;

typedef enum TOKENTYPE {
    TOKEN_UNKNOWN = 0,
    TOKEN_EOF,
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

// Create a new scanner from a file pointer
SCANNER new_scanner(FILE* fd);

void close_scanner(PSCANNER pScanner);

// Return next token from the scanner.
void scan(PSCANNER pScanner, PTOKEN pToken);

// Get the name of the current token
const char* token_name(TOKENTYPE ttype);

#endif /* SCANNER_H */