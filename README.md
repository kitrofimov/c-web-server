# c-web-server
The name is self-explanatory. This is a web server in C, which I am developing as a side-project. I don't plan on making this a "framework" yet. Maybe later.

# TODO
- give `get_exec_path.c`, `logger.c` and `console_utils.h` own repositories
- routes
    - read headers from client
    - read `referer` value
    - do something with it
- formatting in `render_template`
- refactoring

---

- forms?
- cookies? (sessions?)

# ISSUES

- ~~`accept()` accepts the connection twice (if connecting from browser)~~ **It is like that because most browsers auto-request /favicon.ico file (even if there is no such file on the server)**