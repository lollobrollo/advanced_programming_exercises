add_executable(dataset_tests test_dataset.cpp)

find_package(GTest REQUIRED)

target_link_libraries(dataset_tests PRIVATE GTest::gtest)
target_link_libraries(dataset_tests PRIVATE dataset)

add_test(NAME dataset_tests COMMAND dataset_tests)