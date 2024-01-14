#include "notcurses/notcurses.h"
#include "../include/renderer.h"
#include <string.h>
#include <time.h>
#include "../include/data.h"
#include "../include/game.h"
#include <stdio.h>

#define B_WIDTH 12
#define B_HEIGHT 20

bool is_gameover;
int score;
int gameboard[B_HEIGHT][B_WIDTH];

typedef struct {
    int shape;
    int status;
    int x;
    int y;
} Block;

Block cur_block;

void initGame() {
    initWindow();
    waitForStart();
    initGameWindow();

    srand(time(NULL));
    is_gameover = false;
    score = 0;
    

    clock_t last_time = clock();
    clock_t cur_time;
    int key;
    randomItem();
    bool is_bottom();
    while (!is_gameover) {
        cur_time = clock();
        int is_bottom = 0;
        key = waitForKeys();
        switch (key) {
        case 1:
            blockLeft();
            break;
        case 2:
            is_bottom = blockDown();
            break;
        case 3:
            blockRight();
            break;
        case 4:
            blockChange();
            break;
        }
        
        if ((cur_time - last_time) > CLOCKS_PER_SEC) {
            // 更新方块位置
            blockDown();
            // 重置定时器
            last_time = cur_time;
        }
        updateStatus();
        if (is_bottom) {
            randomItem();
        }
    }   
    showGameOver();
     
}

// 
void updateStatus() {
    int line_state;
    for (int i=0; i<B_HEIGHT; i++) {
        line_state = 0;
        for (int j=0; j<B_WIDTH; j++) {
            // 如果没有方块
            if (gameboard[i][j]==0) {
                line_state = 1;
            }
        }
        // 如果方块占满一行
        if (line_state == 0) {
            // 清空
            for (int j=0; j<B_WIDTH; j++) {
                gameboard[i][j]=0;
            }
            // 上面的方块下落
            for (int h=0; h<i; h++) {
                for (int w=0; w<B_WIDTH; w++) {
                    gameboard[h+1][w]=gameboard[h][w];
                }
            }
            score+=10;
        }
    }
}

void randomItem() {
    cur_block.shape = rand() % 7;
    cur_block.status =rand() % 4;
    cur_block.x = 8;
    cur_block.y = 0;
}

int isCrash() {
    int i, j;
    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {
            if(blockShape[cur_block.shape][cur_block.status][i][j] == 1)
            {
                if (i+cur_block.y<0 || i+cur_block.y >= B_HEIGHT || j+cur_block.x<0|| j+cur_block.x>=B_WIDTH) {
                    return -1;
                }
                if(gameboard[i+cur_block.y][j+cur_block.x] == 1)
                {
                    //方块一产生就发生碰撞
                    if(cur_block.y == 0)
                    {
                        return -2;
                    }
                    return -1;
                }
            }
        }
    }
}



void blockLeft() {
    cur_block.x--;
    int res = isCrash();
    if (res==-2) {
        is_gameover = true;
        return;
    }
    else if (res==-1) {
        cur_block.x++;
    }
}
void blockRight() {
    cur_block.x++;
    int res = isCrash();
    if (res==-2) {
        is_gameover = true;
        return;
    }
    else if (res==-1) {
        cur_block.x--;
    }
}

bool blockDown() {
    cur_block.y++;
    int res = isCrash();
    if (res==-2) {
        is_gameover = true;
        return true;
    }
    else if (res==-1) {
        cur_block.y--;
        return true;
    }
    return false;
}

void blockChange() {
    cur_block.status++;
    cur_block.status%=4;
    int res = isCrash();
    if (res==-2) {
        is_gameover = true;
    } else if (res == -1) {
        if (cur_block.status==0) cur_block.status=3;
        else cur_block.status--;
    }
}

// 初始化游戏面板
void initGameBoard() {
    memset(gameboard, 0, sizeof(gameboard[0])*B_HEIGHT);
    // 底部墙壁
    for (int i=0; i<B_WIDTH; i++) {
        gameboard[B_HEIGHT-1][i]=1;
    }
    //左右墙壁
    for (int i=0; i<B_HEIGHT; i++) {
        gameboard[i][0]=1;
        gameboard[i][B_WIDTH-1]=1;
    }
}