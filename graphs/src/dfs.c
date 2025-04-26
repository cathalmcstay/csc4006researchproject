#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <stdbool.h>

void dfs(int **adj, bool *visited, int n, int node) {
    // Mark the current node as visited and print it
    visited[node] = true;
    printf("%d ", node);

    // Recur for all the vertices adjacent to this vertex
    for (int i = 0; i < n; i++) {
        if (adj[node][i] && !visited[i]) {
            dfs(adj, visited, n, i);
        }
    }
}

int main(int argc, char **argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    scanf("%d", &n);
    // Allocate memory for the adjacency matrix
    int **adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        adj[i] = malloc(n * sizeof(int)); 
        for (int j = 0; j < n; j++) {
            scanf("%d", &adj[i][j]);
        }
    }

    // Allocate memory for the visited arra
    bool *visited = calloc(n, sizeof(bool));

    // Perform DFS traversal starting from the first node (0)
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
