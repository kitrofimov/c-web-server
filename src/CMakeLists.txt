add_library(server_utils server_utils.c)
target_link_libraries(server_utils logger get_exec_path)

add_executable(${PROJECT_NAME} main.c)

set(LIBRARIES server_utils logger get_exec_path)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
