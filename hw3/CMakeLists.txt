cmake_minimum_required(VERSION 3.10)
project(hw3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

find_package(Eigen3 REQUIRED)
find_package(muparserx REQUIRED)

add_subdirectory(src)