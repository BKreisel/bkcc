#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unity.h>
#include <sys/wait.h>

#include "compiler.h"
#include "parser.h"
#include "scanner.h"
#include "util.h"

// Globals ----------------------------------------------------------------------------------------
PASTNode pAST1, pAST2 = NULL;
SCANNER scan1, scan2 = {0};

// ------------------------------------------------------------------------------------------------
void setUp(void)
{
    scan1 = scanner_frompath("input/1.txt");
    pAST1 = build_ast(&scan1);

    scan2 = scanner_frompath("input/2.txt");
    pAST2 = build_ast(&scan2);
};

// ------------------------------------------------------------------------------------------------
void tearDown(void)
{
    free_ast(pAST1);
    close_scanner(&scan1);
    free_ast(pAST2);
    close_scanner(&scan2);
};

// ------------------------------------------------------------------------------------------------
int compile_and_run(PASTNode pAST)
{
    remove("test.asm");
    remove("test.o");
    remove("test.elf64");
    char* nasmCmd = "nasm -felf64 test.asm -o test.o";
    char* gccCmd = "gcc -static test.o -o test.elf64";
    
    char exePath[PATH_MAX] = {0};
    if(NULL == getcwd(exePath, PATH_MAX)) {
        TEST_FAIL_MESSAGE("Failed to get cwd.");
    }
    strcat(exePath, "/test.elf64");

    int status;

    compile(pAST, "test.asm");

    status = system(nasmCmd);
    if (status < 0 || 0 != WEXITSTATUS(status)) {
        test_fail_msg("NASM Failed: %s", nasmCmd);
    }

    status = system(gccCmd);
    if (status < 0 || 0 != WEXITSTATUS(status)) {
        test_fail_msg("GCC Failed: %s", gccCmd);
    }

    char buffer[10];
    
    FILE* fp = (FILE*)popen(exePath, "r");
    if(NULL == fp) {
        test_fail_msg("Failed to run: %s", exePath);
    }
    
    wait(&status);
    if(!WIFEXITED(status)) {
        pclose(fp);
        test_fail_msg("%s didn't exit correctly", exePath);
    }
    
    if(NULL == fgets(buffer, sizeof(buffer), fp)) {
        pclose(fp);
        test_fail_msg("Failed to read stdout of: %s", exePath);
    }
    
    pclose(fp);
    return atoi(buffer);
};

// ------------------------------------------------------------------------------------------------
void compile1(void)
{
    // 2 + 3 * 5 - 8 / 3
    TEST_ASSERT_EQUAL_INT(15, compile_and_run(pAST1));
}

// ------------------------------------------------------------------------------------------------
void compile2(void)
{
    /*
        13 -6+  4*
        5
            +
        08 / 3
    */
    TEST_ASSERT_EQUAL_INT(29, compile_and_run(pAST2));
}

// ------------------------------------------------------------------------------------------------
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(compile1);
    RUN_TEST(compile2);
    return UNITY_END();
}