#pragma execution_character_set("utf-8")
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "account.h"
#include "common.h"

User users[MAX_USERS];
int userCount = 0;
char currentLoggedInUser[50] = "";

void loadUsers() {
    FILE* f; userCount = 0;
    if (fopen_s(&f, USER_FILE, "rb") == 0) {
        fread(&userCount, sizeof(int), 1, f);
        if (userCount > 0) fread(users, sizeof(User), userCount, f);
        fclose(f);
    }
}

void saveUsers() {
    FILE* f;
    if (fopen_s(&f, USER_FILE, "wb") == 0) {
        fwrite(&userCount, sizeof(int), 1, f);
        fwrite(users, sizeof(User), userCount, f);
        fclose(f);
    }
}

void passwordInput(char* password, int maxLen, int x, int y) {
    int i = 0; char ch;
    gotoxy(x, y); printf("                "); gotoxy(x, y); showCursor();
    while (i < maxLen - 1) {
        ch = _getch();
        if (ch == '\r') break;
        if (ch == '\b' && i > 0) { i--; printf("\b \b"); }
        else if (ch >= 32 && ch <= 126) { password[i++] = ch; printf("*"); }
    }
    password[i] = '\0'; hideCursor();
}

void registerUser() {
    User tmp = { "", "" }; char conf[50] = "";
    int curR = 0, curC = 0, startY = 6, width = getConsoleWidth();
    int labelX = width / 2 - 15;

    while (1) {
        system("cls"); hideCursor();
        printCentered("--- 账 户 注 册 ---", startY - 3);
        gotoxy(labelX, startY); if (curR == 0) setColor(COLOR_RED);
        printf(curR == 0 ? "> 用户名: %s <" : "  用户名: %s", tmp.username); setColor(COLOR_WHITE);

        gotoxy(labelX, startY + 2); if (curR == 1) setColor(COLOR_RED);
        printf(curR == 1 ? "> 密  码: %s <" : "  密  码: %s", strlen(tmp.password) > 0 ? "******" : ""); setColor(COLOR_WHITE);

        gotoxy(labelX, startY + 4); if (curR == 2) setColor(COLOR_RED);
        printf(curR == 2 ? "> 确  认: %s <" : "  确  认: %s", strlen(conf) > 0 ? "******" : ""); setColor(COLOR_WHITE);

        if (strlen(tmp.password) > 0 && strlen(conf) > 0) {
            gotoxy(labelX + 10, startY + 5);
            if (strcmp(tmp.password, conf) == 0) { setColor(COLOR_GREEN); printf("√ 密码匹配"); }
            else { setColor(COLOR_DARK_RED); printf("× 两次密码不一致"); }
            setColor(COLOR_WHITE);
        }

        drawButton("保 存", width / 2 - 12, startY + 8, curR == 3 && curC == 0);
        drawButton("取 消", width / 2 + 4, startY + 8, curR == 3 && curC == 1);

        char ch = _getch(); handleNavigation(ch, &curR, &curC, 4, curR == 3 ? 2 : 1);
        if (ch == '\r') {
            if (curR == 0) { showCursor(); gotoxy(labelX + 10, startY); scanf_s("%49s", tmp.username, 50); while (getchar() != '\n'); hideCursor(); }
            else if (curR == 1) passwordInput(tmp.password, 50, labelX + 10, startY + 2);
            else if (curR == 2) passwordInput(conf, 50, labelX + 10, startY + 4);
            else if (curR == 3) {
                if (curC == 1) return;
                if (strlen(tmp.username) > 0 && strcmp(tmp.password, conf) == 0) {
                    users[userCount++] = tmp; saveUsers();
                    printCentered("注册成功！", startY + 10); Sleep(1000); return;
                }
            }
        }
    }
}

int accountSystem() {
    loadUsers();
    char inU[50] = "", inP[50] = "";
    int curR = 0, curC = 0, startY = 8, width = getConsoleWidth();
    int labelX = width / 2 - 12;

    while (1) {
        system("cls"); hideCursor();
        printCentered("=== 系 统 登 录 ===", startY - 4);
        gotoxy(labelX, startY); if (curR == 0) setColor(COLOR_RED);
        printf(curR == 0 ? "> 用户名: %s <" : "  用户名: %s", inU); setColor(COLOR_WHITE);

        gotoxy(labelX, startY + 2); if (curR == 1) setColor(COLOR_RED);
        char mk[51] = ""; for (int i = 0; i < (int)strlen(inP); i++) strcat(mk, "*");
        printf(curR == 1 ? "> 密  码: %s <" : "  密  码: %s", mk); setColor(COLOR_WHITE);

        drawButton("登 录", width / 2 - 12, startY + 5, (curR == 2 && curC == 0));
        drawButton("注 册", width / 2 + 2, startY + 5, (curR == 2 && curC == 1));
        drawButton("退出系统", (width - 10) / 2, startY + 8, (curR == 3));

        char ch = _getch(); handleNavigation(ch, &curR, &curC, 4, curR == 2 ? 2 : 1);
        if (ch == '\r') {
            if (curR == 0) { showCursor(); gotoxy(labelX + 10, startY); scanf_s("%49s", inU, 50); while (getchar() != '\n'); hideCursor(); }
            else if (curR == 1) passwordInput(inP, 50, labelX + 10, startY + 2);
            else if (curR == 2) {
                if (curC == 0) {
                    int idx = -1;
                    for (int i = 0; i < userCount; i++) if (strcmp(users[i].username, inU) == 0) { idx = i; break; }
                    if (idx == -1) { setColor(COLOR_DARK_RED); printCentered("错误：用户不存在！", startY + 10); setColor(COLOR_WHITE); _getch(); }
                    else if (strcmp(users[idx].password, inP) != 0) { setColor(COLOR_DARK_RED); printCentered("错误：密码错误！", startY + 10); setColor(COLOR_WHITE); _getch(); }
                    else { strcpy_s(currentLoggedInUser, inU); return 1; }
                }
                else registerUser();
            }
            else if (curR == 3) return 0;
        }
    }
}