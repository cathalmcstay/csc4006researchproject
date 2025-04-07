#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <stdbool.h>

void dfs(int **adj, bool *visited, int n, int node) {
    visited[node] = true;
    printf("%d ", node);

    for (int i = 0; i < n; i++) {
        if (adj[node][i] && !visited[i]) {
            dfs(adj, visited, n, i);
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    scanf("%d", &n);

    int **adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        adj[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &adj[i][j]);
        }
    }

    bool *visited = calloc(n, sizeof(bool));

    double start_time = MPI_Wtime();
    if (rank == 0) {
        printf("DFS traversal: ");
        dfs(adj, visited, n, 0);
        printf("\n");
    }
    double end_time = MPI_Wtime();

    if (rank == 0)
        printf("Execution time: %f seconds\n", end_time - start_time);

    free(visited);
    for (int i = 0; i < n; i++) free(adj[i]);
    free(adj);

    MPI_Finalize();
    return 0;
}
