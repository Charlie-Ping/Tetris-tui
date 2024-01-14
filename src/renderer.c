#include <notcurses/nckeys.h>
#include <notcurses/notcurses.h>
#include "../include/renderer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#endif

struct notcurses* nc;
struct ncplane* stdplane;
struct ncplane* title;
struct ncplane* subtitle;
unsigned int std_plane_dim_y, std_plane_dim_x;

int initWindow() {
    setTitle("Tetris");

    struct notcurses_options options = {0};
    nc = notcurses_init(&options  , stdout);
    if (nc == NULL) {
        return EXIT_FAILURE;
    }

    initStdPlane();
    title = setArticleTitle();
    subtitle = setSubTitle();
    notcurses_render(nc);
}

void setTitle(const char* title) {
    #ifdef _WIN32
        SetConsoleTitle(TEXT(title))
    #else
        printf("\033]0;%s\a", title);
    #endif
}

struct ncplane* setArticleTitle() {
    struct ncplane_options nopts = {
        .y = std_plane_dim_y/2-4,
        .x = std_plane_dim_x/2-32,
        .rows = 8,
        .cols = 64,
        .name = "Title"
    };
    struct ncplane* titleplane = ncplane_create(stdplane, &nopts);
    const char* tetris_art[] = {
        " TTTTT  EEEEE TTTTT RRRR  III  SSS  ",
        "   T    E       T   R   R  I  S     ",
        "   T    EEEE    T   RRRR   I   SSS  ",
        "   T    E       T   R  R   I      S ",
        "   T    EEEEE   T   R   R III  SSS  "
    };
    uint64_t channels = 0;
    ncchannels_set_bg_rgb(&channels, 0x707070);
    ncplane_set_base(titleplane, " ", 1, channels);
    int text_len = strlen(tetris_art[0]);
    for (int i = 0; i < 5; ++i) {
        ncplane_putstr_yx(titleplane, i+1, 32-text_len/2, tetris_art[i]);
    }
    return titleplane;
}

struct ncplane* setSubTitle() {
    // 小标题
    struct ncplane_options opt1 = {
        .y = std_plane_dim_y/2+8,
        .x = std_plane_dim_x/2+16,
        .rows = 1,
        .cols = 32,
        .name = "Subtitle"
    };
    struct ncplane* subtitleplane = ncplane_create(stdplane, &opt1);
    ncplane_putstr_yx(subtitleplane, 0, 8, "按Enter开始游戏 ");
    return subtitleplane;
}

struct ncplane* initStdPlane() {
    stdplane = notcurses_stdplane(nc);
    ncplane_dim_yx(stdplane, &std_plane_dim_y, &std_plane_dim_x);
    // 设置灰色背景
    uint64_t channels = 0;
    ncchannels_set_bg_rgb(&channels, 0x808080); // 背景色灰色
    ncplane_set_base(stdplane, " ", 0, channels);
    return stdplane;
}

void waitForStart() {
    while (true) {
        uint32_t key = notcurses_get(nc, NULL, NULL);
        if (key == '\n' || key == NCKEY_ENTER) {
            break;
        }
    }
}

void initGameWindow() {
    ncplane_destroy(title);
    ncplane_destroy(subtitle);
    struct ncplane_options opt = {
        .y = std_plane_dim_y/2-10,
        .x = std_plane_dim_x/2-10,
        .rows = 20,
        .cols = 20,
        .name = "Game"
    };
    struct ncplane* mainplane = ncplane_create(stdplane, &opt);
    uint64_t bgchan = 0;
    ncchannels_set_bg_alpha(&bgchan, NCALPHA_TRANSPARENT);
    // ncchannels_set_fg_alpha(&bgchan, NCALPHA_TRANSPARENT);
    ncplane_set_base(mainplane, " ", 0, bgchan);

    uint64_t channels = 0;
    ncchannels_set_bg_rgb(&channels, 0x66ccff);
    ncchannels_set_fg_rgb(&channels, 0x66ccff);
    unsigned y, x;
    ncplane_dim_yx(mainplane, &y, &x);
    
    ncplane_double_box_sized(mainplane, 0, channels, y+1, x+1, 0b11110000);
    notcurses_render(nc);
}

int waitForKeys() {
    switch(notcurses_get_nblock(nc, NULL)) {
        case 'a':
        case NCKEY_LEFT: 
            return 1;
            break;
        case 'd':
        case NCKEY_RIGHT: {
            return 3;
            break;
        }
        case 's':
        case NCKEY_DOWN: {
            return 2;
            break;
        }
        case 0:
            return 0;
            break;
        case NCKEY_ESC: {
            exit(0);
        }
        
    }
}

void showGameOver() {
}