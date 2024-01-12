#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#define IS_WINDOWS 0
#else
#include <windows.h>
#define IS_WINDOWS 1
#endif

int main() {
    #if IS_WINDOWS
        //设置控制台窗口标题
        SetConsoleTitle("test1");
        //声明句柄变量
        HANDLE output;
        //设置打印位置
        COORD pos = {5,15};
        COORD pos1 = {10,5};
        // 设置句柄为标准输出屏幕
        output = GetStdHandle(STD_OUTPUT_HANDLE);
        // 设置输出位置
        SetConsoleCursorPosition(output, pos);
        printf("Hello world!\n");

        SetConsoleCursorPosition(output, pos1);
        //设置输出字体颜色为蓝色
        SetConsoleTextAttribute(output,FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf("welcome!\n");

        //设置字体为高亮绿色
        SetConsoleTextAttribute(output,FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        printf("welcome!\n");

        //”设置字体hello,world”为高亮并添加红色背景
        SetConsoleTextAttribute(output,BACKGROUND_INTENSITY| BACKGROUND_RED);
        printf("welcome!\n");

        // 设置颜色
        SetConsoleTextAttribute(output, 0x01);
        printf("hello world!\n");
        SetConsoleTextAttribute(output, 0x25);
        printf("hello world!\n");
        SetConsoleTextAttribute(output, 0x74);
        printf("hello world!\n");

        // 关闭句柄
        CloseHandle(output);
        system("pause");
    #else
        //设置控制台窗口标题
        printf("\033]0;myProgram\a");
        //设置打印位置
        int pos1[2] = {15,5};
        int pos2[2] = {5,10};
        // 设置输出位置
        printf("\033[%d;%dH", pos1[0], pos1[1]);
        printf("Hello world!\n");

        printf("\033[%d;%dH", pos2[0], pos2[1]);
        //设置输出字体颜色为蓝色
        printf("\033[34m");
        printf("welcome!\n");

        //设置字体为高亮绿色
        printf("\033[92m");
        printf("welcome!\n");

        //”设置字体hello,world”为高亮并添加红色背景
        printf("\033[41m");
        printf("welcome!\033[0m\n");

        // 设置颜色
        printf("\033[34m");
        printf("hello world!\n");
        printf("\033[42;95m");
        printf("hello world!\n");
        printf("\033[47;31m");
        printf("hello world!\n");
        system("read");
    #endif
    
    return 0;
}