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

Currently this (library, I guess?) supports only `GET` requests and HTML routing (no form submission, etc.), and I don't think I will add more functionality in the closest future, as this project is not *currently* very fun to me.

# Build

Check `src/config.h` to see (and change if you want) some settings.

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

Returns error code if some error occurred (and logs it), returns nothing if everything is good (because starts an infinite loop)

* `1` - failed to recieve IP information (getaddrinfo)
* `2` - `setsockopt` failure
* `3` - failed to bind
* `4` - `listen` failure

## `create_socket`

```c
int create_socket(char *port, int backlog);
```

Creates, binds and listens to a socket. The valid ports are between 1025 and 65535. `backlog` is the number of connections allowed on the incoming queue. Is a helper function.

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

## `get_ip_str`

```c
void get_ip_str(struct sockaddr_storage client_addr, char *ip_buf, int *port_buf);
```

Get IP string from client's connection information. Sets ip_buf to IP, port_buf to PORT. Is a helper function.
**NOTE**: `ip_buf` should be minimally size `INET6_ADDRSTRLEN`.
Referenced from: https://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c

## `render_template`

```c
char *render_template(int code, char *rel_path_to_html);
```

Returns a HTTP response with `code` and HTML file. Basically concatenating the response template and contents of an HTML file. Is a helper function.

Returns `INTERNAL_SERVER_ERROR` response if failed to open some file.

## `fork_and_respond`

```c
void fork_and_respond(int socket_fd, int client_fd, char *ip_str, int port);
```

Forks the program and responds to a client. Is a helper function.

## `accept_loop`

```c
void accept_loop(int socket_fd);
```

Starts an accept loop. Does not return a value.

## `add_route`

```c
void add_route(char *uri, char *html_path);
```

Adds a route to a linked list of routes.

# TODO
- add `free()`'ing the routes linked list

# ISSUES
- ~~`accept()` accepts the connection twice (if connecting from browser)~~ **It is like that because most browsers auto-request /favicon.ico file (even if there is no such file on the server)**
- `The byte stream was erroneous according to the character encoding that was declared. The character encoding declaration may be incorrect.` - Will be fixed with memory errors in `accept_loop` and `render_template`.