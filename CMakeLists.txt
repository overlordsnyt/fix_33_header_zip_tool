cmake_minimum_required(VERSION 3.21)
project(fix_33zip VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
endif()

add_subdirectory(manga_lib)

add_executable(workflow main_flow.cpp)

target_link_libraries(workflow PUBLIC
        file_util
        check_header_util
        )
target_include_directories(workflow PUBLIC
        ${PROJECT_SOURCE_DIR}/manga_lib
)

add_executable(test
        test.cpp
        )

message("now compiler version: ${CMAKE_CXX_COMPILER_VERSION}")