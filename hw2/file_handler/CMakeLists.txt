add_library(file_handler file_handler.cpp)

# specify directory to include when compiling this library; public allows other files to see the header too
target_include_directories(file_handler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})