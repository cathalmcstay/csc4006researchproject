#include <check.h>
#include <stdlib.h>
#include <stdio.h>

// Declare the quick sort function
void quick_sort(int *arr, int low, int high, int verbose);

START_TEST(test_quick_basic) {
    int arr[] = {10, 7, 8, 9, 1, 5};
    int expected[] = {1, 5, 7, 8, 9, 10};
    int size = sizeof(arr) / sizeof(int);

    quick_sort(arr, 0, size - 1, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

START_TEST(test_quick_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(int);

    quick_sort(arr, 0, size - 1, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

START_TEST(test_quick_reverse_sorted) {
    int arr[] = {9, 8, 7, 6, 5};
    int expected[] = {5, 6, 7, 8, 9};
    int size = sizeof(arr) / sizeof(int);

    quick_sort(arr, 0, size - 1, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

Suite *quick_suite(void) {
    Suite *s = suite_create("Quick Sort");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_quick_basic);
    tcase_add_test(tc_core, test_quick_already_sorted);
    tcase_add_test(tc_core, test_quick_reverse_sorted);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = quick_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
