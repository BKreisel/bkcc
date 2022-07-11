#ifndef TEST_UTIL_H
#define TEST_UTIL_H

// Fail the Unit Test with a printf formatted msg.
void test_fail_msg(char* fmt, ...);

// Instantiate a Scanner from a path
SCANNER scanner_frompath(const char* path);

#endif /* TEST_UTIL_H */