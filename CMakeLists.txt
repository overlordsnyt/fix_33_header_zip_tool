cmake_minimum_required(VERSION 3.21)
project(fix_33zip VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(manga_lib)

add_executable(workflow main_flow.cpp)

target_link_libraries(workflow PUBLIC
        file_util
        check_header_util
        binary_manipulation
        )
target_include_directories(workflow PUBLIC
        ${PROJECT_SOURCE_DIR}/manga_lib
)

add_executable(test
        test.cpp
        )