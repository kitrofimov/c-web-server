add_executable(test_check_port_validity test_check_port_validity.c)
target_include_directories(test_check_port_validity PUBLIC ../lib)
target_link_libraries(test_check_port_validity server_utils)

add_executable(test_create_socket test_create_socket.c)
target_include_directories(test_create_socket PUBLIC ../lib)
target_link_libraries(test_create_socket server_utils)
