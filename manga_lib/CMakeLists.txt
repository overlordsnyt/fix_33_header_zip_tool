cmake_minimum_required(VERSION 3.21)
project(manga_lib VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(read_files STATIC
        read_files.cpp read_files.h
)

add_library(check_file_magic STATIC
        check_file_magic.cpp check_file_magic.h
)
