// UNIT TESTS

#include <stdio.h>
#include <time.h>
#include "../src/server_utils.h"
#include "../lib/console_utils.h"

#define NUM_TESTS_CREATE_SOCKET 5

int main(void)
{
    int status;
    struct timespec start, end;
    double time_spent;

    // create_socket
    printf(WHT_BG BLK "CREATE_SOCKET\n" RESET);

    // structure: PORT, BACKLOG
    int create_socket_test_data[NUM_TESTS_CREATE_SOCKET][2] = {
        {8888, 1000},
        {2222, 500},
        {1488, 700},
        {27272, 727},
        {9898, 130}
    };
    char port[5];
    int backlog;

    for (int i = 0; i < NUM_TESTS_CREATE_SOCKET; i++)
    { 
        clock_gettime(CLOCK_REALTIME, &start);

        sprintf(port, "%i", create_socket_test_data[i][0]);
        backlog = create_socket_test_data[i][1];
        status = create_socket(port, backlog);

        if (status > 0)
        {
            printf(GRN_BG "PASS" RESET);
        }
        else
        {
            printf(RED_BG "FAIL" RESET " | Code: %i", status);
        }

        clock_gettime(CLOCK_REALTIME, &end);
        time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        printf(" | Time: %fs | Create a socket at port %s with backlog of %i\n",
               time_spent, port, backlog);
    }
}