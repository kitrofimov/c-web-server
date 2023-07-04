#include <sys/socket.h>
#include "config.h"

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

struct route
{
    char uri[URI_SIZE];
    char html_path[HTML_PATH_SIZE];
    struct route *next;
};

int start_server(char *port, int backlog);
int create_socket(char *port, int backlog);
void get_ip_str(struct sockaddr_storage client_addr, char *ip_buf, int *port_buf);
char *render_template(int code, char *rel_path_to_html);
void fork_and_respond(int socket_fd, int client_fd, char *ip_str, int port);
void accept_loop(int socket_fd);
void add_route(char *uri, char *html_path);

#endif