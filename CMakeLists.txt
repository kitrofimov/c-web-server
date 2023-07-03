cmake_minimum_required(VERSION 3.18.4)
project(webserver)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(COPY src/html DESTINATION bin/)
file(COPY src/response_templates DESTINATION bin/)

add_subdirectory(lib)
add_subdirectory(src)
