cmake_minimum_required(VERSION 3.21)
project(fix_33zip VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(manga_lib)

add_executable(fix_33zip main.cpp)

target_link_libraries(fix_33zip PUBLIC
        manga_lib)
target_include_directories(fix_33zip PUBLIC
        ${PROJECT_SOURCE_DIR}/manga_lib
)
