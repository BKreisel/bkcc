add_executable(test_scanner
    test_scanner.c
    util.c
    ${SRC}/error.c
    ${SRC}/scanner.c
)
target_include_directories(test_scanner PRIVATE ${HEADERS} util.h)
target_link_libraries(test_scanner Unity)

add_test(NAME scanner COMMAND test_scanner)

add_executable(test_parser
    test_parser.c
    util.c
    ${SRC}/error.c
    ${SRC}/parser.c
    ${SRC}/scanner.c
)
target_include_directories(test_parser PRIVATE ${HEADERS} util.h)
target_link_libraries(test_parser Unity)

add_test(NAME parser COMMAND test_parser)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/test/input ${CMAKE_CURRENT_BINARY_DIR}/input SYMBOLIC)
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/test/input ${CMAKE_CURRENT_BINARY_DIR}/Debug/input SYMBOLIC)
