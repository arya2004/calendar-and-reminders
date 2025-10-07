#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../notes.h"
#define TEST_FILE "test_note.dat"

static void write_note(int dd, int mm, int yy, const char *text) {
    struct Remainder r = {dd, mm, yy, ""};
    strncpy(r.note, text, sizeof(r.note) - 1);

    FILE *fp = fopen(TEST_FILE, "ab");
    assert(fp != NULL);
    fwrite(&r, sizeof r, 1, fp);
    fclose(fp);
}

static void clean_test_file(void) {
    remove(TEST_FILE);
}
static char test_checkNote(int dd, int mm, int yy) {
    struct Remainder r;
    FILE *fp = fopen(TEST_FILE, "rb");
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
static void test_DeleteNote(int dd, int mm, int yy) {
    FILE *fp = fopen(TEST_FILE, "rb");
    assert(fp != NULL);

    FILE *ft = fopen("temp_test.dat", "wb");
    assert(ft != NULL);

    struct Remainder r;
    while (fread(&r, sizeof r, 1, fp) == 1) {
        if (!(r.dd == dd && r.mm == mm && r.yy == yy)) {
            fwrite(&r, sizeof r, 1, ft);
        }
    }
    fclose(fp);
    fclose(ft);

    remove(TEST_FILE);
    rename("temp_test.dat", TEST_FILE);
}
static void test_showNote(int mm, int yy) {
    struct Remainder r;
    FILE *fp = fopen(TEST_FILE, "rb");
    if (!fp) {
        printf("Error opening test notes file.\n");
        return;
    }

    int found = 0;
    while (fread(&r, sizeof r, 1, fp) == 1) {
        if (r.mm == mm && r.yy == yy) {
            printf("Found Note: %02d/%02d/%d -> %s\n", r.dd, r.mm, r.yy, r.note);
            found = 1;
        }
    }
    if (!found) printf("No notes found for %02d/%d.\n", mm, yy);
    fclose(fp);
}

int main(void) {
    printf("Running tests for note operations...\n");

    clean_test_file();

    // --- Test 1: Add / Write Note ---
    write_note(6, 10, 2025, "Test note for October 2025");
    FILE *fp = fopen(TEST_FILE, "rb");
    assert(fp != NULL);

    struct Remainder r;
    fread(&r, sizeof r, 1, fp);
    fclose(fp);
    assert(r.dd == 6 && r.mm == 10 && r.yy == 2025);
    assert(strcmp(r.note, "Test note for October 2025") == 0);
    printf("Test 1 Passed: Note written correctly.\n");

    // --- Test 2: checkNote() ---
    assert(test_checkNote(6, 10, 2025) == '*');
    assert(test_checkNote(1, 1, 2024) == ' ');
    printf("Test 2 Passed: checkNote works correctly.\n");

    // --- Test 3: showNote() ---
    printf("🧪 Display test output for showNote:\n");
    test_showNote(10, 2025); // should print note
    printf("Test 3 Passed: showNote displayed expected note.\n");

    // --- Test 4: DeleteNote() ---
    test_DeleteNote(6, 10, 2025);
    assert(test_checkNote(6, 10, 2025) == ' ');
    printf("Test 4 Passed: DeleteNote removed note correctly.\n");

    clean_test_file();
    printf("\n🎉 All note operation tests passed successfully!\n");

    return 0;
}
