#pragma once
#include <windows.h>

// 常用颜色
#define red 12	//红
#define white 7	//白
#define green 10	//绿
#define deepred 4	//深红

#define handle STD_OUTPUT_HANDLE  //控制台句柄

void gotoxy(int x, int y);	//设置光标位置
void setColor(int color);	//设置文本颜色
void hideCursor();	//隐藏光标
void showCursor();	//显示光标
int getConsoleWidth();	//获取控制台宽度
void printCentered(const char* text, int y);	//居中打印文本
void drawButton(const char* text, int x, int y, int isSelected);	//绘制按钮（如取消，确认）
int handleNavigation(char key, int* curRow, int* curCol, int maxRows, int maxCols);		//处理菜单导航
int isValidPhone(const char* phone);	//验证电话号码格式
int isValidEmail(const char* email);	//验证邮箱格式
