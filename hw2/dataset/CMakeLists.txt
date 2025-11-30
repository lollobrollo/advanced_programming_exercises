add_library(dataset dataset.cpp)

# specify directory to include when compiling this library; public allows other files to see the header too
target_include_directories(dataset PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})