#ifndef NOTES_H
#define NOTES_H

struct Remainder {
    int dd, mm, yy;
    char note[50];
};

char checkNote(int dd, int mm, int yy);
void AddNote(void);
void showNote(int mm, int yy);
void DeleteNote(void);

/* Date helpers */
int days_in_month(int mm, int yy);
int is_valid_date(int dd, int mm, int yy);

#endif
