#include <stdio.h>
#include <string.h>
#include "sdams.h"

// Function declarations
void login(void);
void managerLogin(void);

// Manager login
void managerLogin(void)
{
    FILE *fp;
    char username[30], password[30];
    char savedUser[30], savedPass[30];
    int found = 0;

    printf("\n===== MANAGER LOGIN =====\n");

    printf("Username: ");
    scanf("%29s", username);

    printf("Password: ");
    scanf("%29s", password);

    fp = fopen("manager.txt", "r");

    if (fp == NULL) {
        printf("Manager login file not found.\n");
        return;
    }

    while (fscanf(fp, "%29s %29s",
                  savedUser, savedPass) == 2) {

        if (strcmp(username, savedUser) == 0 &&
            strcmp(password, savedPass) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found == 1) {
        printf("\nLogin successful!\n");
        managerMenu();
    }
    else {
        printf("\nInvalid username or password.\n");
    }
}

// role selection and login
void login(void)
{
    int role;
    int ch;

    printf("\n===== SELECT USER ROLE =====\n");
    printf("1. Manager\n");
    printf("2. Administrator\n");
    printf("3. Student\n");
    printf("4. Instructor\n");
    printf("5. Facilities Officer\n");
    printf("6. Back\n");

    printf("Enter your role: ");

    if (scanf("%d", &role) != 1) {
        role = 0;
    }

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    switch (role) {
        case 1:
            managerLogin();
            break;

        case 2:
            printf("Administrator module not connected yet.\n");
            break;

        case 3:
            printf("Student module not connected yet.\n");
            break;

        case 4:
            printf("Instructor module not connected yet.\n");
            break;

        case 5:
            printf("Facilities module not connected yet.\n");
            break;

        case 6:
            printf("Returning to main menu...\n");
            break;

        default:
            printf("Invalid role selected.\n");
    }
}

// Main program
int main(void)
{
    int choice;
    int ch;

    do {
        printf("\n======= WELCOME TO SDAMS =======\n");
        printf("1. Login\n");
        printf("2. Exit\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            choice = 0;
        }

        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        switch (choice) {
            case 1:
                login();
                break;

            case 2:
                printf("Thank you for using SDAMS!\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 2);

    return 0;
}