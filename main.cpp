#pragma execution_character_set("utf-8")
#include <windows.h>
#include <conio.h>
#include "common.h"
#include "addressbook.h"
#include "account.h"

int main() 
{
    //解决编码显示问题
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 外层大循环：支持多次登录/切换账户 
    while (1) 
    {
        // 1. 账户登录/注册系统
        if (!accountSystem()) 
        {
            return 0; // 在登录界面选“退出”则结束整个程序
        }

        // 2. 登录成功后，初始化通讯录环境
        int choice = 0, dummyCol = 0;
        loadContacts(); // 加载当前登录用户的专属数据
        hideCursor();

        system("cls");
        printCentered("=== 通讯录管理系统 ===", 5);
        printCentered("(方向键或WASD控制, Enter确定)", 6);

        // 通讯录菜单操作 
        int switchAccount = 0; // 标记是否点击了切换账户
        while (1) 
        {
            showMenu(choice);   
            char input = _getch();

            //此时菜单总数变为了 7
            handleNavigation(input, &choice, &dummyCol, 7, 1);

			if (input == '\r') // 回车确认，——getch（）必须为\r,不能是\n
            {
                switch (choice) 
                {
                case 0: addContact(); break;//增
                case 1: deleteContact(); break;//删
				case 2: searchContact(); break;//查
				case 3: displayContacts(); break;// 显示
				case 4: sortContacts(); break;// 排序
                case 5: // --- 切换账户 ---
                    saveContacts(); // 必须先保存当前用户数据
                    switchAccount = 1; // 标记需要切换
                    break;
                case 6: // --- 退出系统 ---
                    saveContacts();
                    system("cls");
                    printCentered("正在安全退出...", 10);
                    Sleep(800);
                    return 0;
                }

                // 如果点击了切换账户，跳出内层菜单循环，重新进入外层登录循环
                if (switchAccount) break;

                // 否则，执行完功能后重绘主界面
                system("cls");
                printCentered("=== 通讯录管理系统 ===", 5);
                printCentered("(方向键或WASD控制, Enter确定)", 6);
            }
        }
        // 执行到这里说明 break 了内层循环，程序会重新运行 accountSystem()
    }
    return 0;
}