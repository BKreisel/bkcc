cmake_minimum_required(VERSION 3.10)

project("bkcc" C)

include(CTest)
add_subdirectory(external)
add_subdirectory(test)
