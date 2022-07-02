#include <stdio.h>
#include <unity.h>
#include "scanner.h"
#include "util.h"

void setUp(void) {};
void tearDown(void) {};

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

void assert_token(PSCANNER pScanner, TOKENTYPE tokType)
{
    TOKEN tok = {0};   
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(tokType, tok.type);
}

void assert_eof(PSCANNER pScanner)
{
    TOKEN tok = {0};
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(TOKEN_EOF, tok.type);
}

void assert_err(PSCANNER pScanner)
{
    TOKEN tok = {0};
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(TOKEN_UNKNOWN, tok.type);
}

void assert_int(PSCANNER pScanner, int value)
{
    TOKEN tok = {0};
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(TOKEN_INTEGER, tok.type);
    TEST_ASSERT_EQUAL_UINT(value, tok.value);
}

void scan1(void)
{
    SCANNER scanner = scanner_frompath("input/scanner1.txt");
    
    //  2 + 3 * 5 - 8 / 3
    assert_int(&scanner, 2);
    assert_token(&scanner, TOKEN_PLUS);
    assert_int(&scanner, 3);
    assert_token(&scanner, TOKEN_STAR);
    assert_int(&scanner, 5);
    assert_token(&scanner, TOKEN_MINUS);
    assert_int(&scanner, 8);
    assert_token(&scanner, TOKEN_SLASH);
    assert_int(&scanner, 3);
    assert_eof(&scanner);
}

void scan2(void)
{
    SCANNER scanner = scanner_frompath("input/scanner2.txt");
    
    /*
        13 -6+  4*
        5
            +
        08 / 3
    */
    assert_int(&scanner, 13);
    assert_token(&scanner, TOKEN_MINUS);
    assert_int(&scanner, 6);
    assert_token(&scanner, TOKEN_PLUS);
    assert_int(&scanner, 4);
    assert_token(&scanner, TOKEN_STAR);
    assert_int(&scanner, 5);
    assert_token(&scanner, TOKEN_PLUS);
    assert_int(&scanner, 8);
    assert_token(&scanner, TOKEN_SLASH);
    assert_int(&scanner, 3);
    assert_eof(&scanner);
}

void scan3(void)
{
    SCANNER scanner = scanner_frompath("input/scanner3.txt");
    
    //  12 34 + -56 * / - - 8 + * 2
    assert_int(&scanner, 12);
    assert_int(&scanner, 34);
    assert_token(&scanner, TOKEN_PLUS);
    assert_token(&scanner, TOKEN_MINUS);
    assert_int(&scanner, 56);
    assert_token(&scanner, TOKEN_STAR);
    assert_token(&scanner, TOKEN_SLASH);
    assert_token(&scanner, TOKEN_MINUS);
    assert_token(&scanner, TOKEN_MINUS);
    assert_int(&scanner, 8);
    assert_token(&scanner, TOKEN_PLUS);
    assert_token(&scanner, TOKEN_STAR);
    assert_int(&scanner, 2);
    assert_eof(&scanner);
}

void scan4(void)
{
    SCANNER scanner = scanner_frompath("input/scanner4.txt");
    
    /*
        23 +
        18 -
        45.6 * 2
        / 18
    */
    assert_int(&scanner, 23);
    assert_token(&scanner, TOKEN_PLUS);
    assert_int(&scanner, 18);
    assert_token(&scanner, TOKEN_MINUS);
    assert_int(&scanner, 45);
    assert_err(&scanner);

}

void scan5(void)
{
    SCANNER scanner = scanner_frompath("input/scanner5.txt");
    
    //  23 * 456abcdefg
    assert_int(&scanner, 23);
    assert_token(&scanner, TOKEN_STAR);
    assert_int(&scanner, 456);
    assert_err(&scanner);
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(scan1);
    RUN_TEST(scan2);
    RUN_TEST(scan3);
    RUN_TEST(scan4);
    RUN_TEST(scan5);
    return UNITY_END();
}