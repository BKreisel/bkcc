#include <stdio.h>
#include <unity.h>
#include "parser.h"
#include "scanner.h"
#include "util.h"

// Globals ----------------------------------------------------------------------------------------
PASTNode pAST1, pAST2, pAST3, pAST4, pAST5 = NULL;
SCANNER scan1, scan2, scan3, scan4, scan5 = {0};

// ------------------------------------------------------------------------------------------------
void setUp(void)
{
    scan1 = scanner_frompath("input/1.txt");
    pAST1 = build_ast(&scan1);

    scan2 = scanner_frompath("input/2.txt");
    pAST2 = build_ast(&scan2);

    scan3 = scanner_frompath("input/3.txt");
    pAST3 = build_ast(&scan3);

    scan4 = scanner_frompath("input/4.txt");
    pAST4 = build_ast(&scan4);

    scan5 = scanner_frompath("input/5.txt");
    pAST5 = build_ast(&scan5);

};

// ------------------------------------------------------------------------------------------------
void tearDown(void)
{
    free_ast(pAST1);
    close_scanner(&scan1);
    free_ast(pAST2);
    close_scanner(&scan2);
    free_ast(pAST3);
    close_scanner(&scan3);
    free_ast(pAST4);
    close_scanner(&scan4);
    free_ast(pAST5);
    close_scanner(&scan5);
};

// ------------------------------------------------------------------------------------------------
void struct1(void)
{
    // 2 + 3 * 5 - 8 / 3
    PASTNode root = pAST1;
    PASTNode pNode = root;
    /*
             -
            /  \
           /    \
          +     '/' 
         / \    / \
        2   *  8   3
           / \    
          3   5
    */

    TEST_ASSERT_EQUAL_UINT(AST_SUB, root->op);

    // Left
    pNode = root->lchild;
    TEST_ASSERT_EQUAL_UINT(AST_ADD, pNode->op);
    TEST_ASSERT_EQUAL_UINT(2, pNode->lchild->value);
    pNode = pNode->rchild;
    TEST_ASSERT_EQUAL_UINT(AST_MULTIPLY, pNode->op);
    TEST_ASSERT_EQUAL_INT(3, pNode->lchild->value);
    TEST_ASSERT_EQUAL_INT(5, pNode->rchild->value);
    
    //Right
    pNode = root->rchild;
    TEST_ASSERT_EQUAL_UINT(AST_DIVIDE, pNode->op);
    TEST_ASSERT_EQUAL_INT(8, pNode->lchild->value);
    TEST_ASSERT_EQUAL_INT(3, pNode->rchild->value);
}

// ------------------------------------------------------------------------------------------------
void parse1(void)
{
    // 2 + 3 * 5 - 8 / 3
    TEST_ASSERT_EQUAL_INT(15, interpret_ast(pAST1));
}

// ------------------------------------------------------------------------------------------------
void struct2(void)
{
    /*
        13 -6+  4*
        5
            +
        08 / 3
    */
    PASTNode root = pAST2;
    PASTNode pNode = root;
    /*
                 +
                /  \
               /    \
              /      \
             +      '/'
            / \     / \
           /   \   8   3
          -     *
         / \   / \    
        13  6 4   5
    */
    
    TEST_ASSERT_EQUAL_UINT(AST_ADD, root->op);

    // Left
    pNode = root->lchild;
    TEST_ASSERT_EQUAL_UINT(AST_ADD, pNode->op);
    TEST_ASSERT_EQUAL_UINT(AST_SUB, pNode->lchild->op);
    TEST_ASSERT_EQUAL_UINT(13, pNode->lchild->lchild->value);
    TEST_ASSERT_EQUAL_UINT(6, pNode->lchild->rchild->value);
    TEST_ASSERT_EQUAL_UINT(AST_MULTIPLY, pNode->rchild->op);
    TEST_ASSERT_EQUAL_UINT(4, pNode->rchild->lchild->value);
    TEST_ASSERT_EQUAL_UINT(5, pNode->rchild->rchild->value);

    //Right
    pNode = root->rchild;
    TEST_ASSERT_EQUAL_UINT(AST_DIVIDE, pNode->op);
    TEST_ASSERT_EQUAL_INT(8, pNode->lchild->value);
    TEST_ASSERT_EQUAL_INT(3, pNode->rchild->value);
}

// ------------------------------------------------------------------------------------------------
void parse2(void)
{
    /*
        13 -6+  4*
        5
            +
        08 / 3
    */
    TEST_ASSERT_EQUAL_INT(29, interpret_ast(pAST2));
}

// ------------------------------------------------------------------------------------------------
void struct3(void)
{
    // 12 34 + -56 * / - - 8 + * 2
    PASTNode pNode = pAST3;
    
    TEST_ASSERT_NULL(pAST3);
}

// ------------------------------------------------------------------------------------------------
void struct4(void)
{
    /*
        23 +
        18 -
        45.6 * 2
        / 18
    */
    PASTNode pNode = pAST4;
    
    TEST_ASSERT_NULL(pAST4);
}

// ------------------------------------------------------------------------------------------------
void struct5(void)
{
    // 23 * 456abcdefg
    PASTNode pNode = pAST5;
    
    TEST_ASSERT_NULL(pAST5);
}

// ------------------------------------------------------------------------------------------------
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(struct1);
    RUN_TEST(parse1);
    RUN_TEST(struct2);
    RUN_TEST(parse2);
    RUN_TEST(struct3);
    RUN_TEST(struct4);
    RUN_TEST(struct5);
    return UNITY_END();
}