#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "server_utils.h"
#include "../lib/logger.h"
#include "../lib/get_exec_path.h"
#include "config.h"


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

    // get the path to the executable to set up logfile
    char logfile_path[MAX_PATH_BUF_SIZE];
    get_executable_folder(logfile_path, MAX_PATH_BUF_SIZE);
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
        return 4;
    }
    else if (socket_fd == -2)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: In 'setsockopt': %s", strerror(errno));
        return 5;
    }
    else if (socket_fd == -3)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: Failed to bind");
        return 6;
    }
    else if (socket_fd == -4)
    {
        log_print(LOG_FATAL, LOG_BOTH, "SERVER: In 'listen': %s", strerror(errno));
        return 7;
    }

    // kill all unused child processes
	// https://beej.us/guide/bgnet/html/split/client-server-background.html#a-simple-stream-server
    struct sigaction sa;
	sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return 8;
    }

	log_print(LOG_INFO, LOG_BOTH, "SERVER: Listening to connections...");

    char *response = render_template("html/index.html");
    if (response == NULL)
    {
        log_print(LOG_FATAL, LOG_BOTH, "Failed to read files");
        return 10;
    }
    accept_loop(socket_fd, response);
}
