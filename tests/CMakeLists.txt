add_executable(unit_tests unit_tests.c)
target_include_directories(unit_tests PUBLIC ../lib)
target_link_libraries(unit_tests server_utils)
