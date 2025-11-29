add_library(integration integration.cpp)

target_link_libraries(integration file_handler)

target_include_directories(integration PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})