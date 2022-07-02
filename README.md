# bkcc
C Compiler

## Build
```
cmake -B build .
cmake --build build
```

## Test
```
ctest --test-dir build --output-on-failure -C Debug
```
