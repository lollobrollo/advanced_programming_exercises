cmake_minimum_required(VERSION 3.10)
project(hw2 LANGUAGES CXX)

enable_testing()

# add directories containing project modules
add_subdirectory(dataset)
add_subdirectory(statistics)
add_subdirectory(integration)
add_subdirectory(main)
