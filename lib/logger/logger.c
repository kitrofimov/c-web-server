#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#include "../console_utils/console_utils.h"
#include "logger.h"

#define SIZE_MSG_FORMATTED 128

char logfile_path[128] = "";

void log_set_logfile(char *path)
{
    strcpy(logfile_path, path);
}

// Log `msg` to `to_where`. See logger.h definitions.
// Returns 0 on success, 1 if there is no such LOG_LEVEL code, 2 if there is no file opened for logging.
int log_print(int level, int to_where, char *format, ...)
{
    va_list args;
    va_start(args, format);

    // get the log level as a string
    char level_str[6];
    char color_macro[6];
    switch (level)
    {
        case 0:
            strcpy(level_str, "DEBUG");
            strcpy(color_macro, CYN);
            break;
        case 1:
            strcpy(level_str, " INFO");
            strcpy(color_macro, WHT);
            break;
        case 2:
            strcpy(level_str, " WARN");
            strcpy(color_macro, YEL);
            break;
        case 3:
            strcpy(level_str, "ERROR");
            strcpy(color_macro, RED);
            break;
        case 4:
            strcpy(level_str, "FATAL");
            strcpy(color_macro, MAG);
            break;

        default:
            return 1;
    }

    // get time
    // https://stackoverflow.com/questions/9101590/fprintf-and-ctime-without-passing-n-from-ctime
    char time_formatted[32];
    time_t t = time(NULL);
    struct tm *p = localtime(&t);
    strftime(time_formatted, sizeof time_formatted, "%d-%m-%Y %H:%M:%S", p);

    char msg_formatted[SIZE_MSG_FORMATTED];
    vsnprintf(msg_formatted, SIZE_MSG_FORMATTED, format, args);

    // print
    if (to_where == LOG_STDERR || to_where == LOG_BOTH)
    {
        fprintf(stderr, "%s%s" RESET " | %s | %s\n", color_macro, level_str, time_formatted, msg_formatted);
    }

    if (to_where == LOG_FILE || to_where == LOG_BOTH)
    {
        if (strlen(logfile_path) == 0)
        {
            log_print(LOG_WARN, LOG_STDERR,
                      "No path_to_logfile! Previous log wasn't writed to the logfile!");
            return 2;
        }

        FILE *fd = fopen(logfile_path, "a");
        fprintf(fd, "%s | %s | %s\n", level_str, time_formatted, msg_formatted);
        fclose(fd);
    }

    return 0;
}
