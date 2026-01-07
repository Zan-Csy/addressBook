#pragma execution_character_set("utf-8")
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "common.h"

void gotoxy(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO c = { 1, FALSE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

void showCursor() {
    CONSOLE_CURSOR_INFO c = { 1, TRUE };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void printCentered(const char* text, int y) {
    int width = getConsoleWidth();
    int visualLen = 0;
    for (int i = 0; text[i]; ) {
        if ((unsigned char)text[i] >= 0x80) { visualLen += 2; i += 3; }
        else { visualLen += 1; i += 1; }
    }
    int x = (width - visualLen) / 2;
    gotoxy(x < 0 ? 0 : x, y);
    printf("%s", text);
}

void drawButton(const char* text, int x, int y, int isSelected) {
    gotoxy(x, y);
    if (isSelected) {
        setColor(COLOR_RED);
        printf("> %s <", text);
    }
    else {
        setColor(COLOR_WHITE);
        printf("  %s  ", text);
    }
    setColor(COLOR_WHITE);
}

int handleNavigation(char key, int* curRow, int* curCol, int maxRows, int maxCols) {
    if (key == -32 || key == 0) { // 处理方向键
        char subKey = _getch();
        if (subKey == 72) key = 'w';      // Up
        else if (subKey == 80) key = 's'; // Down
        else if (subKey == 75) key = 'a'; // Left
        else if (subKey == 77) key = 'd'; // Right
    }
    if (key >= 'A' && key <= 'Z') key += 32;
    switch (key) {
    case 'w': *curRow = (*curRow - 1 + maxRows) % maxRows; return 1;
    case 's': *curRow = (*curRow + 1) % maxRows; return 1;
    case 'a': *curCol = (*curCol - 1 + maxCols) % maxCols; return 1;
    case 'd': *curCol = (*curCol + 1) % maxCols; return 1;
    }
    return 0;
}

int isValidPhone(const char* phone) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++) if (phone[i] < '0' || phone[i] > '9') return 0;
    return 1;
}

int isValidEmail(const char* email) {
    const char* at = strchr(email, '@');
    const char* dot = strstr(email, ".com");
    return (at && dot && at < dot && at > email);
}