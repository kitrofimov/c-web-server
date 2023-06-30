#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "1488"
#define BACKLOG 1000

int create_socket();

int main(void)
{
    // create a socket and handle exceptions
    int server_fd = create_socket();
    if (server_fd == -1)
    {
        printf("SERVER: Failed to get address information\n");
        return 1;
    }
    else if (server_fd == -2)
    {
        printf("SERVER: Failed to find an address\n");
        return 2;
    }
    else if (server_fd == -3)
    {
        printf("SERVER: Failed to listen\n");
        return 3;
    }

    printf("SERVER: Socket created\n");

    struct sockaddr_storage client_addr;
    int client_fd;
    while (1)
    {
        socklen_t client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len);

        if (client_fd != -1)
        {
            printf("Failed to accept a connection\n");
            return 4;
        }

        printf("%s", ((struct sockaddr*) &client_addr)->sa_data);
    }

    return 0;
}

int create_socket()
{
    struct addrinfo hints, *servinfo, *p;
    int sock;

    // set the structure to 0 instead of garbage values
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // fill in my IP for me

    // setup the necessary structs with my IP
    if (getaddrinfo(NULL, PORT, &hints, &servinfo) != 0)
    {
        return -1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // create the socket
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock == -1)  // if failed to create a socket, travel to the next node in LL
        {
            continue;
        }

        if (bind(sock, p->ai_addr, p->ai_addrlen) != 0)
        {
            close(sock);
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);  // free the LL, we don't need it anymore

    if (p == NULL)  // if failed to create and listen to a socket
    {
        return -2;
    }

    if (listen(sock, BACKLOG) != 0)  // listen on the binded socket
    {
        return -3;
    }

    return 0;
}