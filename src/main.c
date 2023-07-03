#include "server.h"

int main(void)
{
    add_route("/", "html/index.html");
    add_route("/hello", "html/hello.html");
    return start_server("8080", 100);
}