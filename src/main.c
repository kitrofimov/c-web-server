#include "server.h"

int main(void)
{
    add_route("/", "html/index.html");
    add_route("/hello", "html/hello.html");
    add_route("/someotherroute", "html/someotherroute.html");  // causes Internal Server Error if accessing
    return start_server("8080", 100);
}