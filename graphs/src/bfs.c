#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <stdbool.h>

void bfs(int **adj, int n, int start) {
    bool *visited = calloc(n, sizeof(bool));
    int *queue = malloc(n * sizeof(int));
    int front = 0, rear = 0;

    visited[start] = true;
    queue[rear++] = start;

    printf("BFS traversal: ");

    while (front < rear) {
        int node = queue[front++];
        printf("%d ", node);

        for (int i = 0; i < n; i++) {
            if (adj[node][i] && !visited[i]) {
                visited[i] = true;
                queue[rear++] = i;
            }
        }
    }

    printf("\n");

    free(visited);
    free(queue);
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

    double start_time = MPI_Wtime();
    if (rank == 0) bfs(adj, n, 0);
    double end_time = MPI_Wtime();

    if (rank == 0)
        printf("Execution time: %f seconds\n", end_time - start_time);

    for (int i = 0; i < n; i++) free(adj[i]);
    free(adj);

    MPI_Finalize();
    return 0;
}
