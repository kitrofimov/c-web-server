#include <string.h>
#include <stdio.h>

#ifdef _WIN32
    #include <Windows.h>
#endif

#ifdef unix
    #include <unistd.h>
#endif

// Get path to the executable. Cross-platform function.
// Returns 0 if success, -1 if some error (and sets `errno` in such case).
int get_executable_path(char *buf, int bufsize)
{
    #ifdef _WIN32  // if windows
        if (GetModuleFileName(NULL, buf, bufsize) == 0)
        {
            return -1;
        }
        return 0;
    #endif

    #ifdef unix  // if unix
        if (readlink("/proc/self/exe", buf, bufsize) == -1)
        {
            return -1;
        }
        return 0;
    #endif
}

// Get path to executable's folder. Cross-platform function.
// Returns 0 on success, -1 if some error (and sets `errno` in such case).
int get_executable_folder(char *buf, int bufsize)
{
    if (get_executable_path(buf, bufsize) == -1)
    {
        printf("Failed to get path to executable\n");
        return -1;
    }

    // find index of last / or \ in path
    #ifdef _WIN32
        int last_occurrence = strrchr(buf, '\\') - buf;
    #endif
    #ifdef unix
        int last_occurrence = strrchr(buf, '/') - buf;
    #endif

    buf[last_occurrence + 1] = '\0'; // cut the path after the last '/'

    return 0;
}