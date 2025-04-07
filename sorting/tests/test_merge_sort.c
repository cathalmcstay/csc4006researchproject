#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Declare the function
void merge_sort(int *arr, int l, int r, int verbose);

START_TEST(test_merge_sort_basic) {
    int arr[] = {5, 2, 4, 6, 1, 3};
    int expected[] = {1, 2, 3, 4, 5, 6};
    int size = sizeof(arr) / sizeof(int);

    merge_sort(arr, 0, size - 1, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

START_TEST(test_merge_sort_already_sorted) {
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3, 4, 5};
    int size = sizeof(arr) / sizeof(int);

    merge_sort(arr, 0, size - 1, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

START_TEST(test_merge_sort_reverse_sorted) {
    int arr[] = {9, 7, 5, 3, 1};
    int expected[] = {1, 3, 5, 7, 9};
    int size = sizeof(arr) / sizeof(int);

    merge_sort(arr, 0, size - 1, 0);

    for (int i = 0; i < size; i++) {
        ck_assert_int_eq(arr[i], expected[i]);
    }
}
END_TEST

Suite *merge_suite(void) {
    Suite *s = suite_create("Merge Sort");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_merge_sort_basic);
    tcase_add_test(tc_core, test_merge_sort_already_sorted);
    tcase_add_test(tc_core, test_merge_sort_reverse_sorted);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = merge_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
