#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>

#define RET_ERR 1
#define RET_OK 0

#define THROW(...) {\
    printf("Error: "); \
    printf(__VA_ARGS__); \
    printf("\n"); \
    goto cleanup; \
}

#ifndef DEBUG
#define DEBUG_PRINT(...)
#else
#define DEBUG_PRINT(...) printf(__VA_ARGS__); printf("\n")
#endif

#endif /* UTILS_H */