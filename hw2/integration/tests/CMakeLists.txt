add_executable(integration_tests test_integration.cpp)

find_package(GTest REQUIRED)

target_link_libraries(integration_tests PRIVATE GTest::gtest)
target_link_libraries(integration_tests PRIVATE integration)

add_test(NAME integration_tests COMMAND integration_tests)