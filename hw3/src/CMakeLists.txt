# Define the list of C++ source files
set(SOURCE_FILES
    dataset/dataset.cpp
    statistics/statistics.cpp
    integration/integration.cpp
    integration/midpoint_rule.cpp
    integration/trapezoidal_rule.cpp
    integration/simpsons_rule.cpp
    integration/gaussian_quadrature.cpp
    bindings.cpp
)

# pybind11 module 'scicpp'
pybind11_add_module(scicpp ${SOURCE_FILES})

# handle include paths for source code compilation
target_include_directories(scicpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# link external libraries
target_link_libraries(scicpp PRIVATE muparserx Eigen3::Eigen)

# install pybind11 module in the ../python directory
install(TARGETS scicpp DESTINATION ${CMAKE_SOURCE_DIR}/python)