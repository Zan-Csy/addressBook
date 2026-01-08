#pragma once

#define MAX_CONTACTS 100        // 最大联系人数量

// 联系人结构
typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;  

extern Contact contacts[MAX_CONTACTS];  // 联系人数组
extern int contactCount;        // 当前联系人数量

void loadContacts();        // 加载联系人数据
void saveContacts();        // 保存联系人数据
void showMenu(int currentChoice);       // 显示主菜单
void addContact();      // 添加联系人
void deleteContact();       // 删除联系人
void searchContact();       // 搜索联系人
void displayContacts();     // 显示所有联系人
void sortContacts();        // 排序联系人