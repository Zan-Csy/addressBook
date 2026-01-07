#pragma execution_character_set("utf-8")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "addressbook.h"
#include "common.h"
#include "account.h"

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

void loadContacts() {
    char fileName[100]; sprintf(fileName, "contacts_%s.dat", currentLoggedInUser);
    FILE* f; contactCount = 0;
    if (fopen_s(&f, fileName, "rb") == 0) {
        fread(&contactCount, sizeof(int), 1, f);
        if (contactCount > 0) fread(contacts, sizeof(Contact), contactCount, f);
        fclose(f);
    }
}

void saveContacts() {
    char fileName[100]; sprintf(fileName, "contacts_%s.dat", currentLoggedInUser);
    FILE* f;
    if (fopen_s(&f, fileName, "wb") == 0) {
        fwrite(&contactCount, sizeof(int), 1, f);
        fwrite(contacts, sizeof(Contact), contactCount, f);
        fclose(f);
    }
}

void showMenu(int currentChoice) {
    int startY = 8, width = getConsoleWidth();
    int xStart = (width - 14) / 2;
    // 增加“切换账户”选项
    const char* opt[] = { "添加联系人", "删除联系人", "查询联系人", "显示所有", "排序名单", "切换账户", "退出系统" };

    // 循环次数改为 7
    for (int i = 0; i < 7; i++) {
        drawButton(opt[i], xStart, startY + i, i == currentChoice);
    }
}

void addContact() {
    Contact tmp = { "", "", "" };
    int curR = 0, curC = 0, startY = 4, width = getConsoleWidth();
    int labelX = width / 2 - 15, stP = 0, stE = 0;

    while (1) {
        system("cls"); hideCursor();
        printCentered("=== 新 建 联 系 人 ===", startY - 2);
        gotoxy(labelX, startY); if (curR == 0) setColor(COLOR_RED);
        printf(curR == 0 ? "> 姓  名: %s <" : "  姓  名: %s", tmp.name); setColor(COLOR_WHITE);

        gotoxy(labelX, startY + 3); if (curR == 1) setColor(COLOR_RED);
        printf(curR == 1 ? "> 电  话: %s <" : "  电  话: %s", tmp.phone); setColor(COLOR_WHITE);
        if (stP != 0) {
            gotoxy(labelX + 10, startY + 4);
            if (stP == 1) { setColor(COLOR_GREEN); printf("√"); }
            else { setColor(COLOR_DARK_RED); printf("× (需要10位数字)"); }
            setColor(COLOR_WHITE);
        }

        gotoxy(labelX, startY + 6); if (curR == 2) setColor(COLOR_RED);
        printf(curR == 2 ? "> 邮  箱: %s <" : "  邮  箱: %s", tmp.email); setColor(COLOR_WHITE);
        if (stE != 0) {
            gotoxy(labelX + 10, startY + 7);
            if (stE == 1) { setColor(COLOR_GREEN); printf("√"); }
            else { setColor(COLOR_DARK_RED); printf("× (格式: xxx@xx.com)"); }
            setColor(COLOR_WHITE);
        }

        drawButton("保 存", width / 2 - 12, startY + 10, curR == 3 && curC == 0);
        drawButton("取 消", width / 2 + 4, startY + 10, curR == 3 && curC == 1);

        char ch = _getch(); handleNavigation(ch, &curR, &curC, 4, curR == 3 ? 2 : 1);
        if (ch == '\r') {
            if (curR < 3) {
                showCursor(); gotoxy(labelX + 10, startY + curR * 3); printf("                ");
                gotoxy(labelX + 10, startY + curR * 3);
                if (curR == 0) scanf_s("%49s", tmp.name, 50);
                else if (curR == 1) { scanf_s("%19s", tmp.phone, 20); stP = isValidPhone(tmp.phone) ? 1 : -1; }
                else if (curR == 2) { scanf_s("%49s", tmp.email, 50); stE = isValidEmail(tmp.email) ? 1 : -1; }
                while (getchar() != '\n'); hideCursor();
            }
            else if (curR == 3) {
                if (curC == 1) return;
                if (strlen(tmp.name) > 0 && stP == 1 && stE == 1) {
                    contacts[contactCount++] = tmp; saveContacts();
                    printCentered("保存成功！", startY + 12); Sleep(800); return;
                }
                else { setColor(COLOR_DARK_RED); printCentered("错误：信息不完整或格式有误！", startY + 12); setColor(COLOR_WHITE); _getch(); }
            }
        }
    }
}

void displayContacts() {
    system("cls"); hideCursor();
    printCentered("=== 所 有 联 系 人 列 表 ===", 3);
    if (contactCount == 0) printCentered("空空如也...", 6);
    else {
        char base[200]; sprintf(base, "1. 姓名: %-10s | 电话: %-12s | 邮箱: %s", contacts[0].name, contacts[0].phone, contacts[0].email);
        int xAlign = (getConsoleWidth() - (int)strlen(base)) / 2;
        for (int i = 0; i < contactCount; i++) {
            gotoxy(xAlign < 2 ? 2 : xAlign, 6 + i);
            printf("%d. 姓名: %-10s | 电话: %-12s | 邮箱: %s", i + 1, contacts[i].name, contacts[i].phone, contacts[i].email);
        }
    }
    drawButton("返回主菜单", getConsoleWidth() / 2 - 6, 6 + contactCount + 3, 1);
    while (_getch() != '\r');
}

void deleteContact() {
    char name[50] = ""; system("cls");
    printCentered("=== 删 除 联 系 人 ===", 5);
    printCentered("请输入要删除的姓名:", 7);
    showCursor(); gotoxy(getConsoleWidth() / 2 - 5, 8); scanf_s("%49s", name, 50); while (getchar() != '\n'); hideCursor();
    int idx = -1;
    for (int i = 0; i < contactCount; i++) if (strcmp(contacts[i].name, name) == 0) { idx = i; break; }
    if (idx == -1) { printCentered("未找到该联系人！", 10); _getch(); return; }
    int col = 1;
    while (1) {
        system("cls"); printCentered("确定要删除吗？", 7);
        drawButton("确 定", getConsoleWidth() / 2 - 12, 10, col == 0);
        drawButton("取 消", getConsoleWidth() / 2 + 4, 10, col == 1);
        char ch = _getch(); int r = 0; handleNavigation(ch, &r, &col, 1, 2);
        if (ch == '\r') {
            if (col == 0) {
                for (int i = idx; i < contactCount - 1; i++) contacts[i] = contacts[i + 1];
                contactCount--; saveContacts(); printCentered("已成功删除！", 12); Sleep(800);
            }
            return;
        }
    }
}

void searchContact() {
    char name[50] = ""; system("cls");
    printCentered("=== 查 询 联 系 人 ===", 5);
    showCursor(); gotoxy(getConsoleWidth() / 2 - 5, 7); scanf_s("%49s", name, 50); while (getchar() != '\n'); hideCursor();
    system("cls"); int f = -1;
    for (int i = 0; i < contactCount; i++) if (strcmp(contacts[i].name, name) == 0) { f = i; break; }
    if (f != -1) {
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
    if (contactCount > 1) {
        qsort(contacts, contactCount, sizeof(Contact), [](const void* a, const void* b) {
            return strcmp(((Contact*)a)->name, ((Contact*)b)->name);
            });
    }
    displayContacts();
}