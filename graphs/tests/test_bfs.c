#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Declare the function under test
void bfs(int **adj, int n, int start);

// Capture output
char *capture_bfs_output(int **adj, int n, int start) {
    FILE *fp = freopen("bfs_output.txt", "w+", stdout);
    bfs(adj, n, start);
    fflush(stdout);

    fseek(fp, 0, SEEK_SET);
    char *output = calloc(1024, sizeof(char));
    fread(output, sizeof(char), 1023, fp);
    fclose(fp);
    freopen("/dev/tty", "w", stdout);  // Restore stdout

    return output;
}

START_TEST(test_bfs_order) {
    int n = 5;
    int **adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) adj[i] = calloc(n, sizeof(int));

    // Build a simple graph:
    // 0 - 1 - 2
    //     |
    //     3 - 4
    adj[0][1] = adj[1][0] = 1;
    adj[1][2] = adj[2][1] = 1;
    adj[1][3] = adj[3][1] = 1;
    adj[3][4] = adj[4][3] = 1;

    char *output = capture_bfs_output(adj, n, 0);

    ck_assert_msg(strstr(output, "BFS traversal: 0 1 2 3 4") != NULL,
                  "Unexpected traversal output: %s", output);

    for (int i = 0; i < n; i++) free(adj[i]);
    free(adj);
    free(output);
}
END_TEST

Suite *bfs_suite(void) {
    Suite *s = suite_create("BFS");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_bfs_order);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = bfs_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
