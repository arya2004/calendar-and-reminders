#include <stdio.h>
#include <string.h>
#include <time.h>
#include "calendar.h"
#include "notes.h"

/* ---- Constants ---- */
#define CALENDAR_SIZE 42   /* 6 weeks × 7 days */
#define DAYS_IN_WEEK  7

/* ---- Internal helper ---- */
/* Day-of-week (0..6 = Sun..Sat) for the first day of a month.
   Sakamoto’s algorithm, adapted to day=1. */
static int first_day_of_month(int year, int month) {
    static const int t[] = {0,3,2,5,0,3,5,1,4,6,2,4};
    int y = year, m = month;
    if (m < 3) y -= 1;
    return (y + y/4 - y/100 + y/400 + t[m-1] + 1) % 7;
}

/* ---- Public API ---- */
void calendar_printer(int year, int month) {
    int grid[CALENDAR_SIZE];
    memset(grid, 0, sizeof(grid));

    const int start   = first_day_of_month(year, month);
    const int n_days  = days_in_month(month, year);
    for (int d = 1; d <= n_days; ++d) grid[start + d - 1] = d;

    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    const int cur_d = lt ? lt->tm_mday      : -1;
    const int cur_m = lt ? (lt->tm_mon + 1) : -1;
    const int cur_y = lt ? (lt->tm_year + 1900) : -1;

    printf(" Sun\tMon\tTue\tWed\tThu\tFri\tSat\n");
    for (int i = 0; i < CALENDAR_SIZE; ++i) {
        if (grid[i] == 0) {
            printf("\t");
        } else {
            const int d = grid[i];
            const int is_today = (d == cur_d && month == cur_m && year == cur_y);
            char note = checkNote(d, month, year);   /* '*' if note exists, ' ' otherwise */

            if (is_today) printf("[%2d%c]\t", d, note);   /* e.g. [25*] */
            else          printf("%3d%c\t",  d, note);    /* e.g.  25*  */
        }
        if ((i + 1) % DAYS_IN_WEEK == 0) printf("\n");
    }
}

int is_leap_year(int year) {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

int days_in_month(int month, int year) {
    static const int base[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && is_leap_year(year)) return 29;
    return base[month - 1];
}
