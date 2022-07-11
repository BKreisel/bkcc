add_executable(bkcc
    main.c
    error.c
    scanner.c
)

target_include_directories(bkcc PRIVATE ${HEADERS})
