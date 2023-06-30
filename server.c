#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT "1488"
#define BACKLOG 1000

//TODO: fix random connection resets
// TODO: try to render some HTML
// TODO: routes?
// TODO: forms?
// TODO: cookies? (sessions?)

int main(void)
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
    int status = getaddrinfo(NULL, PORT, &hints, &servinfo);
	if (status != 0)
    {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

	// loop through all the results in this LL and try to bind
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // create the socket
        socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_fd == -1)  // if failed to create a socket, travel to the next node in LL
        {
			perror("socket");
            continue;
        }

		// make socket "free" itself when it's closed
		// (https://beej.us/guide/bgnet/html/split/system-calls-or-bust.html#bind)
		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
			return 2;
        }

        // try to bind the socket
		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(socket_fd);
			perror("bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);  // free the LL, we don't need it anymore

    if (p == NULL)  // if failed to create and bind a socket
    {
		fprintf(stderr, "failed to bind\n");
        return 2;
    }

    printf("SERVER: Socket created\n");

    if (listen(socket_fd, BACKLOG) == -1)  // listen on the binded socket
    {
		perror("listen");
		return 3;
    }

	// TODO: understand what this thing is 
	// (https://beej.us/guide/bgnet/html/split/client-server-background.html#a-simple-stream-server)
	/* sa.sa_handler = sigchld_handler; // reap all dead processes */
    /* sigemptyset(&sa.sa_mask); */
    /* sa.sa_flags = SA_RESTART; */
    /* if (sigaction(SIGCHLD, &sa, NULL) == -1) { */
        /* perror("sigaction"); */
        /* exit(1); */
    /* } */

	printf("SERVER: Listening to connections...\n");
	
	// initialize variables for incoming connections
    struct sockaddr_storage client_addr;
	socklen_t client_addr_len;
    int client_fd, sent_bytes;

	// the message we want to send
	char *msg = "<h1>Hello, client</h1>";
	int msg_len = strlen(msg);

	while (1)
    {
		// accept the incoming connection
        client_addr_len = sizeof(client_addr);
        client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_fd == -1)
        {
			perror("accept");
			continue;
        }

		if (!fork())  // this is the child process
		{
			close(socket_fd);  // child doesn't need the listener
            if (send(client_fd, msg, msg_len, 0) == -1)
                perror("send");
            close(client_fd);
            exit(0);
        }
        close(client_fd);  // parent doesn't need the accepter anymore
    }

    return 0;
}
