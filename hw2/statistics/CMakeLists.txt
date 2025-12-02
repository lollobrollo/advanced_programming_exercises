add_library(statistics statistics.cpp)

target_link_libraries(statistics dataset)

target_include_directories(statistics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})