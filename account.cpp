#pragma execution_character_set("utf-8")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "account.h"
#include "common.h"

User users[MAX_USERS];  // 用户数组
int userCount = 0;      // 当前用户数量
char currentLoggedInUser[50] = "";  // 当前登录用户名

// 从文件加载用户数据
void loadUsers() 
{
	FILE* f;    
	userCount = 0;      // 当前用户数量
	if (fopen_s(&f, USER_FILE, "rb") == 0)  // 打开文件 
    {
		fread(&userCount, sizeof(int), 1, f);       // 读取用户数量
		if (userCount > 0) fread(users, sizeof(User), userCount, f);  // 读取用户数据
        fclose(f);
    }
}

// 保存用户数据到文件
void saveUsers() 
{
    FILE* f;
	if (fopen_s(&f, USER_FILE, "wb") == 0)  // 打开文件
    {
		fwrite(&userCount, sizeof(int), 1, f);	  // 写入用户数量
		fwrite(users, sizeof(User), userCount, f);  // 写入用户数据
		fclose(f);  // 关闭文件
    }
}

// 密码输入函数（隐藏显示）
void passwordInput(char* password, int maxLen, int x, int y) 
{
    int i = 0; 
    char input;
	gotoxy(x, y); printf("                ");   
	gotoxy(x, y); showCursor();     // 显示光标
    while (i < maxLen - 1) 
    {
        input = _getch();
        if (input == '\r') break;
		if (input == '\b' && i > 0)     // 处理退格键
        {
            i--; printf("\b \b");
        }
		else if (input >= 32 && input <= 126)   // 可打印字符
        {
			password[i++] = input; printf("*"); // 显示*
        }
    }
	password[i] = '\0'; // 结束字符串
    hideCursor();
}

// 用户注册
void registerUser() 
{
	User tmp = { "", "" };  // 临时用户变量
	char confirm[50] = ""; /// 确认密码
	int curX = 0, curY = 0, startY = 6, width = getConsoleWidth();// 当前行列，起始Y坐标，控制台宽度
    int labelX = width / 2 - 15;    //标签X坐标

    while (1) 
    {
        system("cls"); 
        hideCursor();
        printCentered("--- 账 户 注 册 ---", startY - 3);
        gotoxy(labelX, startY); 
		if (curX == 0) setColor(red);   //用户名
		printf(curX == 0 ? "> 用户名: %s <" : "  用户名: %s", tmp.username);// 显示用户名
		setColor(white);    //恢复颜色


		gotoxy(labelX, startY + 2);     //密码
        if (curX == 1) setColor(red);
        printf(curX == 1 ? "> 密  码: %s <" : "  密  码: %s", strlen(tmp.password) > 0 ? "******" : ""); 
        setColor(white);

        gotoxy(labelX, startY + 4); 
        if (curX == 2) setColor(red);

        printf(curX == 2 ? "> 确  认: %s <" : "  确  认: %s", (strlen(confirm) > 0 ? "******" : ""));
        setColor(white);

        if (strlen(tmp.password) > 0 && strlen(confirm) > 0) 
        {
            gotoxy(labelX + 10, startY + 5);
            if (strcmp(tmp.password, confirm) == 0) 
            {
                setColor(green); 
				printf("√ 密码匹配"); //显示密码匹配
            }
            else 
            {
                setColor(deepred); 
				printf("× 两次密码不一致"); //显示密码不匹配
            }
			setColor(white);    //恢复颜色
        }

		drawButton("保 存", width / 2 - 12, startY + 8, curX == 3 && curY == 0);  //保存按钮
		drawButton("取 消", width / 2 + 4, startY + 8, curX == 3 && curY == 1);  //取消按钮

        char ch = _getch();
		handleNavigation(ch, &curX, &curY, 4, curX == 3 ? 2 : 1);       //处理导航
        if (ch == '\r') 
        {
			if (curX == 0) // 用户名输入
            { 
				showCursor(); // 显示光标
                gotoxy(labelX + 10, startY); 
                scanf_s("%49s", tmp.username, 50);
                while (getchar() != '\n');
                hideCursor();
            }
            else if (curX == 1) passwordInput(tmp.password, 50, labelX + 10, startY + 2);
            else if (curX == 2) passwordInput(confirm, 50, labelX + 10, startY + 4);
            else if (curX == 3) 
            {
                if (curY == 1) return;
                if (strlen(tmp.username) > 0 && strcmp(tmp.password, confirm) == 0) 
                {
					users[userCount++] = tmp; //储存新用户
					saveUsers();// 保存用户数据
                    printCentered("注册成功！", startY + 10); 
                    Sleep(1000); 
                    return;
                }
            }
        }
    }
}


// 账户登录/注册系统
int accountSystem() 
{
    loadUsers();
	char inU[50] = "", inP[50] = "";    // 输入的用户名和密码
    int curX = 0, curY = 0, startY = 8, width = getConsoleWidth();
	int labelX = width / 2 - 12;// 标签X坐标

    while (1) 
    {
        system("cls"); 
        hideCursor();
        
        printCentered("=== 系 统 登 录 ===", startY - 4);
        gotoxy(labelX, startY); 
        if (curX == 0) setColor(red);
        printf(curX == 0 ? "> 用户名: %s <" : "  用户名: %s", inU); 
        setColor(white);

        gotoxy(labelX, startY + 2); if (curX == 1) setColor(red);
        
		char mk[51] = "";   // 显示用的密码变量
		for (int i = 0; i < (int)strlen(inP); i++) strcat(mk, "*"); // 用*号显示密码
        printf(curX == 1 ? "> 密  码: %s <" : "  密  码: %s", mk); 
        setColor(white);

        drawButton("登 录", width / 2 - 12, startY + 5, (curX == 2 && curY == 0));
        drawButton("注 册", width / 2 + 2, startY + 5, (curX == 2 && curY == 1));
        drawButton("退出系统", (width - 10) / 2, startY + 8, (curX == 3));

        char ch = _getch(); 
		handleNavigation(ch, &curX, &curY, 4, curX == 2 ? 2 : 1);// 处理导航
		if (ch == '\r') // 回车键输入 覆盖
        {
			if (curX == 0) // 用户名输入
            {
                showCursor();//显示光标
                gotoxy(labelX + 10, startY); 
                scanf_s("%49s", inU, 50);
                while (getchar() != '\n');
                hideCursor(); 
            }
			else if (curX == 1) passwordInput(inP, 50, labelX + 10, startY + 2);    // 密码输入
			else if (curX == 2) // 登录或注册
            {
                if (curY == 0) 
                {
                    int idx = -1;
					for (int i = 0; i < userCount; i++) //判断用户是否存在
                        if (strcmp(users[i].username, inU) == 0) 
                        {
                            idx = i; 
                            break;
                        }
					if (idx == -1) // 用户不存在
                    {
                        setColor(deepred); 
                        printCentered("错误：用户不存在！", startY + 10); 
                        setColor(white); _getch(); 
                    }
					// 用户存在，检查密码
                    else if (strcmp(users[idx].password, inP) != 0) 
                    {
                        setColor(deepred); 
                        printCentered("错误：密码错误！", startY + 10);
                        setColor(white); _getch();
                    }
					// 登录成功
                    else 
                    {
                        strcpy_s(currentLoggedInUser, inU); 
                        return 1;
                    }
                }
				// 注册新用户
                else registerUser();
            }
            //退出
            else if (curX == 3) return 0;
        }
    }
}