add_executable(bkcc
    main.c
    error.c
    scanner.c
    parser.c
)

target_include_directories(bkcc PRIVATE ${HEADERS})
