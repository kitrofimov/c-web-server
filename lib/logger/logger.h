#ifndef LOGGER_H
#define LOGGER_H

// Log `level` definitions
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
#define LOG_FATAL 4

// Log `to_where` definitions
#define LOG_STDERR 0
#define LOG_FILE 1
#define LOG_BOTH 2

void log_set_logfile(char *path);
int log_print(int level, int to_where, char *format, ...);

#endif