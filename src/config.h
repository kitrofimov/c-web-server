#define RESPONSE_TEMPLATES_DIR "response_templates"
#define LOG_FILENAME "serverlog.log"
#define HTML_IF_404 "html/404.html"  // html to render if 404 (SHOULD EXIST!)
#define HTML_IF_405 "html/405.html"  // html to render if 405 (method not allowed) (SHOULD EXIST!)

#define PATH_BUF_SIZE 512  // size of buffers for any paths
#define READ_BUF_SIZE 1024  // size of buffer to read to client's request
#define URI_SIZE 64  // size of buffer to store URI (/, /hello, etc.)
#define HTML_PATH_SIZE 128  // size of buffer to store *path* to HTML
#define HTML_SIZE 512  // size of buffer to store HTML (not the name, but contents!)
#define RESPONSE_SIZE 1024  // size of buffer to store response to the client