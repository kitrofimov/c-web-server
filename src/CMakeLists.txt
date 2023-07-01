add_library(server_utils server_utils.c)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} server_utils)
