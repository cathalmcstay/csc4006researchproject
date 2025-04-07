#include <check.h>
#include <gmp.h>
#include <stdlib.h>

// Declare the function to test
void fibonacci_recursive(unsigned int n, mpz_t result);

// Helper function to convert result to string and check equality
void assert_fibonacci(unsigned int n, const char *expected_str) {
    mpz_t result;
    mpz_init(result);

    fibonacci_recursive(n, result);

    char *result_str = mpz_get_str(NULL, 10, result);
    ck_assert_str_eq(result_str, expected_str);

    free(result_str);
    mpz_clear(result);
}

START_TEST(test_fib_0) {
    assert_fibonacci(0, "0");
}
END_TEST

START_TEST(test_fib_1) {
    assert_fibonacci(1, "1");
}
END_TEST

START_TEST(test_fib_2) {
    assert_fibonacci(2, "1");
}
END_TEST

START_TEST(test_fib_10) {
    assert_fibonacci(10, "55");
}
END_TEST

START_TEST(test_fib_20) {
    assert_fibonacci(20, "6765");
}
END_TEST

Suite* fib_suite(void) {
    Suite *s = suite_create("Fibonacci Recursive");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_fib_0);
    tcase_add_test(tc_core, test_fib_1);
    tcase_add_test(tc_core, test_fib_2);
    tcase_add_test(tc_core, test_fib_10);
    tcase_add_test(tc_core, test_fib_20);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = fib_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
