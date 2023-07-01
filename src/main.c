#include <stdio.h>
#include <stdlib.h>
#include "server_utils.h"

int main(int argc, char **argv)
{
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
    if (socket_fd < 0)  // if some kind of failure when creating socket
    {
        return 4;
    }

    printf("SERVER: Socket created\n");

	// TODO: understand what this thing is 
	// https://beej.us/guide/bgnet/html/split/client-server-background.html#a-simple-stream-server

	/* sa.sa_handler = sigchld_handler; // reap all dead processes */
    /* sigemptyset(&sa.sa_mask); */
    /* sa.sa_flags = SA_RESTART; */
    /* if (sigaction(SIGCHLD, &sa, NULL) == -1) { */
        /* perror("sigaction"); */
        /* exit(1); */
    /* } */

	printf("SERVER: Listening to connections...\n");

    accept_loop(socket_fd, "<h1>Hello, client</h1>");
}