#pragma once

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

extern Contact contacts[MAX_CONTACTS];
extern int contactCount;

void loadContacts();
void saveContacts();
void showMenu(int currentChoice);
void addContact();
void deleteContact();
void searchContact();
void displayContacts();
void sortContacts();