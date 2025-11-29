add_library(statistics statistics.cpp)

target_link_libraries(statistics file_handler)

target_include_directories(statistics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})