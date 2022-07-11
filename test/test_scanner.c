#include <stdio.h>
#include <unity.h>
#include "scanner.h"

#include "util.h"

// Globals ----------------------------------------------------------------------------------------
SCANNER scanner1, scanner2, scanner3, scanner4, scanner5 = {0};

// ------------------------------------------------------------------------------------------------
void setUp(void)
{
    scanner1 = scanner_frompath("input/1.txt");
    scanner2 = scanner_frompath("input/2.txt");
    scanner3 = scanner_frompath("input/3.txt");
    scanner4 = scanner_frompath("input/4.txt");
    scanner5 = scanner_frompath("input/5.txt");
};

// ------------------------------------------------------------------------------------------------
void tearDown(void) {
    close_scanner(&scanner1);
    close_scanner(&scanner2);
    close_scanner(&scanner3);
    close_scanner(&scanner4);
    close_scanner(&scanner5);
};

// ------------------------------------------------------------------------------------------------
void assert_token(PSCANNER pScanner, TOKENTYPE tokType)
{
    TOKEN tok = {0};   
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(tokType, tok.type);
}

// ------------------------------------------------------------------------------------------------
void assert_eof(PSCANNER pScanner)
{
    TOKEN tok = {0};
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(TOKEN_EOF, tok.type);
}

// ------------------------------------------------------------------------------------------------
void assert_err(PSCANNER pScanner)
{
    TOKEN tok = {0};
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(TOKEN_UNKNOWN, tok.type);
}

// ------------------------------------------------------------------------------------------------
void assert_int(PSCANNER pScanner, int value)
{
    TOKEN tok = {0};
    scan(pScanner, &tok);
    TEST_ASSERT_EQUAL_UINT(TOKEN_INTEGER, tok.type);
    TEST_ASSERT_EQUAL_UINT(value, tok.value);
}

// ------------------------------------------------------------------------------------------------
void scan1(void)
{
    //  2 + 3 * 5 - 8 / 3
    assert_int(&scanner1, 2);
    assert_token(&scanner1, TOKEN_PLUS);
    assert_int(&scanner1, 3);
    assert_token(&scanner1, TOKEN_STAR);
    assert_int(&scanner1, 5);
    assert_token(&scanner1, TOKEN_MINUS);
    assert_int(&scanner1, 8);
    assert_token(&scanner1, TOKEN_SLASH);
    assert_int(&scanner1, 3);
    assert_eof(&scanner1);
    assert_eof(&scanner1);
}

// ------------------------------------------------------------------------------------------------
void scan2(void)
{
    /*
        13 -6+  4*
        5
            +
        08 / 3
    */
    assert_int(&scanner2, 13);
    assert_token(&scanner2, TOKEN_MINUS);
    assert_int(&scanner2, 6);
    assert_token(&scanner2, TOKEN_PLUS);
    assert_int(&scanner2, 4);
    assert_token(&scanner2, TOKEN_STAR);
    assert_int(&scanner2, 5);
    assert_token(&scanner2, TOKEN_PLUS);
    assert_int(&scanner2, 8);
    assert_token(&scanner2, TOKEN_SLASH);
    assert_int(&scanner2, 3);
    assert_eof(&scanner2);
}

// ------------------------------------------------------------------------------------------------
void scan3(void)
{
        //  12 34 + -56 * / - - 8 + * 2
    assert_int(&scanner3, 12);
    assert_int(&scanner3, 34);
    assert_token(&scanner3, TOKEN_PLUS);
    assert_token(&scanner3, TOKEN_MINUS);
    assert_int(&scanner3, 56);
    assert_token(&scanner3, TOKEN_STAR);
    assert_token(&scanner3, TOKEN_SLASH);
    assert_token(&scanner3, TOKEN_MINUS);
    assert_token(&scanner3, TOKEN_MINUS);
    assert_int(&scanner3, 8);
    assert_token(&scanner3, TOKEN_PLUS);
    assert_token(&scanner3, TOKEN_STAR);
    assert_int(&scanner3, 2);
    assert_eof(&scanner3);
}

// ------------------------------------------------------------------------------------------------
void scan4(void)
{
    /*
        23 +
        18 -
        45.6 * 2
        / 18
    */
    assert_int(&scanner4, 23);
    assert_token(&scanner4, TOKEN_PLUS);
    assert_int(&scanner4, 18);
    assert_token(&scanner4, TOKEN_MINUS);
    assert_int(&scanner4, 45);
    assert_err(&scanner4);
}

// ------------------------------------------------------------------------------------------------
void scan5(void)
{
    //  23 * 456abcdefg
    assert_int(&scanner5, 23);
    assert_token(&scanner5, TOKEN_STAR);
    assert_int(&scanner5, 456);
    assert_err(&scanner5);
}

// ------------------------------------------------------------------------------------------------
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
