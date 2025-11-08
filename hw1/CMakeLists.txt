cmake_minimum_required(VERSION 3.10)
project(hw1_sparse LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile options: enable warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Source files
set(SOURCES
  main.cpp
  sparse.cpp
  sparsecoo.cpp
  sparsecsr.cpp
)

add_executable(hw1 ${SOURCES})

target_include_directories(hw1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
