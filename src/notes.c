#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notes.h"

static void flush_line(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

/* ---------- Date validation (Gregorian) ---------- */

static int is_leap(int yy) {
    return (yy % 4 == 0 && yy % 100 != 0) || (yy % 400 == 0);
}

int days_in_month(int mm, int yy) {
    static const int dm[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (mm < 1 || mm > 12) return 0;
    if (mm == 2) return dm[2] + (is_leap(yy) ? 1 : 0);
    return dm[mm];
}

int is_valid_date(int dd, int mm, int yy) {
    if (yy < 1) return 0;                         /* reject year 0/negatives */
    int dim = days_in_month(mm, yy);
    if (dim == 0) return 0;                       /* invalid month */
    if (dd < 1 || dd > dim) return 0;             /* invalid day for that month/year */
    return 1;
}

/* ---------- Existing features ---------- */

char checkNote(int dd, int mm, int yy) {
    struct Remainder r;
    FILE *fp = fopen("note.dat", "rb");
    if (!fp) return ' ';

    while (fread(&r, sizeof r, 1, fp) == 1) {
        if (r.dd == dd && r.mm == mm && r.yy == yy) {
            fclose(fp);
            return '*';
        }
    }
    fclose(fp);
    return ' ';
}

void AddNote(void) {
    struct Remainder r;
    FILE *fp = fopen("note.dat", "ab");
    if (!fp) {
        puts("Failed to open notes file.");
        return;
    }

    printf("Enter the date (DD MM YYYY): ");
    if (scanf("%d %d %d", &r.dd, &r.mm, &r.yy) != 3) {
        puts("Invalid date input.");
        fclose(fp);
        flush_line();
        return;
    }
    flush_line();

    if (r.mm < 1 || r.mm > 12 || r.dd < 1 || r.dd > days_in_month(r.mm, r.yy)) {
        puts("Invalid day or month for the given year.");
        fclose(fp);
        return;
    }

    /* ---- NEW: validate the date before proceeding ---- */
    if (!is_valid_date(r.dd, r.mm, r.yy)) {
        puts("Invalid day for the given month and year.");
        fclose(fp);
        return;
    }

    printf("Enter the Note (max 49 chars): ");
    if (scanf(" %49[^\n]", r.note) != 1) {
        puts("Failed to read note.");
        fclose(fp);
        flush_line();
        return;
    }
    flush_line();

    if (fwrite(&r, sizeof r, 1, fp) == 1) {
        puts("Note saved successfully.");
    } else {
        puts("Failed to save note.");
    }
    fclose(fp);
}

void showNote(int mm, int yy) {
    struct Remainder r;
    FILE *fp = fopen("note.dat", "rb");
    if (!fp) {
        puts("Error opening notes file.");
        return;
    }

    int i = 0, found = 0;
    while (fread(&r, sizeof r, 1, fp) == 1) {
        if (r.mm == mm && r.yy == yy) {
            printf("\nNote %d Day = %d: %s\n", ++i, r.dd, r.note);
            found = 1;
        }
    }
    if (!found) puts("\nThis month contains no note.");
    fclose(fp);
}

void DeleteNote(void) {
    FILE *fp = fopen("note.dat", "rb");
    if (!fp) {
        puts("No notes found.");
        return;
    }

    FILE *ft = fopen("temp.dat", "wb");
    if (!ft) {
        puts("Unable to create temporary file.");
        fclose(fp);
        return;
    }

    int d, m, y;
    int found = 0;
    struct Remainder r;

    printf("Enter date of note to delete (DD MM YYYY): ");
    if (scanf("%d %d %d", &d, &m, &y) != 3) {
        puts("Invalid input.");
        fclose(fp);
        fclose(ft);
        remove("temp.dat");
        flush_line();
        return;
    }
    flush_line();

    /* ---- NEW: validate date before searching ---- */
    if (!is_valid_date(d, m, y)) {
        puts("Invalid day for the given month and year.");
        fclose(fp);
        fclose(ft);
        remove("temp.dat");
        return;
    }

    while (fread(&r, sizeof r, 1, fp) == 1) {
while (fread(&r, sizeof r, 1, fp) == 1) {
    if (r.dd == d && r.mm == m && r.yy == y) {
        found = 1;      /* delete this record */
        continue;       /* skip write -> effectively removes it */
    }
    fwrite(&r, sizeof r, 1, ft);
}
        }
        fwrite(&r, sizeof r, 1, ft);
    }

    fclose(fp);
    fclose(ft);

    if (!found) {
        printf("No note found for %02d/%02d/%d.\n", d, m, y);
        remove("temp.dat");
        return;
    }

    if (remove("note.dat") != 0 || rename("temp.dat", "note.dat") != 0) {
        puts("Failed to update notes file.");
        return;
    }

    printf("All notes for %02d/%02d/%d deleted successfully.\n", d, m, y);
}
