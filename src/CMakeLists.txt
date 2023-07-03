add_library(server server.c)
target_link_libraries(server logger get_exec_path)

add_executable(${PROJECT_NAME} main.c)

set(LIBRARIES server)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
