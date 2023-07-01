#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

int create_socket(char *port, int backlog);
void accept_loop(int socket_fd, char *msg);

#endif