#pragma once

#define USER_FILE "users.dat"
#define MAX_USERS 50

typedef struct {
    char username[50];
    char password[50];
} User;

extern char currentLoggedInUser[50];

void loadUsers();
void saveUsers();
int accountSystem();
void registerUser();
void passwordInput(char* password, int maxLen, int x, int y);