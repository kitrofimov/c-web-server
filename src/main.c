#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server_utils.h"
#include "../lib/logger.h"

int main(int argc, char **argv)
{
    log_set_logfile("../log.txt");

    char *port;
    int backlog;

    if (argc == 2)
    {
        port = argv[1];

        if (check_port_validity(port) == 0)
        {
            printf("The valid ports are between 1025 and 65535\n");
            return 1;
        }

        backlog = 1000;
    }
    else if (argc == 3)
    {
        port = argv[1];

        if (check_port_validity(port) == 0)
        {
            printf("The valid ports are between 1025 and 65535\n");
            return 2;
        }

        backlog = atoi(argv[2]);
    }
    else
    {
        printf("USAGE: server PORT [BACKLOG]\nThe default value for BACKLOG is 1000\n");
        return 3;
    }

    int socket_fd = create_socket(port, backlog);
    char buf[128];  // for exceptions
    // exception handling (logging)
    if (socket_fd > 0)
    {
        log_print("SERVER: Socket created", LOG_INFO, LOG_BOTH);
    }
    else if (socket_fd == -1)
    {
        log_print("SERVER: Failed to recieve IP information (getaddrinfo)", LOG_FATAL, LOG_BOTH);
        return 4;
    }
    else if (socket_fd == -2)
    {
        sprintf(buf, "SERVER: In 'setsockopt': %s", strerror(errno));
        log_print(buf, LOG_FATAL, LOG_BOTH);
        return 5;
    }
    else if (socket_fd == -3)
    {
        log_print("SERVER: Failed to bind", LOG_FATAL, LOG_BOTH);
        return 6;
    }
    else if (socket_fd == -4)
    {
        sprintf(buf, "SERVER: In 'listen': %s", strerror(errno));
        log_print(buf, LOG_FATAL, LOG_BOTH);
        return 7;
    }

	// TODO: understand what this thing is 
	// https://beej.us/guide/bgnet/html/split/client-server-background.html#a-simple-stream-server

	// sa.sa_handler = sigchld_handler; // reap all dead processes
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART;
    // if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    //     perror("sigaction");
    //     exit(1);
    // }

	log_print("SERVER: Listening to connections...", LOG_INFO, LOG_BOTH);

    accept_loop(socket_fd, "<h1>Hello, client</h1>");
}