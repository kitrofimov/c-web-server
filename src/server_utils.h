#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

int check_port_validity(char *port);
int create_socket(char *port, int backlog);
void accept_loop(int socket_fd, char *msg);
char *render_template(char *path_to_html);

#endif