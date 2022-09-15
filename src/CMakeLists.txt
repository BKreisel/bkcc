add_executable(bkcc
    main.c
    amd64.c
    compiler.c
    error.c
    scanner.c
    parser.c
)

target_include_directories(bkcc PRIVATE ${HEADERS})
