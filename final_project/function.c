
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h> 
#include "datatype.h"
#include "function.h"
#define MAX_USERS 100
#define DATA_FILE "user.bin"




AccountInfo info[MAX_USERS];
User users[MAX_USERS];
int userCount = 0;
int userExists(const char *userId) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            return 1; 
        }
    }
    return 0; 
}
//sort user
int compareByName(const void *a, const void *b) {
    return strcmp(((User *)a)->name, ((User *)b)->name);
}
int compareByNameDescending(const void *a, const void *b) {
    return strcmp(((User *)b)->name, ((User *)a)->name); // Reverse order
}

// limit of input
int isValidPhone(const char *phone) {
    if (strlen(phone) < 10 || strlen(phone) > 11) return 0;
    for (int i = 0; i < strlen(phone); i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int isValidId(const char *userId) {
    if (strlen(userId) < 3 || strlen(userId) > 10) return 0;
    return 1;
}

int isValidEmail(const char *email) {
    if (strlen(email) < 3 || strlen(email) > 50) return 0;
    if (!strchr(email, '@') || !strchr(email, '.')) return 0;
    return 1;
}

int isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2025) return 0;
    if (month < 1 || month > 12) return 0;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
        daysInMonth[1] = 29;  
    }
    return (day >= 1 && day <= daysInMonth[month - 1]);
}
int isValidPasswordAdmin(const char *password, const char *phone) {
    //ktra do dai mk
    if (strlen(password) != strlen(phone)) {
        return 0; 
    }

    //ktra matkhau giong sdt hay ko
    if (strcmp(password, phone) != 0) {
        return 0; 
    }

    //ktra cac ky tu cua mk co phai so hay ko
    for (int i = 0; i < strlen(password); i++) {
        if (!isdigit(password[i])) {
            return 0; 
        }
    }

    return 1;
}




void saveUsersToFile() {
    FILE *file = fopen(DATA_FILE, "wb"); // Open file in write-binary mode
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    fwrite(info, sizeof(AccountInfo), userCount, file);

    // Debug: Print data being saved
    printf("Saving %d users to file\n", userCount);
    for (int i = 0; i < userCount; i++) {
        printf("User %d: ID=%s, Email=%s, Password=%s\n", i, users[i].userId, users[i].email, info[i].password);
    }

    fclose(file);
}

void loadUsersFromFile() {
    FILE *file = fopen(DATA_FILE, "rb"); // Open file in read-binary mode
    if (!file) {
        printf("No user data found. Initializing empty user list.\n");
        userCount = 0;
        return;
    }

    // Read the number of users
    size_t items_read = fread(&userCount, sizeof(int), 1, file);
    if (items_read != 1 || userCount < 0 || userCount > MAX_USERS) {
        printf("Warning: Invalid user count (%d) in file. Resetting...\n", userCount);
        fclose(file);
        userCount = 0;
        return;
    }

    // Read the user data
    items_read = fread(users, sizeof(User), userCount, file);
    if (items_read != userCount) {
        printf("Warning: Expected %d users but read %zu. Adjusting...\n", userCount, items_read);
        userCount = items_read;
    }

    // Read the account info data
    items_read = fread(info, sizeof(AccountInfo), userCount, file);
    if (items_read != userCount) {
        printf("Warning: Expected %d account info records but read %zu. Adjusting...\n", userCount, items_read);
    }

    fclose(file);

    // Debug: Print loaded users
    printf("Loaded %d users from file.\n", userCount);
    for (int i = 0; i < userCount; i++) {
        printf("User %d: ID=%s, Email=%s, Password=%s\n", i, users[i].userId, users[i].email, info[i].password);
    }

    // Validate all loaded users
    int valid_count = 0;
    for (int i = 0; i < userCount; i++) {
        // Basic validation: Check if user has valid date and non-empty ID
        if (strlen(users[i].userId) > 0 && 
            isValidDate(users[i].dateOfBirth.day, 
                        users[i].dateOfBirth.month, 
                        users[i].dateOfBirth.year)) {
            // If valid, keep it (potentially moving it earlier in the array)
            if (i != valid_count) {
                users[valid_count] = users[i];
                info[valid_count] = info[i];
            }
            valid_count++;
        } else {
            printf("Warning: Found corrupted user data for ID: %s\n", users[i].userId);
        }
    }

    if (valid_count != userCount) {
        printf("Warning: %d out of %d users had invalid data and were removed.\n", userCount - valid_count, userCount);
        userCount = valid_count;
        saveUsersToFile(); // Save the cleaned-up data
    }
}
void resetUserData() {
    // Clear all user data
  userCount = 0;
    memset(users, 0, sizeof(users));
    
    // Save the empty user array
    saveUsersToFile();
    printf("User data has been reset. You can now add new users.\n");
}
// MENU CUA NGUOi DUNG
void userMenu() {
    int userChoice;
    do {  
        system("cls");
        printf("\n*** Customers Management System Using C ***\n");
        printf("\n%15s USER MENU\n", "");
        printf("%8s============================\n", "");
        printf("%8s[1] Show user details\n", "");
        printf("%8s[2] Change user info\n", "");
        printf("%8s[3] User money management\n","");
        printf("%8s[4] User Guideline\n", "");
        printf("%8s[5] About Us\n", "");
        printf("%8s[0] Back to role menu\n", "");
        printf("%8s============================\n", "");
        printf("%8sEnter your choice: ", "");
        
        scanf("%d", &userChoice);
        while (getchar() != '\n'); 
        
        switch (userChoice) {
            case 1:
                showUserDetail();
              
                break;
            case 2:
                showUserDetailToChange();
               
                break;
            case 3:
				UserMoneyManagement();
            	break;
            case 4:
                userGuideline();
               
                break;
            case 5:
                aboutUs();
                
                break;
            case 0:
                printf("\nPress Enter to return to menu...");
				getchar();
                break;
            default:
                printf("Invalid option! Please try again.\n");
                printf("\nPress Enter to continue...");
                getchar();
        }
    } while (userChoice != 0);
}


void displayRoleMenu() {
    int roleChoice;
    do { 
        system("cls");
        printf("\n*** Bank Management System Using C ***\n");
        printf("\n%9s CHOOSE YOUR ROLE\n","");
        printf("%5s========================\n","");
        printf("%5s[1] Admin.\n","");
        printf("%5s[2] User.\n","");
        printf("%5s[0] Exit the Program.\n","");
        printf("%5s========================\n","");
        printf("%5sEnter Your Choice: ","");
        scanf("%d", &roleChoice);
        while (getchar() != '\n'); // Clear buffer

        switch (roleChoice) {
            case 0:
                system("cls");
                printf("%8s========= See You Soon ======\n", "");
                printf("%8s========= Thank You =========\n","");
                exit(0);
            case 1:
                adminLogin(); 
                break;
            case 2:
            	userLogin();
                break;
            default:
                printf("Invalid choice! Please choose again!\n");
                Sleep(1000);
        }

    } while (roleChoice != 0);
}

void adminMenu(){
    int choice;
    do {  
        system("cls");
        printf("\n***Customers Management System Using C***\n","");
        printf("\n%15sMENU\n","");
        printf("%8s========================\n","");
        printf("%8s[1] Add A New user.\n","");
        printf("%8s[2] Show All users.\n","");
        printf("%8s[3] Show detail an user.\n","");
        printf("%8s[4] Lock (Unlock) an user.\n","");
        printf("%8s[5] User Guideline.\n","");
        printf("%8s[6] About Us.\n","");
        printf("%8s[0] Exit the Program.\n","");
        printf("%8s========================\n","");
        printf("%8sEnter The Choice: ","");
        scanf("%d", &choice);
        while (getchar() != '\n');
       
        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                displayUsers();
                break;
            case 3:
                showUserDetail();
                break;
            case 4:
                toggleUserStatus();
                break;
            case 5:
                userGuideline();
                break;
            case 6:
                aboutUs();
                break;
            
            case 0:
                system("cls");
                printf("%8s========= Thank You =========\n","");
                printf("%8s========= See You Soon ======","");
                break;
            default:
                printf("Invalid option! Try again.\n");
        }
    } while (choice != 0);
}

void sortUsersByName() {
    qsort(users, userCount, sizeof(User), compareByName);
	printf("Sorted by Name (A-Z) successfully!\n");
}
void sortUsersByNameDescending() {
    qsort(users, userCount, sizeof(User), compareByNameDescending);
    printf("Sorted by Name (Z-A) successfully!\n");
}
void menuSortUsers(){
    
system("cls");
   

    int choice;
    printf("\n*** Display Users ***\n");
    printf("============================\n");
    printf("[1] Sort Users by Name (A-Z)\n");
    printf("[2] Sort Users by Name (Z-A)\n");
    printf("[0] Back to display users menu\n");
    printf("============================\n");
    printf("Enter Your Choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); 

    switch (choice) {
        case 1:
          sortUsersByName(); 
            break; 
        case 2:
            sortUsersByNameDescending();
            break;
        case 0:
            return;  
        default:
            printf("Invalid option! Choose again!\n");
    }

    printf("Press ENTER to go back...");
    while (getchar() != '\n');
}



void addUser() {
	User newUser;
	  
    if (userCount >= MAX_USERS) {
        printf("User list is full!\n");
        return;
    }
    
    // Initialize all fields to zero first
    memset(&newUser, 0, sizeof(User));
    
    char temp[100];
    
    do {
        system("cls");
        printf("Enter ID(>=3 letters): ");
        fgets(newUser.userId, sizeof(newUser.userId), stdin);
        newUser.userId[strcspn(newUser.userId, "\n")] = 0;

        if (strlen(newUser.userId) == 0) {
            printf("User ID cannot be empty. Please enter again!\n");
            	 Sleep(1000);
        } else if (userExists(newUser.userId)) {
        
            printf("User ID already exists! Enter a different one.\n");
            	 Sleep(1000);
        }
        if (!isValidId(newUser.userId)) {
            printf("Invalid ID! ID must be 3-10 digits.\n");
            	 Sleep(1000);
        }
    } while (strlen(newUser.userId) == 0 || userExists(newUser.userId) || !isValidId(newUser.userId));

    do {
        printf("Enter Name: ");
        fgets(newUser.name, sizeof(newUser.name), stdin);
        newUser.name[strcspn(newUser.name, "\n")] = '\0';

        if (strlen(newUser.name) == 0) {
            printf("Name cannot be empty. Please enter again!\n");
        }
    } while (strlen(newUser.name) == 0);

  do {
      printf("Enter Username: ");
      fgets(info[userCount].username, sizeof(info[userCount].username), stdin);
      info[userCount].username[strcspn(info[userCount].username, "\n")] = '\0';
  
      if (strlen(info[userCount].username) == 0) {
          printf("Username cannot be empty. Please enter again!\n");
      }
  } while (strlen(info[userCount].username) == 0);
  
    do {
        printf("Enter Phone: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;

        if (!isValidPhone(temp)) {
            printf("Invalid phone number! Phone number must be 10-11 digits.\n");
        }
    } while (!isValidPhone(temp));
    strcpy(newUser.phone, temp);
strcpy(info[userCount].password, temp);
    do {
        printf("Enter Email: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;

        if (!isValidEmail(temp)) {
            printf("Invalid email format! Example: user@example.com\n");
        }
    } while (!isValidEmail(temp));
    strcpy(newUser.email, temp);

    int day, month, year;
    do {
        printf("Enter the date of birth:\n");
        printf("  Enter the day (1-31): ");
        if (scanf("%d", &day) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            day = 0; // Reset to invalid value
            continue;
        }

        printf("  Enter the month (1-12): ");
        if (scanf("%d", &month) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            month = 0; // Reset to invalid value
            continue;
        }

        printf("  Enter the year (1900-2025): ");
        if (scanf("%d", &year) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            year = 0; // Reset to invalid value
            continue;
        }

        if (!isValidDate(day, month, year)) {
            printf("Invalid date! Please enter a correct date.\n");
        }
    } while (!isValidDate(day, month, year));

    // Store the validated date
    newUser.dateOfBirth.day = day;
    newUser.dateOfBirth.month = month;
    newUser.dateOfBirth.year = year;

    while (getchar() != '\n');  // Clear input buffer

    // Set default status (unlocked)
    newUser.status = 0;
users[userCount++] = newUser;
	    saveUsersToFile();
    printf("Add User successfully! Press Enter to exit");
    while (getchar() != '\n'); 
}      
//menu xoa nguoi dung

void displayDeleteUsers() {
    int choice;
    do {
        system("cls");
        printf("\n*** DELETE USERS MENU ***\n");
        printf("[1] Delete a user\n");
        printf("[2] Delete all users\n");
        printf("[3] Back to menu\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) { 
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }
        
        switch (choice) {
            case 1:
                deleteUser();
                break;
            case 2: {  // Add braces around the declaration
                char confirm;
                printf("WARNING: This will erase ALL user data. Are you sure? (y/n): ");
                scanf(" %c", &confirm);
                while (getchar() != '\n');
                
                if (confirm == 'y' || confirm == 'Y') {
                    resetUserData();
                } else {
                    printf("Reset operation cancelled.\n");
                }
                printf("Press Enter to continue...");
                getchar();
                break;
            }
            case 3:
                return;  // Exit menu
            default:
                printf("Invalid choice! Please enter again.\n");
                break;
        }
    } while (choice != 3);
}

void deleteUser() {
    char userId[20];

    printf("Enter User ID to delete: ");
    while (getchar() != '\n'); 
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            found = 1;
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            saveUsersToFile();
            printf("User %s deleted successfully!\n", userId);
            break;
        }
    }

    if (!found) {
        printf("User ID not found!\n");
    }

    printf("Press ENTER to go back...");
    while (getchar() != '\n');
    getchar();
}



//menu hien thi nguoi dung
void displayUsers() {
	system("cls");
    if (userCount == 0) {
        printf("No users to display.\n");
        printf("Press ENTER to go back...");
        while (getchar() != '\n');
        return;
    }

    int choice;
    printf("\n*** Display Users ***\n");
    printf("============================\n");
    printf("[1] Show Users\n");
    printf("[2] Sort Users By Name\n");
    printf("[3] Delete Users \n");
    printf("[0] Back to Main Menu\n");
    printf("============================\n");
    printf("Enter Your Choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); 

    switch (choice) {
        case 1:
        	showUsers();
            break; 
        case 2:
            menuSortUsers();
            break;
            case 3:
        	displayDeleteUsers();
        	break;
        case 0:
            return;  
        default:
            printf("Invalid option! Showing unsorted list...\n");
    }


  
}



void toggleUserStatus() {
    char choice;
     int found = 0;
    do {
    
        char userId[20];
        printf("Enter User ID to Lock/Unlock: ");
        fgets(userId, sizeof(userId), stdin);
        userId[strcspn(userId, "\n")] = 0;
       
       
       
         for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].userId, userId) == 0) {
            	 found = 1;
              printf("One user was found for ID: %s\n", users[i].userId);
                printf("User %s status was %s, %s user? \n", userId, users[i].status ? "Locked" : "Unlocked",users[i].status ? "unlock" : "lock");
       
                  printf("Yes(y)? or No(n)?: ");
            char choice;
            scanf(" %c", &choice);
            while (getchar() != '\n'); 

            if (choice == 'y' || choice == 'Y') {
                    users[i].status = !users[i].status;
                    saveUsersToFile();
                    printf("User %s successfully %s.\n",
                           users[i].userId,
                           users[i].status ? "Locked" : "Unlocked");
                } else {
                    printf("Operation canceled!\n");
                }
    
    
   
    	
    	break;
	}
	}
       if(!found){
    	printf("No user was found for ID: %s \n",userId);
   }

        printf("Press 'm' to go back to the main menu: ");
        scanf(" %c", &choice);
    } while (choice != 'm');
}

void userGuideline() {
char choice;
do{
system("cls");
    printf("\n%8sUser Guideline:\n");
    printf("%8s1. Use option 1 to add a new user only active with admin role.\n","");
    printf("%8s2. Use option 2 to view all users and make change only active with admin role.\n","");
    printf("%8s3. Use option 3 to view a specific user details.\n","");
    printf("%8s4. Use option 4 to lock/unlock users only active with admin role.\n","");
    printf("%8s5. Use option 6 to know more about us (The creator)\n","");
    printf("%8s========================================================================================\n","");

       
        printf("%8sGo back(b)? or Exit(0)?: ","");
        scanf(" %c", &choice);
        while (getchar() != '\n'); 

        if (choice == '0') {
        	 system("cls");
    printf("%8s========= Thank You =========\n", "");
printf("%8s========= See You Soon ======\n", "");
            exit(0);
        }
    } while (choice != 'b');
}
void aboutUs() {
    char choice;  
    do {
    	system("cls");
        printf("\n%8sAbout Us:\n", "");
        printf("%8sThis is a program made by Nguyen Quang Anh using the C language.\n", "");
        printf("%8sIt allows user registration, viewing details, and managing access.\n", "");
        printf("%8s========================================================================================\n", "");
       
        printf("%8sGo back (b)? or Exit (0)?: ", "");
        scanf(" %c", &choice);
        while (getchar() != '\n'); 

        if (choice == '0') {
           system("cls");

            printf("%8s========= Thank You =========\n", "");
            printf("%8s========= See You Soon ======\n", "");
            exit(0);
        }
    } while (choice != 'b');
}
void showUserDetail() {
    system("cls");
    fflush(stdin);
    if (userCount == 0) {
        printf("%8s%s", "", "No users available.\n\n");
        return;
    }

    char userId[20]; 
    printf("%8s%s", "", "Enter User ID to view details: ");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0'; 

    int found = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            found = 1;
            printf("%8s%s", "", "===========================================\n");
            printf("%8s%s", "", "             USER DETAILS\n");
            printf("%8s%s", "", "===========================================\n");
            printf("%11s%s\n", "ID:", users[i].userId);
            printf("%13s%s\n", "Name:", users[i].name);
             printf("%17s%s\n", "Username:", info[i].username);
            printf("%8s%s\n", "", "Date of Birth:");
            printf("%19s%d\n", "Day:", users[i].dateOfBirth.day);
            printf("%19s%d\n", "Month:", users[i].dateOfBirth.month);
            printf("%19s%d\n", "Year:", users[i].dateOfBirth.year);
            printf("%14s%s\n", "Phone:", users[i].phone);
            printf("%14s%s\n", "Email:", users[i].email);
            printf("%15s%s\n", "Status:", users[i].status ? "Locked" : "Unlocked");
            printf("%10s%s", "", "===========================================\n");
            break;
        }
    }

    if (!found) {
        printf("%8s%s", "", "User ID not found or invalid(ID must be 3-10 digits)!\n");
    }

    printf("%8s%s", "", "Press ENTER to go back...");
    while (getchar() != '\n');
}
void showUserDetailToChange() {
    system("cls");
    fflush(stdin);
    if (userCount == 0) {
        printf("%8s%s", "", "No users available.\n\n");
        return;
    }

    char userId[20]; 
    printf("%8s%s", "", "Enter User ID to view details to change: ");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0'; 

    int found = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            found = 1;
            printf("%8s%s", "", "===========================================\n");
            printf("%8s%s", "", "            CHANGE USER DETAILS\n");
            printf("%8s%s", "", "===========================================\n");
            printf("%11s%s\n", "ID:", users[i].userId);
            printf("%13s%s\n", "Name:", users[i].name);
            printf("%17s%s\n", "Username:", info[i].username);
            printf("%8s%s\n", "", "Date of Birth:");
            printf("%19s%d\n", "Day:", users[i].dateOfBirth.day);
            printf("%19s%d\n", "Month:", users[i].dateOfBirth.month);
            printf("%19s%d\n", "Year:", users[i].dateOfBirth.year);
            printf("%14s%s\n", "Phone:", users[i].phone);
            printf("%14s%s\n", "Email:", users[i].email);
            printf("%15s%s\n", "Status:", users[i].status ? "Locked" : "Unlocked");
            printf("%10s%s", "", "===========================================\n");

            // Call changeUserInfo function to change user details
            changeUserInfo(i);
            break;
        }
    }

    if (!found) {
        printf("%8s%s", "", "User ID not found or invalid(ID must be 3-10 digits)!\n");
    }

    printf("%8s%s", "", "Press ENTER to go back...");
    while (getchar() != '\n');
}


void showUsers() {
	 system("cls");
	 int choice;
     printf("\n=============+======================+===========================+=================+=========\n");
    printf("| %-10s | %-20s | %-25s | %-15s | %-5s |\n", "ID", "Name", "Email", "Phone", "Status");
    printf("===========================================================================================\n");
    for (int i = 0; i < userCount; i++) {
    printf("| %-10s | %-20s | %-25s | %-15s | %-6s |\n", 
            users[i].userId, users[i].name, users[i].email, users[i].phone, users[i].status ? "Lock" : "Open");
    }
     printf("=============+======================+===========================+=================+=========\n");
do {
    printf("Go back(b)? or Exit(0)?: ");
    choice = getchar();
    while (getchar() != '\n');
    if (choice == '0') {
    	system("cls");
    printf("%8s========= Thank You =========\n", "");
printf("%8s========= See You Soon ======\n", "");

        exit(0);
    }
} while (choice != 'b');
}
//DANG NHAP CUA ADMIN 
void inputMaskedPassword(char *password, int maxLength) {
    char ch;
    int i = 0;

    while (i < maxLength - 1) {
        ch = _getch(); // Lay mot ky tu nhung khong hien thi

        if (ch == 13) // Neu nhan Enter -> ket thuc nhap
            break;
        else if (ch == 8 && i > 0) { // Neu nhan Backspace -> xoa ky tu
            printf("\b \b"); // Xóa ky tu tren man hinh
            i--;
        } else if (ch != 8) { // Neu la ky tu hop le
            password[i++] = ch;
            printf("%c", ch); // Hien thi ky tu thuc te
            Sleep(200); // Cho 1 giây
            printf("\b*"); // Thay the ky tu bang '*'
        }
    }
    password[i] = '\0'; // Ket thuc chuoi
}


void adminLogin() {
    char inputEmail[50];
    char inputPassword[50];
    char correctEmail[] = "qn50606@gmail.com";
    char correctPassword[] = "0825862962";

    system("cls");

    // Lap den khi nhap dung email
    do {
        system("cls");
        printf("\n\n\n\n\n\n");
        printf("%45sAdmin email: qn50606@gmail.com\n","");
        printf("%45sAdmin pass: 0825862962\n","");
        printf("%45s============ LOGIN ============\n", "");

        printf("%45sEnter admin email: ", "");
        fgets(inputEmail, sizeof(inputEmail), stdin);
        inputEmail[strcspn(inputEmail, "\n")] = '\0';

        if (strcmp(inputEmail, correctEmail) != 0) {
            printf("%45sInvalid email! Please try again.\n", "");
            Sleep(1000);
        }
    } while (strcmp(inputEmail, correctEmail) != 0);

    // Lap den khi nhap dung mat khau
    do {
        printf("%45sEnter admin password: ", "");
        inputMaskedPassword(inputPassword, sizeof(inputPassword));
        printf("\n");

        if (strcmp(inputPassword, correctPassword) != 0) {
            printf("%45sIncorrect password! Please try again.\n", "");
            Sleep(1000);
        }
    } while (strcmp(inputPassword, correctPassword) != 0);

    printf("%45sLogin successful!\n", "");
    Sleep(2000);
    adminMenu();
}




void userLogin() {
    char inputEmail[50];
    char inputPassword[50];
    int foundUser = -1;
    char choice;

    // Load user data from file
    loadUsersFromFile();

    // Debug: Print loaded users
    printf("Loaded %d users from file.\n", userCount);
    for (int i = 0; i < userCount; i++) {
        printf("User %d: Email: %s, Password: %s\n", i, users[i].email, info[i].password);
    }

    while (1) {  // Infinite loop until login is successful or user exits
        system("cls");
        printf("\n\n\n\n\n\n");
        printf("%45s============ USER LOGIN ============\n", "");
        
        printf("%45sEnter email: ", "");
        fgets(inputEmail, sizeof(inputEmail), stdin);
        inputEmail[strcspn(inputEmail, "\n")] = '\0';
        
        // Debug: Print input email
        printf("Input Email: %s\n", inputEmail);
        
        // Check if user wants to exit right after entering email
        if (strcmp(inputEmail, "exit") == 0) {
            return;  // Return to role menu
        }
        
        printf("%45sEnter password: ", "");
        inputMaskedPassword(inputPassword, sizeof(inputPassword));
        inputPassword[strcspn(inputPassword, "\r\n")] = '\0';
        printf("\n");
        
        // Debug: Print input password
        printf("Input Password: %s\n", inputPassword);
        
        // Check if user wants to exit after entering password
        if (strcmp(inputPassword, "exit") == 0) {
            return;  // Return to role menu
        }
        
        // Find user by email
        foundUser = -1;
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].email, inputEmail) == 0) {
                if (users[i].status) {
                    printf("%45sThis account is locked. Please contact admin.\n", "");
                    Sleep(2000);
                    printf("%45sPress any key to try again or 'b' to go back to role menu: ", "");
                    choice = getchar();
                    while (getchar() != '\n');  // Clear input buffer
                    
                    if (choice == 'b' || choice == 'B') {
                        return;  // Back to role menu
                    }
                    break;  // Break out of the for loop, but stay in the while loop
                }
                
                if (strcmp(info[i].password, inputPassword) == 0) {
                    foundUser = i;
                    break;
                }
            }
        }
        
        if (foundUser != -1) {
            printf("%45sLogin successful! Welcome, %s\n", "", users[foundUser].name);
            Sleep(2000);
            userMenu();
            return;  // After user menu, return to role menu
        } else if (foundUser == -1 && strcmp(inputEmail, "exit") != 0 && strcmp(inputPassword, "exit") != 0) {
            printf("%45sInvalid email or password!\n", "");
            printf("%45sPress any key to try again or 'b' to go back to role menu: ", "");
            choice = getchar();
            while (getchar() != '\n');  // Clear input buffer
            
            if (choice == 'b' || choice == 'B') {
                return;  // Back to role menu
            }
        }
    }
}

void UserMoneyManagement() {
	system("cls");
    char userId[20];
    printf("Enter User ID for money management: ");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0';

    int found = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0) {
            found = 1;
            printf("User %s found!\n", userId);
            int choice;
            do {system("cls");
                printf("\n*** User Money Management ***\n");
                printf("[1] Deposit Money\n");
                printf("[2] Withdraw Money\n");
                printf("[3] Check Balance\n");
                printf("[0] Back to User Menu\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                while (getchar() != '\n'); // Clear buffer

                switch (choice) {
                    case 1: {
                        double amount;
                        printf("Enter amount to deposit: ");
                        scanf("%lf", &amount);
                        while (getchar() != '\n'); // Clear buffer
                        if (amount > 0) {
                            info[i].balance += amount;
                            printf("Deposited %.2lf successfully! New balance: %.2lf\n", amount, info[i].balance);
                        } else {
                            printf("Invalid amount! Please enter a positive number.\n");
                        }
                        break;
                    }
                    case 2: {
                        double amount;
                        printf("Enter amount to withdraw: ");
                        scanf("%lf", &amount);
                        while (getchar() != '\n'); // Clear buffer
                        if (amount > 0 && amount <= info[i].balance) {
                            info[i].balance -= amount;
                            printf("Withdrew %.2lf successfully! New balance: %.2lf\n", amount, info[i].balance);
                        } else {
                            printf("Invalid amount! Please enter a positive number less than or equal to the balance.\n");
                        }
                        break;
                    }
                    case 3:
                        printf("Current balance: %.2lf\n", info[i].balance);
                        break;
                    case 0:
                        return;
                    default:
                        printf("Invalid choice! Please try again.\n");
                }
            } while (choice != 0);
            break;
        }
    }

    if (!found) {
        printf("User ID not found!\n");
    }

    printf("Press ENTER to go back...");
    while (getchar() != '\n');
}
//Thay doi thong tin nguoi dung o user
void changeUserInfo(int userIndex) {
    do { 
        printf("Enter new name: ");
        fgets(users[userIndex].name, sizeof(users[userIndex].name), stdin);
        users[userIndex].name[strcspn(users[userIndex].name, "\n")] = '\0'; 
        if (strlen(users[userIndex].name) == 0) {
            printf("Name cannot be empty. Please enter again!\n");
        }
    } while (strlen(users[userIndex].name) == 0);

    do { 
        printf("Enter new Username: ");
        fgets(info[userIndex].username, sizeof(info[userIndex].username), stdin);
        info[userIndex].username[strcspn(info[userIndex].username, "\n")] = '\0'; 
        if (strlen(info[userIndex].username) == 0) {
            printf("Username cannot be empty. Please enter again!\n");
        }
    } while (strlen(info[userIndex].username) == 0);

    // Validate phone input
    char temp[100];
    do {
        printf("Enter new phone: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;

        if (!isValidPhone(temp)) {
            printf("Invalid phone number! Phone number must be 10-11 digits.\n");
        }
    } while (!isValidPhone(temp));
    strcpy(users[userIndex].phone, temp);

    // Validate email input
    do {
        printf("Enter new email: ");
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;

        if (!isValidEmail(temp)) {
            printf("Invalid email format! Example: user@example.com\n");
        }
    } while (!isValidEmail(temp));
    strcpy(users[userIndex].email, temp);

    // Validate date input
    do {
        printf("Enter new date of birth:\n");
        printf("  Enter the day: ");
        if (scanf("%d", &users[userIndex].dateOfBirth.day) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        printf("  Enter the month: ");
        if (scanf("%d", &users[userIndex].dateOfBirth.month) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        printf("  Enter the year: ");
        if (scanf("%d", &users[userIndex].dateOfBirth.year) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        if (!isValidDate(users[userIndex].dateOfBirth.day, users[userIndex].dateOfBirth.month, users[userIndex].dateOfBirth.year)) {
            printf("Invalid date! Please enter a correct date.\n");
        }
    } while (!isValidDate(users[userIndex].dateOfBirth.day, users[userIndex].dateOfBirth.month, users[userIndex].dateOfBirth.year));

    while (getchar() != '\n');  // Clear input buffer

    saveUsersToFile();
    printf("User info updated successfully!\n");
    printf("Press ENTER to go back...");
    getchar();
}

//void transaction(){
//	
//}
