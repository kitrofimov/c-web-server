cmake_minimum_required(VERSION 3.18.4)
project(c_web_server)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)
add_subdirectory(tests)
