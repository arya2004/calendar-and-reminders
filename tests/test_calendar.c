#include <check.h>

/* If the project already has include/calendar.h, prefer it: */
#ifdef __has_include
#  if __has_include("include/calendar.h")
#    include "include/calendar.h"
#  endif
#endif

/* Fallback prototypes (won't hurt if header also declares them) */
int is_leap_year(int year);
int days_in_month(int month, int year);

/* ---- is_leap_year tests ---- */
START_TEST(test_leap_year_true_examples) {
    ck_assert_int_eq(is_leap_year(2000), 1);  /* divisible by 400 */
    ck_assert_int_eq(is_leap_year(2024), 1);  /* divisible by 4, not 100 */
}
END_TEST

START_TEST(test_leap_year_false_examples) {
    ck_assert_int_eq(is_leap_year(1900), 0);  /* divisible by 100 but not 400 */
    ck_assert_int_eq(is_leap_year(2023), 0);  /* not divisible by 4 */
}
END_TEST

/* ---- days_in_month tests ---- */
START_TEST(test_days_in_month_normal) {
    ck_assert_int_eq(days_in_month(1, 2023), 31);  /* Jan */
    ck_assert_int_eq(days_in_month(4, 2023), 30);  /* Apr */
    ck_assert_int_eq(days_in_month(2, 2023), 28);  /* Feb non-leap */
    ck_assert_int_eq(days_in_month(2, 2024), 29);  /* Feb leap */
}
END_TEST

START_TEST(test_days_in_month_invalid) {
    /* Many projects return 0 for invalid months; adjust if repo differs */
    ck_assert_int_eq(days_in_month(0, 2023), 0);
    ck_assert_int_eq(days_in_month(13, 2023), 0);
}
END_TEST

Suite* calendar_suite(void) {
    Suite *s = suite_create("calendar");

    TCase *tc_leap = tcase_create("is_leap_year");
    tcase_add_test(tc_leap, test_leap_year_true_examples);
    tcase_add_test(tc_leap, test_leap_year_false_examples);
    suite_add_tcase(s, tc_leap);

    TCase *tc_dim = tcase_create("days_in_month");
    tcase_add_test(tc_dim, test_days_in_month_normal);
    tcase_add_test(tc_dim, test_days_in_month_invalid);
    suite_add_tcase(s, tc_dim);

    return s;
}
