cmake_minimum_required(VERSION 3.21)
project(manga_lib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(file_util STATIC
        file_util.cpp file_util.h
)

add_library(check_header_util STATIC
        check_header_util.cpp check_header_util.h
)

add_library(binary_manipulation
        binary_manipulation.cpp binary_manipulation.h
        )
