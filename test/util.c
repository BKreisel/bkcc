#include <stdarg.h>
#include <stdio.h>
#include <unity.h>

#include "scanner.h"

// Prototypes -------------------------------------------------------------------------------------
void test_fail_msg(char* fmt, ...);
SCANNER scanner_frompath(const char* path);


// ------------------------------------------------------------------------------------------------
void test_fail_msg(char* fmt, ...)
{
    char errMsg[1024] = {0};
    va_list args;

    va_start(args, fmt);
    vsprintf((char*)&errMsg, fmt, args);
    va_end(args);

    TEST_FAIL_MESSAGE(errMsg);
}

// ------------------------------------------------------------------------------------------------
SCANNER scanner_frompath(const char* path)
{
    FILE* input = fopen(path, "r");
    if (!input)
    {
        test_fail_msg("Failed to read input file: '%s'", path);
    }
    
    SCANNER s = new_scanner(input);
    return s;
}