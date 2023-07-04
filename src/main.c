#include "server.h"

int main(void)
{
    add_route("/", "html/index.html");
    add_route("/hello", "html/hello.html");

    // causes Internal Server Error if accessing, because there is no such HTML file
    add_route("/someotherroute", "html/someotherroute.html");

    return start_server("3434", 100);
}