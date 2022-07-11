#include<ctype.h>
#include<stdio.h>
#include "error.h"
#include "scanner.h"

// Prototypes -------------------------------------------------------------------------------------
SCANNER new_scanner(FILE* fd);
void close_scanner(PSCANNER pScanner);
void scan(PSCANNER pScanner, PTOKEN pToken);
const char* token_name(TOKENTYPE ttype);
char next_char(PSCANNER pScanner);
void read_number(PSCANNER pScanner, PTOKEN pToken, char firstChar);

// ------------------------------------------------------------------------------------------------
SCANNER new_scanner(FILE* fd)
{
    SCANNER s = {
        .fd = fd,
        .putback = false,
        .lineNum = 1
    };
    return s;
}

// ------------------------------------------------------------------------------------------------
void close_scanner(PSCANNER pScanner)
{
    fclose(pScanner->fd);
}

// ------------------------------------------------------------------------------------------------
char next_char(PSCANNER pScanner)
{
    char nextChar;

    if(pScanner->putback){
        nextChar = pScanner->lastChar;
        pScanner->putback = false;
    } else {
        nextChar = fgetc(pScanner->fd);
        pScanner->colNum++;
    }

    if('\n' == nextChar) {
        pScanner->lineNum++;
        pScanner->colNum = 0;
    }

    while(
          ' ' == nextChar || '\t' == nextChar || '\n' == nextChar ||
          '\r' == nextChar || '\f' == nextChar
    ){
        nextChar = fgetc(pScanner->fd);
        pScanner->colNum++;
    }
    return nextChar;
}

// ------------------------------------------------------------------------------------------------
void read_number(PSCANNER pScanner, PTOKEN pToken, char firstChar)
{
    char nextChar;

    pToken->type = TOKEN_INTEGER;
    pToken->value = firstChar - ASCII_NUM_START;
    
    do {
        nextChar = fgetc(pScanner->fd);
        pScanner->colNum++;
        if (isdigit(nextChar)) {
            pToken->value *= 10;
            pToken->value += nextChar - ASCII_NUM_START;
        } else {
            pScanner->putback = true;
            pScanner->lastChar = nextChar;
            break;
        }
    } while(true);
    return;
}

// ------------------------------------------------------------------------------------------------
void scan(PSCANNER pScanner, PTOKEN pToken)
{
    char nextChar = next_char(pScanner);

    if (isdigit(nextChar)) {
        read_number(pScanner, pToken, nextChar);
        return;
    }

    switch(nextChar) {
        case EOF:
            pToken->type = TOKEN_EOF;
            break;
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
            set_error(ERR_TOKEN,
                "Line %d, Col %d. Token: '%s'",
                pScanner->lineNum,
                pScanner->colNum,
                nextChar
            );
            pToken->type = TOKEN_UNKNOWN;
            pToken->value = nextChar;
    }
    return;
}

// ------------------------------------------------------------------------------------------------
const char* token_name(TOKENTYPE ttype)
{
    switch(ttype)
    {
        case TOKEN_EOF:
            return "EOF";
        case TOKEN_STAR:
            return "*";
        case TOKEN_SLASH:
            return "/";
        case TOKEN_PLUS:
            return "+";
        case TOKEN_MINUS:
            return "-";
        case TOKEN_INTEGER:
            return "INT";

    }
    return "Unknown";
}
