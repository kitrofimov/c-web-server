#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#include "server.h"
#include "../lib/logger/logger.h"
#include "../lib/get_exec_path/get_exec_path.h"
#include "config.h"

struct route *routes_ll;

// Start a server.
// The valid ports are between 1025 and 65535.
int start_server(char *port, int backlog)
{
    // get the path to the executable to set up logfile
    char logfile_path[PATH_BUF_SIZE];
    get_executable_folder(logfile_path, PATH_BUF_SIZE);
    strcat(logfile_path, LOG_FILENAME);
    log_set_logfile(logfile_path);

    int socket_fd = create_socket(port, backlog);
    char exceptions_buf[128];
    // exception handling (logging)
    if (socket_fd > 0)
    {
        log_print(LOG_INFO, LOG_BOTH, "SERVER: Socket created");
    }
    else if (socket_fd == -1)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: Failed to recieve IP information (getaddrinfo)");
        return 1;
    }
    else if (socket_fd == -2)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: In 'setsockopt': %s", strerror(errno));
        return 2;
    }
    else if (socket_fd == -3)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: Failed to bind");
        return 3;
    }
    else if (socket_fd == -4)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: In 'listen': %s", strerror(errno));
        return 4;
    }

    // kill all unused child processes
	// https://beej.us/guide/bgnet/html/split/client-server-background.html#a-simple-stream-server
    struct sigaction sa;
	sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return 5;
    }

	log_print(LOG_INFO, LOG_BOTH, "SERVER: Listening to connections...");

    accept_loop(socket_fd);
}

// Creates, binds and listens to a socket.
// Returns a file descriptor if no exeptions, if there are some, returns negative values.
int create_socket(char *port, int backlog)
{
    struct addrinfo hints, *servinfo, *p;
    int socket_fd;
	int yes = 1;

    // set the structure to 0 instead of garbage values
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // fill in my IP for me

    // setup the necessary structs with my IP
    int getaddrinfo_status = getaddrinfo(NULL, port, &hints, &servinfo);
	if (getaddrinfo_status != 0)
    {
        return -1;
    }

	// loop through all the results in this LL and try to bind
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // create the socket
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1)  // if failed to create a socket, travel to the next node in LL
        {
            continue;
        }

		// make socket "free" itself when it's closed
		// (https://beej.us/guide/bgnet/html/split/system-calls-or-bust.html#bind)
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			return -2;
        }

        // try to bind the socket
		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(socket_fd);
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);  // free the LL, we don't need it anymore

    if (p == NULL)  // if failed to create and bind a socket
    {
        return -3;
    }

    if (listen(socket_fd, backlog) == -1)  // listen on the binded socket
    {
		return -4;
    }

    return socket_fd;
}

// Helper function to kill all unused child processes in `start_server`.
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// Starts an accept loop. Does not return a value.
// Currently only sends the `response` to the client.
// A LOT OF memory errors here I guess. Sometime, I will fix them. I hope so.
void accept_loop(int socket_fd)
{
    // initialize variables for incoming connections
    struct sockaddr_storage client_addr;
	socklen_t client_addr_len;
    struct in_addr ipv4_addr;
    struct in6_addr ipv6_addr;

    int client_fd, sent_bytes;
    char exceptions_buf[128];  // for exceptions

    char ip_str[INET6_ADDRSTRLEN];
    int port;

    char request[READ_BUF_SIZE];
    char response[RESPONSE_SIZE];
    response[0] = '\0';

	while (1)
    {
		// accept the incoming connection
        client_addr_len = sizeof(client_addr);
        client_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_addr_len);

        if (client_fd == -1)
        {
            if (errno != 0)  // if some error happened
            {
                log_print(LOG_ERROR, LOG_BOTH, "SERVER: In 'accept': %s", strerror(errno));
            }
			continue;
        }

        // get the IP string
        // https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c
        if (((struct sockaddr*) &client_addr)->sa_family == AF_INET)  // if using ipv4
        {
            ipv4_addr = ((struct sockaddr_in*) &client_addr)->sin_addr;
            inet_ntop(AF_INET, &ipv4_addr, ip_str, INET_ADDRSTRLEN);
            port = ntohs(((struct sockaddr_in*) &client_addr)->sin_port);
        }
        else if (((struct sockaddr*) &client_addr)->sa_family == AF_INET6)  // if using ipv6
        {
            ipv6_addr = ((struct sockaddr_in6*) &client_addr)->sin6_addr;
            inet_ntop(AF_INET6, &ipv4_addr, ip_str, INET6_ADDRSTRLEN);
            port = ntohs(((struct sockaddr_in6*) &client_addr)->sin6_port);
        }

		if (!fork())  // child process
		{
			close(socket_fd);  // child doesn't need the listener

            if (recv(client_fd, request, READ_BUF_SIZE, 0) == -1)  // reading request
                log_print(LOG_ERROR, LOG_BOTH, "SERVER: In 'recv': %s", strerror(errno));

            // cutting the request at first newline (so now we only have the first line)
            char *newline = strchr(request, '\n');
            *newline = '\0';
            log_print(LOG_INFO, LOG_BOTH, "SERVER: %s:%i; %s", ip_str, port, request);

            char method[8];
            char route_path[URI_SIZE];

            strcpy(method, request);
            method[7] = '\0';
            char *space = strchr(method, ' ');  // cut the method
            *space = '\0';

            if (strcmp(method, "GET") == 0)  // if it is GET method
            {
                // find the first space in string (that's the one before URI)
                // and initialize a new string starting with it
                char *route_path = strchr(request, ' ') + 1;
                space = strchr(route_path, ' ');  // cut the route_path
                *space = '\0';

                // if the route_path in routes linked list, render it
                // if not, render 404
                bool rendered = false;

                for (struct route *p = routes_ll; p != NULL; p = p->next)
                {
                    if (strcmp(p->uri, route_path) == 0)
                    {
                        strcpy(response, render_template(200, p->html_path));
                        rendered = true;
                    }
                }
                if (!rendered)
                    strcpy(response, render_template(404, HTML_IF_404));
            }
            else  // if it is not a GET method
            {
                strcpy(response, render_template(405, HTML_IF_405));
            }

            int response_len = strlen(response);

            if (send(client_fd, response, response_len, 0) == -1)  // send data
                log_print(LOG_ERROR, LOG_BOTH, "SERVER: In 'send': %s", strerror(errno));

            close(client_fd);
            exit(0);
        }
        close(client_fd);  // parent doesn't need the accepter anymore
    }
}

// Render a template. Basically concatenating the response template and contents of an HTML file.
char *render_template(int code, char *rel_path_to_html)
{
    char exec_folder[PATH_BUF_SIZE];
    get_executable_folder(exec_folder, PATH_BUF_SIZE);

    // open html
    char path_to_html[PATH_BUF_SIZE];
    strcpy(path_to_html, exec_folder);
    strcat(path_to_html, rel_path_to_html);

    FILE *pHtml = fopen(path_to_html, "r");
    if (pHtml == NULL)
    {
        return NULL;
    }

    // open response_template
    char path_to_template[PATH_BUF_SIZE];
    strcpy(path_to_template, exec_folder);

    char template_filename[9];
    snprintf(template_filename, 9, "/%i.txt", code);

    strcat(path_to_template, RESPONSE_TEMPLATES_DIR);
    strcat(path_to_template, template_filename);

    FILE *pResponse_template = fopen(path_to_template, "r");
    if (pResponse_template == NULL)
    {
        fclose(pHtml);
        return NULL;
    }

    // read html
    char html[HTML_SIZE];
    fread(html, 1, HTML_SIZE, pHtml);
    fclose(pHtml);

    // read response template
    char response_template[RESPONSE_SIZE];
    fread(response_template, 1, RESPONSE_SIZE, pResponse_template);
    fclose(pResponse_template);

    return strcat(response_template, html);
}

// Add new route to linked list.
void add_route(char *uri, char *html_path)
{
    struct route *new_route = malloc(sizeof(struct route));
    strncpy(new_route->uri, uri, URI_SIZE);
    strncpy(new_route->html_path, html_path, HTML_PATH_SIZE);

    new_route->next = routes_ll;
    routes_ll = new_route;
}