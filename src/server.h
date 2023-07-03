#include "config.h"

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

struct route
{
    char uri[MAX_URI_SIZE];
    char html_path[MAX_HTML_PATH_SIZE];
    struct route *next;
};

int check_port_validity(char *port);
int create_socket(char *port, int backlog);
void accept_loop(int socket_fd);
char *render_template(int code, char *rel_path_to_html);
void sigchld_handler(int s);
int start_server(char *port, int backlog);
void add_route(char *uri, char *html_path);

#endif