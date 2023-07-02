#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

// #define METHOD_GET 0
// #define METHOD_POST 1

// struct request
// {
//     int method;
//     char route;
// };

int check_port_validity(char *port);
int create_socket(char *port, int backlog);
void accept_loop(int socket_fd, char *msg);
char *render_template(char *path_to_html);

void sigchld_handler(int s);

#endif