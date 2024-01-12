#include "../include/windows_ui.h"
#include "../include/curses.h"
#include <string.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <stdio.h>

void initWindow() {

    // 初始化pdcurses
    cbreak();
    noecho();


    printw("hello world!你好世界");
    refresh();
    // height = 300;
    // width = 5000;
    // startx = (COLS-width)/2;
    // starty = (LINES-height)/2;

    // win = newwin(height, width, starty, startx);
    
    // mvwprintw(win, 1, 1, "hello world!");
    // wrefresh(win);
    
    getch();
    // delwin(win);
    endwin();
}

void setTitle(char *title) {
    #ifdef _WIN32
        SetConsoleTitle(TEXT(title));
    #else
        printf("\033]0;%s\a", title);
    #endif
}