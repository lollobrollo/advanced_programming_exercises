add_library(integration SHARED
    integration.cpp 
    integration_strategy.hpp 
    midpoint_rule.cpp 
    trapezoidal_rule.cpp 
    simpsons_rule.cpp 
    gaussian_quadrature.cpp)

find_package(Eigen3 REQUIRED)
find_package(muparserx REQUIRED)

target_link_libraries(integration dataset muparserx Eigen3::Eigen)

target_include_directories(integration PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

# compile options
target_compile_features(integration PUBLIC cxx_std_17)
target_compile_options(integration PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_subdirectory(tests)