add_executable(test_scanner
    test_scanner.c
    util.c
    ${SRC}/scanner.c
)
target_include_directories(test_scanner PRIVATE ${HEADERS} util.h)
target_link_libraries(test_scanner Unity)

add_test(NAME scanner COMMAND test_scanner)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/test/input ${CMAKE_CURRENT_BINARY_DIR}/input SYMBOLIC)