cmake_minimum_required(VERSION 3.10)
project("bkcc" C)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(HEADERS ${SRC}/include)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(src)

add_subdirectory(external)
include(CTest)
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
set(CMAKE_CTEST_ARGUMENTS "--stop-on-failure")
set(CMAKE_CTEST_ARGUMENTS "--verbose")
add_subdirectory(test)
