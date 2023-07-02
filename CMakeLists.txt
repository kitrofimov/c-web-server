cmake_minimum_required(VERSION 3.18.4)
project(c_web_server)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(COPY src/html DESTINATION bin/)
file(COPY src/response_templates DESTINATION bin/)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)

enable_testing ()
add_test (NAME check_port_validity COMMAND test_check_port_validity)
add_test (NAME create_socket COMMAND test_create_socket)
