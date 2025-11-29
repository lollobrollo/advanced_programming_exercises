add_executable(main main.cpp)

# link all used libraries to this executable
target_link_libraries(main file_handler statistics integration)