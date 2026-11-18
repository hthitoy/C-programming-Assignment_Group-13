#include <stdio.h>
#include <string.h>
#include "sdams.h"

struct Administrator {
    char id[20];
    char name[50];
    char username[30];
    char password[30];
    char contact[20];
    int active;
};

void clearInput(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int readNumber(void)
{
    int number;
    if (scanf("%d", &number) != 1) {
        clearInput();
        return -1;
    }
    clearInput();
    return number;
}

void addAdministrator(void)
{
    struct Administrator a, old;
    FILE *fp;
    int found = 0;

    printf("\n--- Add Administrator ---\n");
    printf("ID: "); scanf("%19s", a.id);
    printf("Name (use _ for spaces): "); scanf("%49s", a.name);
    printf("Username: "); scanf("%29s", a.username);
    printf("Password: "); scanf("%29s", a.password);
    printf("Contact: "); scanf("%19s", a.contact);
    a.active = 1;

    fp = fopen("administrators.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %49s %29s %29s %19s %d",
                      old.id, old.name, old.username, old.password,
                      old.contact, &old.active) == 6) {
            if (strcmp(a.id, old.id) == 0 ||
                strcmp(a.username, old.username) == 0) {
                found = 1;
            }
        }
        fclose(fp);
    }
    if (found == 1) {
        printf("ID or username already exists.\n");
        return;
    }

    fp = fopen("administrators.txt", "a");
    if (fp == NULL) {
        printf("Could not open administrators.txt\n");
        return;
    }
    fprintf(fp, "%s %s %s %s %s %d\n",
            a.id, a.name, a.username, a.password, a.contact, a.active);
    fclose(fp);
    printf("Administrator added successfully.\n");
}

void viewAdministrators(void)
{
    struct Administrator a;
    FILE *fp;
    int total = 0;

    fp = fopen("administrators.txt", "r");
    if (fp == NULL) {
        printf("No administrator records found.\n");
        return;
    }
    printf("\n--- Administrators ---\n");
    while (fscanf(fp, "%19s %49s %29s %29s %19s %d",
                  a.id, a.name, a.username, a.password,
                  a.contact, &a.active) == 6) {
        printf("ID: %s | Name: %s | User: %s | Contact: %s | Status: %s\n",
               a.id, a.name, a.username, a.contact,
               a.active == 1 ? "Active" : "Not active");
        total++;
    }
    fclose(fp);
    printf("Total administrators: %d\n", total);
}

void searchAdministrator(void)
{
    struct Administrator a;
    FILE *fp;
    char keyword[50];
    int found = 0;

    printf("Enter ID or exact name (use _ for spaces): ");
    scanf("%49s", keyword);
    fp = fopen("administrators.txt", "r");
    if (fp == NULL) {
        printf("No administrator records found.\n");
        return;
    }
    while (fscanf(fp, "%19s %49s %29s %29s %19s %d",
                  a.id, a.name, a.username, a.password,
                  a.contact, &a.active) == 6) {
        if (strcmp(keyword, a.id) == 0 || strcmp(keyword, a.name) == 0) {
            printf("ID: %s | Name: %s | Contact: %s | Status: %s\n",
                   a.id, a.name, a.contact,
                   a.active == 1 ? "Active" : "Not active");
            found = 1;
        }
    }
    fclose(fp);
    if (found == 0) printf("Administrator not found.\n");
}

void updateAdministrator(void)
{
    struct Administrator a;
    FILE *fp, *temp;
    char id[20];
    int found = 0, newStatus;

    printf("Enter administrator ID to update: ");
    scanf("%19s", id);
    fp = fopen("administrators.txt", "r");
    if (fp == NULL) {
        printf("No administrator records found.\n");
        return;
    }
    temp = fopen("temporary.txt", "w");
    if (temp == NULL) {
        printf("Cannot create temporary file.\n");
        fclose(fp);
        return;
    }
    while (fscanf(fp, "%19s %49s %29s %29s %19s %d",
                  a.id, a.name, a.username, a.password,
                  a.contact, &a.active) == 6) {
        if (strcmp(a.id, id) == 0) {
            found = 1;
            printf("New name (use _ for spaces): "); scanf("%49s", a.name);
            printf("New contact: "); scanf("%19s", a.contact);
            printf("New status (1=Active, 0=Not active): ");
            newStatus = readNumber();
            if (newStatus == 0 || newStatus == 1) {
                a.active = newStatus;
            } else {
                printf("Invalid status. Old status kept.\n");
            }
        }
        fprintf(temp, "%s %s %s %s %s %d\n",
                a.id, a.name, a.username, a.password, a.contact, a.active);
    }
    fclose(fp);
    if (fclose(temp) != 0) {
        printf("Error saving changes.\n");
        return;
    }
    if (found == 0) {
        remove("temporary.txt");
        printf("Administrator not found.\n");
        return;
    }
    
    if (remove("administrators.txt") != 0 ||
        rename("temporary.txt", "administrators.txt") != 0) {
        printf("Error replacing records; check temporary.txt.\n");
        return;
    }
    printf("Administrator updated.\n");
}

void manageAdministrators(void)
{
    int choice;
    do {
        printf("\n--- Manage Administrators ---\n");
        printf("1. Add\n2. View\n3. Search\n4. Update\n5. Back\n");
        printf("Choice: ");
        choice = readNumber();
        switch (choice) {
            case 1: addAdministrator(); break;
            case 2: viewAdministrators(); break;
            case 3: searchAdministrator(); break;
            case 4: updateAdministrator(); break;
            case 5: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

void staffReport(void)
{
    struct Administrator a;
    FILE *fp;
    char id[20], name[50], contact[20];
    char keyword[50];
    int status, role, admins = 0, instructors = 0;

    printf("\n--- Staff Report ---\n");
    printf("1. All staff\n2. Administrators only\n3. Instructors only\n");
    printf("Choice: "); role = readNumber();
    if (role < 1 || role > 3) {
        printf("Invalid role choice.\n");
        return;
    }
    printf("Exact name to search (0 for all, use _ for spaces): ");
    scanf("%49s", keyword);
    fp = fopen("administrators.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %49s %29s %29s %19s %d",
                      a.id, a.name, a.username, a.password,
                      a.contact, &a.active) == 6) {
            admins++;
            if (role != 3 &&
                (strcmp(keyword, "0") == 0 || strcmp(keyword, a.name) == 0)) {
                printf("Administrator | %s | %s\n", a.id, a.name);
            }
        }
        fclose(fp);
    }
    fp = fopen("instructors.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %49s %19s %d",
                      id, name, contact, &status) == 4) {
            instructors++;
            if (role != 2 &&
                (strcmp(keyword, "0") == 0 || strcmp(keyword, name) == 0)) {
                printf("Instructor    | %s | %s\n", id, name);
            }
        }
        fclose(fp);
    }
    printf("Total administrators: %d\n", admins);
    printf("Total instructors: %d\n", instructors);
    printf("Total staff: %d\n", admins + instructors);
}

void revenueReport(void)
{
    FILE *fp;
    char id[20], student[20], relatedID[20], status[20];
    int choice, year, month = 0, recordYear, recordMonth;
    int registrations = 0;
    double amount, revenue = 0.0;

    printf("\n--- Revenue Report ---\n");
    printf("1. Monthly\n2. Yearly\nChoice: ");
    choice = readNumber();
    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n"); return;
    }
    printf("Enter year: "); year = readNumber();
    if (year < 2000 || year > 2100) {
        printf("Invalid year.\n"); return;
    }
    if (choice == 1) {
        printf("Enter month (1-12): "); month = readNumber();
        if (month < 1 || month > 12) {
            printf("Invalid month.\n"); return;
        }
    }
    fp = fopen("bookings.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %19s %19s %d %d %19s",
                      id, student, relatedID, &recordYear, &recordMonth, status) == 6) {
            if (recordYear == year && (choice == 2 || recordMonth == month) &&
                strcmp(status, "BOOKED") == 0) {
                registrations++;
            }
        }
        fclose(fp);
    }
    fp = fopen("payments.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%19s %19s %19s %d %d %lf %19s",
                      id, student, relatedID, &recordYear, &recordMonth,
                      &amount, status) == 7) {
            if (recordYear == year && (choice == 2 || recordMonth == month) &&
                strcmp(status, "PAID") == 0) {
                revenue += amount;
            }
        }
        fclose(fp);
    }
    printf("Registrations: %d\n", registrations);
    printf("Total revenue (RM): %.2f\n", revenue);
}

void managerMenu(void)
{
    int choice;
    do {
        printf("\n======= MANAGER MENU =======\n");
        printf("1. Manage Administrators\n");
        printf("2. Staff Report\n");
        printf("3. Revenue Report\n");
        printf("4. Logout\n");
        printf("Choice: ");
        choice = readNumber();
        switch (choice) {
            case 1: manageAdministrators(); break;
            case 2: staffReport(); break;
            case 3: revenueReport(); break;
            case 4: printf("Logged out.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}