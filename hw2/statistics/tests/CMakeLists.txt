add_executable(test_statistics test_statistics.cpp)

find_package(GTest REQUIRED)

target_link_libraries(test_statistics PRIVATE GTest::gtest)
target_link_libraries(test_statistics PRIVATE statistics)

add_test(NAME test_statistics COMMAND test_statistics)