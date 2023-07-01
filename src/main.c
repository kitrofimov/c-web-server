#include <stdio.h>
#include "server_utils.h"

#define PORT "1488"
#define BACKLOG 1000

int main(void)
{
    int socket_fd = create_socket(PORT, BACKLOG);
    if (socket_fd < 0)  // if some kind of failure when creating socket
    {
        return 1;
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