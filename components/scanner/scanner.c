#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

const char *TokenStrings[] = {
    "Unknown",
    "*",
    "/",
    "+",
    "-",
    "Integer"
};


// Prototypes -------------------------------------------------------------------------------------

bool init_scanner(PSCANNER pScanner, FILE* fd);
bool scan(PSCANNER pScanner, PTOKEN pToken);

char next_char(PSCANNER pScanner);
void read_number(PSCANNER pScanner, PTOKEN pToken, char firstChar);


// ------------------------------------------------------------------------------------------------
const char* token_name(PTOKEN pToken) {
    return TokenStrings[pToken->type];
}

// ------------------------------------------------------------------------------------------------
bool init_scanner(PSCANNER pScanner, FILE* fd) {
    pScanner->fd = fd;
    pScanner->putback = false;
    return true;
}

// ------------------------------------------------------------------------------------------------
char next_char(PSCANNER pScanner) {
    char nextChar;

    if(pScanner->putback){
        nextChar = pScanner->lastChar;
        pScanner->putback = false;
    } else {
        nextChar = fgetc(pScanner->fd);
    }

    while(
          ' ' == nextChar || '\t' == nextChar || '\n' == nextChar ||
          '\r' == nextChar || '\f' == nextChar
    ){
        nextChar = fgetc(pScanner->fd);
    }
    return nextChar;
}

// ------------------------------------------------------------------------------------------------
bool scan(PSCANNER pScanner, PTOKEN pToken) {
    char nextChar = next_char(pScanner);

    if(EOF == nextChar) {
        return false;
    }

    if isdigit(nextChar) {
        read_number(pScanner, pToken, nextChar);
        return true;
    }

    switch(nextChar) {
        case '*':
            pToken->type = TOKEN_STAR;
            break;
        case '/':
            pToken->type = TOKEN_SLASH;
            break;
        case '+':
            pToken->type = TOKEN_PLUS;
            break;
        case '-':
            pToken->type = TOKEN_MINUS;
            break;
        default:
            pToken->type = TOKEN_UNKNOWN;
            pToken->value = nextChar;
    }
    return true;
}

// ------------------------------------------------------------------------------------------------
void read_number(PSCANNER pScanner, PTOKEN pToken, char firstChar) {
    char nextChar;
    int pow = 10;

    pToken->type = TOKEN_INTEGER;
    pToken->value = firstChar - ASCII_NUM_START;
    
    do {
        nextChar = fgetc(pScanner->fd);
        if isdigit(nextChar) {
            pToken->value *= pow;
            pow *= 10;
            pToken->value += nextChar - ASCII_NUM_START;
        } else {
            pScanner->putback = true;
            pScanner->lastChar = nextChar;
            break;
        }
    } while(true);
    return;
}