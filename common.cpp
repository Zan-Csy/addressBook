#pragma execution_character_set("utf-8")
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "common.h"

// 设置光标位置
void gotoxy(int x, int y) 
{
	COORD pos = { x, y };  //  创建坐标结构体
	SetConsoleCursorPosition(GetStdHandle(handle), pos); // 设置光标位置
}

// 设置文本颜色
void setColor(int color) 
{
	SetConsoleTextAttribute(GetStdHandle(handle), color);// 设置文本颜色
}


// 隐藏光标
void hideCursor() 
{
	CONSOLE_CURSOR_INFO c = { 1, FALSE };// 隐藏光标
	SetConsoleCursorInfo(GetStdHandle(handle), &c);// 设置光标信息
}

// 显示光标
void showCursor() 
{
	CONSOLE_CURSOR_INFO c = { 1, TRUE };    // 显示光标
	SetConsoleCursorInfo(GetStdHandle(handle), &c);
}

// 获取控制台宽度
int getConsoleWidth() 
{
	CONSOLE_SCREEN_BUFFER_INFO length;    
    GetConsoleScreenBufferInfo(GetStdHandle(handle), &length);
	return length.srWindow.Right - length.srWindow.Left + 1;        // 计算宽度
}


// 居中打印文本
void printCentered(const char* txt, int y) //表示行号
{
	int width = getConsoleWidth();  //获取控制台宽度
	int strlens = 0;      //计算文本的可视长度
	for (int i = 0; txt[i]; )  //遍历文本
    {
		if ((unsigned char)txt[i] >= 0x80) //中文字符 UTF-8 编码的中文字符（占3字节
        {
			strlens += 2; 
            i += 3;     //跳过三个字节
        }
        else 
        {
			strlens += 1; 
            i += 1;     //跳过一个字节
        }
    }
	int x = (width - strlens) / 2;      //计算起始打印位置
	gotoxy(x < 0 ? 0 : x, y);   //设置光标位置
	printf("%s", txt); //打印文本
}

// 绘制按钮
void drawButton(const char* txt, int x, int y, int isSelected) 
{
	gotoxy(x, y);   //设置光标打印位置
    if (isSelected) 
    {
        setColor(red);
        printf("> %s <", txt);
    }
    else 
    {
        setColor(white);
        printf("  %s  ", txt);
    }
	setColor(white);// 恢复颜色
}


// 处理菜单导航
int handleNavigation(char input, int* curRow, int* curCol, int maxRows, int maxCols) 
{
    if (input == -32 || input == 0) 
    { 
        char curinput = _getch();
        if (curinput == 72) input = 'w';      // 上
        else if (curinput == 80) input = 's'; // 下
        else if (curinput == 75) input = 'a'; // 左
        else if (curinput == 77) input = 'd'; // 右
    }
    if (input >= 'A' && input <= 'Z') input += 32;
    switch (input) 
    {
	case 'w': *curRow = (*curRow - 1 + maxRows) % maxRows; return 1;    // 行号-1，表示循环
    case 's': *curRow = (*curRow + 1) % maxRows; return 1;
    case 'a': *curCol = (*curCol - 1 + maxCols) % maxCols; return 1;
    case 'd': *curCol = (*curCol + 1) % maxCols; return 1;
    }
    return 0;
}

// 验证电话号码格式
int isValidPhone(const char* phone) 
{
	if (strlen(phone) != 10) return 0;// 长度必须为10
	for (int i = 0; i < 10; i++) // 每个字符必须是数字
        if (phone[i] < '0' || phone[i] > '9') 
            return 0;
    return 1;
}


// 验证邮箱格式
int isValidEmail(const char* email) 
{
	const char* a = strchr(email, '@');        // 查找 '@' 符号
	const char* dot = strstr(email, ".com");    // 查找 '.com' 后缀
    return (a && dot && (a < dot) && (a > email));
}