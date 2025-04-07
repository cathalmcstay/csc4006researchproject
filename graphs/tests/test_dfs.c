#include <check.h>
#include <stdlib.h>
#include <stdbool.h>

// Declare the function to test
void dfs(int **adj, bool *visited, int n, int node);

START_TEST(test_dfs_graph_traversal) {
    int n = 5;

    int **adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        adj[i] = calloc(n, sizeof(int));
    }

    // Create a graph: 0-1-2, 1-3, 3-4
    adj[0][1] = adj[1][0] = 1;
    adj[1][2] = adj[2][1] = 1;
    adj[1][3] = adj[3][1] = 1;
    adj[3][4] = adj[4][3] = 1;

    bool *visited = calloc(n, sizeof(bool));

    dfs(adj, visited, n, 0);

    for (int i = 0; i < n; i++) {
        ck_assert_msg(visited[i], "Node %d was not visited", i);
    }

    for (int i = 0; i < n; i++) free(adj[i]);
    free(adj);
    free(visited);
}
END_TEST

Suite *dfs_suite(void) {
    Suite *s = suite_create("DFS");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_dfs_graph_traversal);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = dfs_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
