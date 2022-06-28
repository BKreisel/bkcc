add_executable(test_scanner scanner.c)

target_link_libraries(test_scanner Unity)

add_test(scanner test_scanner)