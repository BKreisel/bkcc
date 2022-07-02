add_executable(bkcc
    main.c
    scanner.c
)

target_include_directories(bkcc PRIVATE ${HEADERS})
