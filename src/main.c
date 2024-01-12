#include <stdlib.h>
#include <stdio.h>
#include "../include/curses.h"
#include "../include/game.h"
#include "../include/windows_ui.h"

#ifdef _WIN32
    #define PDC_DLL_BUILD
#endif

int main() {
    initWindow();
    initGame();
    ;
    return 0;
}