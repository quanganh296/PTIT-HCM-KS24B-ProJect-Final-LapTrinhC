#ifndef FUNCTION_H
#define FUNCTION_H

#include "datatype.h"  

// hien thi 
void showUsers();
void saveUsersToFile();
void loadUsersFromFile();
void displayRoleChoice();
void displayRoleMenu();
void displayUsers();
void addUser();
void deleteUser();
void toggleUserStatus();
int userExists(const char *userId);
void showUserDetail();
void userGuideline();
void aboutUs();
void menuSortUsers();
// Sorting Functions
void sortUsersByName();
void sortUsersByNameDescending();
void sortUsersByID();
void sortUsersByEmail();
void sortUsersByPhone();
void printUsers();

// Delete User
void displayDeleteUsers();
void deleteUser();
void deleteAllUsers();
// Menu
void adminMenu();
void userMenu();

// Login
void inputMaskedPassword();
void adminLogin();
void userLogin();
//change user info
void changeUserInfo();
void resetUserData();
//User management
void UserMoneyManagement();
#endif
