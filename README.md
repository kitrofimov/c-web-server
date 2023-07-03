# c-web-server
The name is self-explanatory. This is a web server in C, which I am developing as a side-project. I don't plan on making this a "framework" yet. Maybe later.

# TODO
- fix `malloc(): invalid next size (unsorted)` when routing
- refactor the code

# ISSUES
- ~~`accept()` accepts the connection twice (if connecting from browser)~~ **It is like that because most browsers auto-request /favicon.ico file (even if there is no such file on the server)**