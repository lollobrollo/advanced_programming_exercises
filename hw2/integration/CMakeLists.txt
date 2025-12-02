add_library(integration integration.cpp)

target_link_libraries(integration dataset)

target_include_directories(integration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# compile options
target_compile_features(integration PUBLIC cxx_std_17)
target_compile_options(integration PRIVATE -Wall -Wextra -Wpedantic -Werror)