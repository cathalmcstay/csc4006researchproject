#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Declare the function under test
void bubble_sort(int *arr, int size, int verbose);

START_TEST(test_bubble_basic) {
    int input[] = {5, 2, 9, 1, 5, 6};
    int expected[] = {1, 2, 5, 5, 6, 9};
    int size = sizeof(input) / sizeof(int);

    bubble_sort(input, size, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_msg(input[i] == expected[i],
                      "Mismatch at index %d: got %d, expected %d", i, input[i], expected[i]);
    }
}
END_TEST

START_TEST(test_bubble_already_sorted) {
    int input[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3, 4, 5};
    int size = sizeof(input) / sizeof(int);

    bubble_sort(input, size, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(input[i], expected[i]);
    }
}
END_TEST

START_TEST(test_bubble_reverse_sorted) {
    int input[] = {9, 8, 7, 6, 5};
    int expected[] = {5, 6, 7, 8, 9};
    int size = sizeof(input) / sizeof(int);

    bubble_sort(input, size, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(input[i], expected[i]);
    }
}
END_TEST

Suite *bubble_suite(void) {
    Suite *s = suite_create("Bubble Sort");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_bubble_basic);
    tcase_add_test(tc_core, test_bubble_already_sorted);
    tcase_add_test(tc_core, test_bubble_reverse_sorted);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = bubble_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
