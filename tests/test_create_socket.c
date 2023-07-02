#include <stdio.h>
#include "../src/server_utils.h"

#define NUM_TESTS_CREATE_SOCKET 5

int main(void)
{
    int result;

    // structure: PORT, BACKLOG
    int test_data[NUM_TESTS_CREATE_SOCKET][2] = {
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
        sprintf(port, "%i", test_data[i][0]);  // int -> char[5]
        backlog = test_data[i][1];
        result = create_socket(port, backlog);

        if (result > 0)
            continue;
        else
            return 1;
    }
}