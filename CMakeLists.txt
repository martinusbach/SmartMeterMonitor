# Copyright (c) 2019 Martien Oppeneer.
# This software is MIT licensed; see LICENSE.MIT.

cmake_minimum_required(VERSION 3.5)

project(smm CXX)


# Deamon, works only on linux
if (UNIX)
    add_executable(smm
        src/tty_telegram_generator.cpp
        src/smm.cpp)
endif()


# Test cases should work on windows as well
add_subdirectory(ext_libs/Catch2)

add_executable(test_smm
    test/file_telegram_generator.cpp
    test/tests_main.cpp
    test/test_smm.cpp)

configure_file(
    ${CMAKE_SOURCE_DIR}/telegrams.log
    ${CMAKE_CURRENT_BINARY_DIR}/telegrams.log
    COPYONLY)

target_link_libraries(test_smm Catch2::Catch2)
