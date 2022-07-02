#include <stdarg.h>
#include <stdio.h>
#include <unity.h>

void test_fail_msg(char* fmt, ...)
{
    char errMsg[1024] = {0};
    va_list args;

    va_start(args, fmt);
    vsprintf((char*)&errMsg, fmt, args);
    va_end(args);

    TEST_FAIL_MESSAGE(errMsg);
}
