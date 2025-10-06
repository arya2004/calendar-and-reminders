#include <stdio.h>
#include "calendar.h"
#include "notes.h"

/* robust integer input */
static int read_int(const char *prompt) {
    int x, ch;
    for (;;) {
        if (prompt && *prompt) printf("%s", prompt);
        if (scanf("%d", &x) == 1) { while ((ch=getchar())!='\n' && ch!=EOF){} return x; }
        printf("Invalid input! Try again.\n");
        while ((ch=getchar())!='\n' && ch!=EOF) {}
    }
}

int main(void) {
    for (;;) {
        printf("\n=== Calendar & Reminders ===\n");
        printf("1. Show calendar\n");
        printf("2. Add note\n");
        printf("3. View reminders for a month\n");   /* <- new option per issue #8 */
        printf("4. Delete a note by date\n");
        printf("5. Exit\n");

        int choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1: {
                int year  = read_int("Enter year  (e.g., 2025): ");
                int month = read_int("Enter month (1-12): ");
                calendar_printer(year, month);
                break;
            }
            case 2:
                AddNote();
                break;
            case 3: {                                   /* <- calls existing filter */
                int mm = read_int("Enter month (1-12): ");
                int yy = read_int("Enter year  (e.g., 2025): ");
                showNote(mm, yy);
                break;
            }
            case 4:
                DeleteNote();
                break;
            case 5:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please select 1-5.\n");
        }
    }
}
