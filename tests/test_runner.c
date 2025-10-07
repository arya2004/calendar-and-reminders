#include <check.h>
#include <stdlib.h>

/* Declared in test_calendar.c */
Suite* calendar_suite(void);  
/* Declared in test_smoke.c */
Suite* smoke_suite(void);

int main(void) {
    int number_failed;
    SRunner *sr = srunner_create(NULL);
    srunner_add_suite(sr, smoke_suite());     
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
