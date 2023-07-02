#include <stdio.h>
#include "../src/server_utils.h"

#define NUM_TESTS_CHECK_PORT_VALIDITY 8

int main(void)
{
    int result;

    // structure: {port, needed_output}
    int test_data[NUM_TESTS_CHECK_PORT_VALIDITY][2] = {
        {1488, 1},
        {72727, 0},
        {13, 0},
        {65535, 1},
        {1024, 0},
        {1025, 1},
        {65536, 0},
        {8080, 1}
    };
    char port[5];

    for (int i = 0; i < NUM_TESTS_CHECK_PORT_VALIDITY; i++)
    {
        sprintf(port, "%i", test_data[i][0]);
        result = check_port_validity(port);

        if (result == test_data[i][1])
            continue;
        else
            return 1;
    }
}