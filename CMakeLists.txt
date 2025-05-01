set(CMAKE_C_COMPILER gcc)

set(CMAKE_CXX_COMPILER )cmake_minimum_required(VERSION 3.10)

project(Float-Lib)

set(CMAKE_C_STANDARD 99)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
    src/float.c
    test/test.c
    )

add_executable(test_program ${SOURCE_FILES})

target_compile_options(test_program PRIVATE -Wall -lm)

enable_testing()

add_test(NAME Float_tests COMMAND test_program)