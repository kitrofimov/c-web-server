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
