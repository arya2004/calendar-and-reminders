#include <stdio.h>
#include "calendar.h"
#include "notes.h"

int main() {
    int choice;
    int year, month, mm, yy;

    while (1) {
        printf("\n0. EXIT\n1. Print the calendar\n2. Add Reminder\n3. Show Reminder\n4. Delete Reminder\n");
        printf("ENTER YOUR CHOICE: ");

        
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter month (MM): ");
                scanf("%d", &month);
                printf("Enter year (YYYY): ");
                scanf("%d", &year);
                calendar_printer(year, month);
                break;
            case 2:
                AddNote();
                break;
            case 3:
                printf("Enter month (MM): ");
                scanf("%d", &mm);
                printf("Enter year (YYYY): ");
                scanf("%d", &yy);
                showNote(mm, yy);
                break;
            case 4:
                DeleteNote();
                break;
            case 0:
                return 0;
            default:
                printf("\nInvalid choice. Please try again.\n");
                continue;
        }
    }
    return 0;
}
