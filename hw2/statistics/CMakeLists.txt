add_library(statistics statistics.cpp)

# link against Dataset
target_link_libraries(statistics dataset)

# allow this library to be included by other files
target_include_directories(statistics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# compile options
target_compile_features(statistics PUBLIC cxx_std_17)
target_compile_options(statistics PRIVATE -Wall -Wextra -Wpedantic -Werror)