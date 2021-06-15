#include <stdbool.h>
#include <stdio.h>
#include "scanner.h"
#include "utils.h"


// ------------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    FILE* fd = NULL;
    int retval = RET_OK;
    SCANNER scanner = {};
    TOKEN token = {};

    if(argc != 2) {
        THROW("usage: main <infile>\n");
        retval = RET_ERR;
    }

    fd = fopen(argv[1], "r");
    if(NULL == fd) {
        THROW("Failed to open %s\n", argv[1]);
        retval = RET_ERR;
    }

    if(!init_scanner(&scanner, fd)) {
        THROW("Scanner initialization failed.");
        retval = RET_ERR;
    }

    printf("Scanner:\n\n");

    while(scan(&scanner, &token)) {
        printf("Got Token: %s", token_name(&token));

        if(TOKEN_UNKNOWN == token.type) {
            printf("\t(%c)\n", token.value);
            break;
        }
        if(TOKEN_INTEGER == token.type) {
            printf("\t(%d)", token.value);
        }

        printf("\n");
    }
    

    cleanup:
        if(NULL != fd) {
            fclose(fd);
            fd = NULL;
        }
        return retval;
}