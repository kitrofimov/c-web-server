#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#define BLK "\033[30m"
#define RED "\033[31m"
#define GRN "\033[32m"
#define YEL "\033[33m"
#define BLU "\033[34m"
#define MAG "\033[35m"
#define CYN "\033[36m"
#define WHT "\033[37m"

#define BLK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GRN_BG "\033[42m"
#define YEL_BG "\033[43m"
#define BLU_BG "\033[44m"
#define MAG_BG "\033[45m"
#define CYN_BG "\033[46m"
#define WHT_BG "\033[47m"

#define RESET "\033[0m"

// Clears the screen
#define clear() printf("\033[H\033[J")
// Goes to (x, y) coordinates on the console
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

#endif