#pragma execution_character_set("utf-8")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "addressbook.h"
#include "common.h"
#include "account.h"

Contact contacts[MAX_CONTACTS]; // 联系人数组
int contactCount = 0;// 当前联系人数量


// 从文件加载联系人数据
void loadContacts() 
{
    char fileName[100]; 
	
    sprintf(fileName, "contacts_%s.dat", currentLoggedInUser);
	
    FILE* f; contactCount = 0;// 当前联系人数量
    if (fopen_s(&f, fileName, "rb") == 0) //读取
    {
        fread(&contactCount, sizeof(int), 1, f);
        if (contactCount > 0) fread(contacts, sizeof(Contact), contactCount, f);
        fclose(f);
    }
}

// 保存联系人数据到文件
void saveContacts() 
{
    char fileName[100];
    sprintf(fileName, "contacts_%s.dat", currentLoggedInUser);
    FILE* f;
	if (fopen_s(&f, fileName, "wb") == 0) //写入
    {
        fwrite(&contactCount, sizeof(int), 1, f);
        fwrite(contacts, sizeof(Contact), contactCount, f);
        fclose(f);
    }
}

// 显示主菜单
void showMenu(int currentChoice) 
{
    int startY = 8, width = getConsoleWidth();
    int xStart = (width - 14) / 2;
    // 选项
    const char* opt[] = { "添加联系人", "删除联系人", "查询联系人", "显示所有", "排序名单", "切换账户", "退出系统" };//

    for (int i = 0; i < 7; i++) 
    {
        drawButton(opt[i], xStart, startY + i, i == currentChoice);//传入选择0,1判断颜色选择
    }
}

// 添加联系人
void addContact() 
{
	Contact tmp = { "", "", "" };// 临时联系人变量
    int curX = 0, curY = 0, startY = 4, width = getConsoleWidth();
	int labelX = width / 2 - 15, stP = 0, stE = 0;// 标签X坐标，电话状态，邮箱状态

    while (1) 
    {
        system("cls"); 
        hideCursor();//先隐藏光标
        printCentered("=== 新 建 联 系 人 ===", startY - 2);
        gotoxy(labelX, startY);

        if (curX == 0) setColor(red);

        printf(curX == 0 ? "> 姓  名: %s <" : "  姓  名: %s", tmp.name); setColor(white);

        gotoxy(labelX, startY + 3); if (curX == 1) setColor(red);
        
        printf(curX == 1 ? "> 电  话: %s <" : "  电  话: %s", tmp.phone); setColor(white);
        if (stP != 0) //选中电话
        {
            gotoxy(labelX + 10, startY + 4);
            if (stP == 1) 
            {
                setColor(green);
                printf("√"); 
            }
            else 
            {
                setColor(deepred);
                printf("× (需要10位数字)"); 
            }
            setColor(white);
        }

        gotoxy(labelX, startY + 6); if (curX == 2) setColor(red);
        printf(curX == 2 ? "> 邮  箱: %s <" : "  邮  箱: %s", tmp.email); setColor(white);
        if (stE != 0) //选中邮箱
        {
            gotoxy(labelX + 10, startY + 7);
            if (stE == 1) 
            {
                setColor(green); printf("√"); 
            }
            else 
            {
                setColor(deepred); printf("× (格式: xxx@xx.com)"); 
            }
			setColor(white);//恢复颜色
        }

        drawButton("保 存", width / 2 - 12, startY + 10, curX == 3 && curY == 0);
        drawButton("取 消", width / 2 + 4, startY + 10, curX == 3 && curY == 1);

        char ch = _getch(); handleNavigation(ch, &curX, &curY, 4, curX == 3 ? 2 : 1);

        if (ch == '\r')//选中 
        {
            if (curX < 3) 
            {
                showCursor();
                gotoxy(labelX + 10, startY + curX * 3);//乘三表示跨越三个字
                printf("                ");
                gotoxy(labelX + 10, startY + curX * 3);
				if (curX == 0) scanf_s("%49s", tmp.name, 50);// 姓名
				else if (curX == 1) //电话
                {
                    scanf_s("%19s", tmp.phone, 20);
                    stP = isValidPhone(tmp.phone) ? 1 : -1; 
                }
				else if (curX == 2) //邮箱
                {
                    scanf_s("%49s", tmp.email, 50);
                    stE = isValidEmail(tmp.email) ? 1 : -1; 
                }
                while (getchar() != '\n'); hideCursor();
            }
            else if (curX == 3) 
            {
                if (curY == 1) return;
                if (strlen(tmp.name) > 0 && stP == 1 && stE == 1) 
                {
                    contacts[contactCount++] = tmp; saveContacts();
                    printCentered("保存成功！", startY + 12); Sleep(800); 
                    return;
                }
                else 
                {
                    setColor(deepred); 
                    printCentered("错误：信息不完整或格式有误！", startY + 12); 
                    setColor(white); 
                    _getch(); 
                }
            }
        }
    }
}

void displayContacts() 
{
    system("cls");
    hideCursor();
    printCentered("=== 所 有 联 系 人 列 表 ===", 3);
	if (contactCount == 0) printCentered("空空如也...", 6);// 无联系人
    else 
    {
		char base[200]; 
        sprintf(base, "1. 姓名: %-10s | 电话: %-12s | 邮箱: %s", contacts[0].name, contacts[0].phone, contacts[0].email);
		int Xpos = (getConsoleWidth() - (int)strlen(base)) / 2;// 计算对齐位置
        for (int i = 0; i < contactCount; i++) 
        {
            gotoxy(Xpos < 2 ? 2 : Xpos, 6 + i);
            printf("%d. 姓名: %-10s | 电话: %-12s | 邮箱: %s", i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
        }
    }
    drawButton("返回主菜单", getConsoleWidth() / 2 - 6, 6 + contactCount + 3, 1);
    while (_getch() != '\r');
}

void deleteContact() 
{
    char name[50] = ""; 
    system("cls");
    
    printCentered("=== 删 除 联 系 人 ===", 5);
    printCentered("请输入要删除的姓名:", 7);
    
    showCursor(); 
    gotoxy(getConsoleWidth() / 2 - 5, 8); 
    scanf_s("%49s", name, 50); 
    while (getchar() != '\n'); 
	hideCursor();//输入完隐藏光标

    int idx = -1;
    for (int i = 0; i < contactCount; i++) //找人
        if (strcmp(contacts[i].name, name) == 0) 
        { 
            idx = i; break; 
        }
    if (idx == -1) 
    {
        printCentered("未找到该联系人！", 10);
        _getch();
        return; 
    }
	int col = 1;// 默认选中取消
    while (1) 
    {
        system("cls"); 
        printCentered("确定要删除吗？", 7);
        
        drawButton("确 定", getConsoleWidth() / 2 - 12, 10, col == 0);
        
        drawButton("取 消", getConsoleWidth() / 2 + 4, 10, col == 1);
        
		char ch = _getch();// 获取按键
        int r = 0;
        handleNavigation(ch, &r, &col, 1, 2);
        
        if (ch == '\r') //选择
        {
            if (col == 0) 
            {
                for (int i = idx; i < contactCount - 1; i++) contacts[i] = contacts[i + 1];
                contactCount--;
                saveContacts(); 
                printCentered("已成功删除！", 12);
                Sleep(800);
            }
            return;
        }
    }
}

// 快速排序辅助函数
void swapContacts(Contact* a, Contact* b) {
    Contact temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int low, int high) {
    // 选择最右边的元素作为基准 (Pivot)
    char* pivot = contacts[high].name;
    int i = (low - 1); // 较小元素的索引

    for (int j = low; j <= high - 1; j++) {
        // 如果当前联系人姓名按字典序小于或等于基准值
        if (strcmp(contacts[j].name, pivot) <= 0) {
            i++;
            swapContacts(&contacts[i], &contacts[j]);
        }
    }
    // 放基准值
    swapContacts(&contacts[i + 1], &contacts[high]);
    return (i + 1);
}

void mySort(int low, int high) {
    if (low < high) {
        //分区
        int pi = partition(low, high);
        mySort(low, pi - 1);
        mySort(pi + 1, high);
    }
}


//二分查找
int binarysearch(const char* name)
{
    int low = 0, high = contactCount - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int res = strcmp(contacts[mid].name, name);
        if (res == 0) return mid;      // 找到了
        if (res < 0) low = mid + 1;    // 在右半边
        else high = mid - 1;           // 在左半边
    }
    return -1;
}

void searchContact() 
{
    char name[50] = ""; system("cls");
    printCentered("=== 查 询 联 系 人 ===", 5);

    showCursor(); 
    gotoxy(getConsoleWidth() / 2 - 5, 7); 
    scanf_s("%49s", name, 50); 
    while (getchar() != '\n');
    hideCursor();
    
    system("cls");
    
	mySort(0, contactCount - 1); //确保有序

    int f = binarysearch(name);
   // for (int i = 0; i < contactCount; i++) 
   //     if (strcmp(contacts[i].name, name) == 0) 
   //     {
   //         f = i; 
			//break; //找到联系人
   //     }
    if (f != -1) 
    {
        printCentered("--- 查 询 结 果 ---", 5);
        int x = (getConsoleWidth() - 30) / 2;
        gotoxy(x, 7); printf("姓  名: %s", contacts[f].name);
        gotoxy(x, 8); printf("电  话: %s", contacts[f].phone);
        gotoxy(x, 9); printf("邮  箱: %s", contacts[f].email);
    }
    else printCentered("未找到该联系人！", 8);
    drawButton("返回", getConsoleWidth() / 2 - 3, 12, 1);
    while (_getch() != '\r');
}


void sortContacts() {
    /*if (contactCount > 1)
    {
        qsort(contacts, contactCount, sizeof(Contact), [](const void* a, const void* b) {
            return strcmp(((Contact*)a)->name, ((Contact*)b)->name);
            });
    }*/
    mySort(0, contactCount - 1);
    displayContacts();
}


