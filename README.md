# c-web-server
This is a web server in C, which I am creating as a side-project. Here is an example of a simple server with 2 routes (code is from `src/main.c`):

```c
#include "server.h"

int main(void)
{
    add_route("/", "html/index.html");
    add_route("/hello", "html/hello.html");
    return start_server("8080", 100);
}
```

Currently this (library, I guess?) **is memory unsafe** and supports only `GET` requests, and I don't think I will add more methods in the closest future, I would rather fix memory errors and restructure the code overall.

# Build

Check `src/config.h` to see (and probably change) some settings.

```bash
mkdir build
cd build
cmake ..
make
```

The binaries will appear in `bin/` directory.

**Note**: *you shouldn't delete `HTML_IF_404`, `HTML_IF_405` and everything in `RESPONSE_TEMPLATES_DIR`! (see `src/config.h`)*

# Documentation

## `start_server`

```c
int start_server(char *port, int backlog);
```

Start a server. The valid ports are between 1025 and 65535. `backlog` is the number of connections allowed on the incoming queue.

## `create_socket`

```c
int create_socket(char *port, int backlog);
```

Creates, binds and listens to a socket. The valid ports are between 1025 and 65535. `backlog` is the number of connections allowed on the incoming queue. 

Returns a file descriptor if no exeptions, if there are some, returns negative values (and sets `errno`).

* `-1` - `getaddrinfo` failed
* `-2` - `setsockopt` failed
* `-3` - failed to create and bind a socket
* `-4` - failed to start listening

## `sigchld_handler`

```c
void sigchld_handler(int s);
```

Helper function to kill all unused child processes in `start_server`.

## `accept_loop`

```c
void accept_loop(int socket_fd);
```

Starts an accept loop. Does not return a value.

## `render_template`

```c
char *render_template(int code, char *rel_path_to_html);
```

Returns a HTTP response with `code` and HTML file. Basically concatenating the response template and contents of an HTML file.

Returns `NULL` if failed to open some file.

## `add_route`

```c
void add_route(char *uri, char *html_path);
```

Adds a route to a linked list of routes.

# TODO
- refactor the code
- add `free()`'ing the routes linked list

# ISSUES
- ~~`accept()` accepts the connection twice (if connecting from browser)~~ **It is like that because most browsers auto-request /favicon.ico file (even if there is no such file on the server)**