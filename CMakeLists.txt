# Copyright (c) 2019 Martien Oppeneer. 
# This software is MIT licensed; see LICENSE.MIT. 

cmake_minimum_required(VERSION 3.5)

project(smm CXX)

add_executable(smm 
    src/smm.cpp
    src/telegram_generator.cpp)

add_subdirectory(ext_libs/Catch2)

add_executable(test_smm 
    test/tests_main.cpp
    test/test_smm.cpp)

target_link_libraries(test_smm Catch2::Catch2)
 
