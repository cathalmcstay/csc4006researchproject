#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Declare the function
void dijkstra(int **graph, int n, int start);

// Capture output from dijkstra() into a buffer
char *capture_dijkstra_output(int **graph, int n, int start) {
    FILE *fp = freopen("dijkstra_output.txt", "w+", stdout);
    dijkstra(graph, n, start);
    fflush(stdout);

    fseek(fp, 0, SEEK_SET);
    char *output = calloc(1024, sizeof(char));
    fread(output, sizeof(char), 1023, fp);
    fclose(fp);
    freopen("/dev/tty", "w", stdout);  // Reset stdout

    return output;
}

START_TEST(test_dijkstra_basic_graph) {
    int n = 5;
    int **graph = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = calloc(n, sizeof(int));
    }

    // Create weighted graph
    // 0--1--2
    //  \    |
    //   \   |
    //    \--3--4
    graph[0][1] = 2; graph[1][0] = 2;
    graph[1][2] = 3; graph[2][1] = 3;
    graph[0][3] = 6; graph[3][0] = 6;
    graph[3][2] = 1; graph[2][3] = 1;
    graph[3][4] = 5; graph[4][3] = 5;

    char *output = capture_dijkstra_output(graph, n, 0);

    // Check if expected shortest distances are in output
    ck_assert_msg(strstr(output, "Node 0: 0") != NULL, "Missing distance for node 0");
    ck_assert_msg(strstr(output, "Node 1: 2") != NULL, "Missing distance for node 1");
    ck_assert_msg(strstr(output, "Node 2: 5") != NULL, "Missing distance for node 2");
    ck_assert_msg(strstr(output, "Node 3: 6") != NULL, "Missing distance for node 3");
    ck_assert_msg(strstr(output, "Node 4: 11") != NULL, "Missing distance for node 4");

    for (int i = 0; i < n; i++) free(graph[i]);
    free(graph);
    free(output);
}
END_TEST

Suite *dijkstra_suite(void) {
    Suite *s = suite_create("Dijkstra");
    TCase *tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_dijkstra_basic_graph);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = dijkstra_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
