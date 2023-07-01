// UNIT TESTS

#include <stdio.h>
#include <time.h>
#include "../src/server_utils.h"
#include "../lib/console_utils.h"

#define NUM_TESTS_CREATE_SOCKET 5
#define NUM_TESTS_CHECK_PORT_VALIDITY 8

int result;
struct timespec start, end;
double time_spent;
char buf[128];

void test_create_socket(void);
void test_check_port_validity(void);

int main(void)
{
    test_create_socket();
    test_check_port_validity();
}

void print_pass(void)
{
    printf(GRN_BG "PASS" RESET);
}

void print_fail(void)
{
    printf(RED_BG "FAIL" RESET " | Returned: %i", result);
}

void print_final(double time_spent, char *string)
{
    printf(" | Time: %fs | %s", time_spent, string);
}

void test_create_socket(void)
{
    printf(WHT_BG BLK "CREATE_SOCKET\n" RESET);

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
        clock_gettime(CLOCK_REALTIME, &start);

        sprintf(port, "%i", test_data[i][0]);
        backlog = test_data[i][1];
        result = create_socket(port, backlog);

        if (result > 0)
            print_pass();
        else
            print_fail();

        clock_gettime(CLOCK_REALTIME, &end);
        time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        sprintf(buf, "Create a socket at port %s with backlog of %i\n", port, backlog);
        print_final(time_spent, buf);
    }
}

void test_check_port_validity(void)
{
    printf(WHT_BG BLK "CHECK_PORT_VALIDITY\n" RESET);

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
        clock_gettime(CLOCK_REALTIME, &start);

        sprintf(port, "%i", test_data[i][0]);
        result = check_port_validity(port);

        if (result == test_data[i][1])
            print_pass();
        else
            print_fail();

        clock_gettime(CLOCK_REALTIME, &end);
        time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        sprintf(buf, "Check %s port validity\n", port);
        print_final(time_spent, buf);
    }
}