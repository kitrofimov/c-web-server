#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include "server_utils.h"
#include "../lib/logger.h"
#include "../lib/get_exec_path.h"
#include "config.h"

// Checks the port validity (if it is in the range [1025, 65535]).
// Returns 1 if it is valid, 0 if it is not
int check_port_validity(char *port)
{
    int port_num = atoi(port);

    // if the port num is not in the range [1025, 65535]
    if (port_num < 1025 || port_num > 65535)
    {
        return 0;
    }
    else
    {
        return 1;
    }
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

// Starts an accept loop. Does not return a value.
// Currently only sends the `msg` to the client.
void accept_loop(int socket_fd, char *msg)
{
    // initialize variables for incoming connections
    struct sockaddr_storage client_addr;
	socklen_t client_addr_len;
    struct in_addr ipv4_addr;
    struct in6_addr ipv6_addr;

    int client_fd, sent_bytes;
	int msg_len = strlen(msg);
    char exceptions_buf[128];  // for exceptions
    char ip_str[INET6_ADDRSTRLEN];
    int port;
    char log_str[INET6_ADDRSTRLEN + 30];

	while (1)
    {
		// accept the incoming connection
        client_addr_len = sizeof(client_addr);
        client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_addr_len);

        if (client_fd == -1)
        {
            if (errno != 0)  // if some error happened
            {
                log_print(LOG_WARN, LOG_BOTH, "SERVER: In 'accept': %s", strerror(errno));
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

        log_print(LOG_INFO, LOG_BOTH, "SERVER: Got connection from %s:%i",
                  ip_str, port);

		if (!fork())  // child process
		{
			close(socket_fd);  // child doesn't need the listener
            if (send(client_fd, msg, msg_len, 0) == -1)  // send data
            {
                log_print(LOG_WARN, LOG_BOTH, "SERVER: In 'send': %s", strerror(errno));
            }
            close(client_fd);
            exit(0);
        }
        close(client_fd);  // parent doesn't need the accepter anymore
    }
}

char *render_template(char *rel_path_to_html)
{
    char exec_folder[MAX_PATH_BUF_SIZE];
    get_executable_folder(exec_folder, MAX_PATH_BUF_SIZE);

    // open html
    char path_to_html[MAX_PATH_BUF_SIZE];
    strcpy(path_to_html, exec_folder);
    strcat(path_to_html, rel_path_to_html);

    FILE *pHtml = fopen(path_to_html, "r");
    if (pHtml == NULL)
    {
        return NULL;
    }

    // open response_template
    char path_to_template[MAX_PATH_BUF_SIZE];
    strcpy(path_to_template, exec_folder);
    strcat(path_to_template, RESPONSE_TEMPLATES_DIR "/200.txt");

    FILE *pResponse_template = fopen(path_to_template, "r");
    if (pResponse_template == NULL)
    {
        fclose(pHtml);
        return NULL;
    }

    // calculate html size (`fsize` is not crossplatform)
    fseek(pHtml, 0, SEEK_END);
    unsigned long html_size = ftell(pHtml);
    fseek(pHtml, 0, SEEK_SET);

    // read html
    char *html = malloc(html_size);
    fread(html, 1, html_size, pHtml);
    fclose(pHtml);

    // calculate response_template size (`fsize` is not crossplatform)
    fseek(pResponse_template, 0, SEEK_END);
    unsigned long response_template_size = ftell(pResponse_template);
    fseek(pResponse_template, 0, SEEK_SET);

    // read response template
    char *response_template = malloc(html_size + response_template_size);
    fread(response_template, 1, response_template_size, pResponse_template);
    fclose(pResponse_template);

    return strcat(response_template, html);
}