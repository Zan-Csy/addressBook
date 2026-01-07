#pragma once
#include <windows.h>

// 常用颜色
#define COLOR_RED 12
#define COLOR_WHITE 7
#define COLOR_GREEN 10
#define COLOR_DARK_RED 4

// UI 工具
void gotoxy(int x, int y);
void setColor(int color);
void hideCursor();
void showCursor();
int getConsoleWidth();
void printCentered(const char* text, int y);
void drawButton(const char* text, int x, int y, int isSelected);

// 逻辑封装
int handleNavigation(char key, int* curRow, int* curCol, int maxRows, int maxCols);
int isValidPhone(const char* phone);
int isValidEmail(const char* email);
