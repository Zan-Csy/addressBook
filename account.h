#pragma once

#define USER_FILE "users.dat"   // 用户数据文件
#define MAX_USERS 50            // 最大用户数量

// 用户结构
typedef struct {
    char username[50];
    char password[50];
} User;

extern char currentLoggedInUser[50];        // 当前登录用户名

void loadUsers();       // 加载用户数据
void saveUsers();	   // 保存用户数据
int accountSystem();	  // 账户登录/注册系统
void registerUser();	 // 用户注册
void passwordInput(char* password, int maxLen, int x, int y);  // 密码输入（隐藏显示）