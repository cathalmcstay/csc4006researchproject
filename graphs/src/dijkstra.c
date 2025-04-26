#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

#define INF 1000000000  // A large number representing infinity

int min_distance(int dist[], int visited[], int n) {
    // Find the vertex with the minimum distance value from the set of vertices not yet included in the shortest path tree
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int **graph, int n, int start) {
    // Dijkstra's algorithm to find the shortest path from start node to all other nodes
    int *dist = malloc(n * sizeof(int));
    int *visited = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) dist[i] = INF;
    dist[start] = 0;

    // Main loop of Dijkstra's algorithm
    for (int count = 0; count < n - 1; count++) {
        int u = min_distance(dist, visited, n);
        if (u == -1) break;

        visited[u] = 1;
        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("Shortest distances from node %d:\n", start);
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF)
            printf("Node %d: INF\n", i);
        else
            printf("Node %d: %d\n", i, dist[i]);
    }

    free(dist);
    free(visited);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n;
    scanf("%d", &n);

    // Read the graph from standard input
    int **graph = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }
    double start_time = MPI_Wtime();
    if (rank == 0) dijkstra(graph, n, 0);
    double end_time = MPI_Wtime();

    if (rank == 0)
        printf("Execution time: %f seconds\n", end_time - start_time);

    for (int i = 0; i < n; i++) free(graph[i]);
    free(graph);

    MPI_Finalize();
    return 0;
}

