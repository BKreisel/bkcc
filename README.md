# bkcc
Minimal C Compiler



## Requirements
* WSL2 or Linux OS
```
aptitude install gcc cmake build-essential nasm gdb
```

## Build
```
cmake -B build .
cmake --build build
```

## Test
```
ctest -j8 -C Debug -T test --output-on-failure --test-dir build
```
