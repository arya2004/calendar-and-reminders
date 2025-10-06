#include <check.h>
START_TEST(test_truth) { ck_assert_int_eq(1,1); } END_TEST

Suite* smoke_suite(void){
    Suite *s = suite_create("smoke");
    TCase *tc = tcase_create("truth");
    tcase_add_test(tc, test_truth);
    suite_add_tcase(s, tc);
    return s;
}
