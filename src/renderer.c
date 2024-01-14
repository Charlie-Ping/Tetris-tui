#include <notcurses/nckeys.h>
#include <notcurses/notcurses.h>
#include "../include/renderer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#endif

int initWindow() {
    setTitle("Tetris");

    struct notcurses_options options = {0};
    struct notcurses* nc = notcurses_init(&options  , stdout);
    if (nc == NULL) {
        return EXIT_FAILURE;
    }

    // stdplane
    unsigned int plane_dim_y, plane_dim_x;
    struct ncplane* stdplane = notcurses_stdplane(nc);
    ncplane_dim_yx(stdplane, &plane_dim_y, &plane_dim_x);
    // 设置灰色背景
    uint64_t channels = 0;
    ncchannels_set_bg_rgb(&channels, 0x808080); // 背景色灰色
    ncplane_set_base(stdplane, " ", 0, channels);

    // 设置提示窗口
    struct ncplane_options nopts = {
        .y = plane_dim_y/2-4,
        .x = plane_dim_x/2-32,
        .rows = 8,
        .cols = 64,
        .name = "Game"
    };
    struct ncplane* titleplane = ncplane_create(stdplane, &nopts);
    const char* tetris_art[] = {
        " TTTTT  EEEEE TTTTT RRRR  III  SSS  ",
        "   T    E       T   R   R  I  S     ",
        "   T    EEEE    T   RRRR   I   SSS  ",
        "   T    E       T   R  R   I      S ",
        "   T    EEEEE   T   R   R III  SSS  "
    };
    uint64_t chan1 = 0;
    ncchannels_set_bg_rgb(&channels, 0x707070);
    ncplane_set_base(titleplane, " ", 1, channels);
    

    uint32_t chan2 = 0xff0000;
    ncplane_set_fg_rgb(titleplane, chan2);

    int text_len = strlen(tetris_art[0]);
    for (int i = 0; i < 5; ++i) {
        ncplane_putstr_yx(titleplane, i+1, 32-text_len/2, tetris_art[i]);
    }

    // 小标题
    struct ncplane_options opt1 = {
        .y = plane_dim_y/2+8,
        .x = plane_dim_x/2+16,
        .rows = 1,
        .cols = 32,
        .name = "Subtitle"
    };
    struct ncplane* subtitleplane = ncplane_create(stdplane, &opt1);
    ncplane_putstr_yx(subtitleplane, 0, 8, "按Enter开始游戏 ");
    // struct ncplane* titleplane = ncplane_create(stdplane, &nopts);

    // uint64_t chan1 = 0;
    // ncchannels_set_bg_rgb(&chan1, 0x808080); // 背景色深灰色
    // ncplane_set_base(titleplane, " ", 0, chan1);
    
    // struct ncplane* boxplane = ncplane_create(stdplane, &box_opts);
    // ncplane_set_base(boxplane, " ", 0, channels);
    // ncplane_box(boxplane, NULL, NULL, 0, 0, 0, 0, 0, 0, 0);
    // // 设置边框颜色
    // uint64_t borderchannels = 0;
    // ncchannels_set_fg_rgb(&borderchannels, 0xff0000);
    // ncchannels_set_bg_rgb(&borderchannels, ncchannels_bg_rgb(borderchannels));

    // // 绘制边框
    // ncplane_box(plane, NULL, NULL, 0, 0, 0, 0, 0, 0, 0b11111111);

    // 渲染屏幕
    notcurses_render(nc);

    while (true) {
        uint32_t key = notcurses_get(nc, NULL, NULL);
        if (key == '\n' || key == NCKEY_ENTER) {
            break;
        }
    }
    ncplane_destroy(titleplane);
    ncplane_destroy(subtitleplane);

    notcurses_render(nc);
    while (true) {
        uint32_t key = notcurses_get(nc, NULL, NULL);
        if (key == 'e') {
            break;
        }
    }

    notcurses_stop(nc);
    return EXIT_SUCCESS;

}

void setTitle(const char* title) {
    #ifdef _WIN32
        SetConsoleTitle(TEXT(title))
    #else
        printf("\033]0;%s\a", title);
    #endif
}

