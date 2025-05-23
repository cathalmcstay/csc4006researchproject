#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void print_array(int *arr, int size) {
    // Print the array elements
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
}

int partition(int *arr, int low, int high, int verbose) {
    // Partition the array and return the pivot index
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = tmp;

    if (verbose) {
        printf("Pivot %d placed at index %d: ", pivot, i + 1);
        print_array(arr + low, high - low + 1);
    }

    return i + 1;
}

void quick_sort(int *arr, int low, int high, int verbose) {
    // Sort the array using the quicksort algorithm
    if (low < high) {
        int pi = partition(arr, low, high, verbose);
        quick_sort(arr, low, pi - 1, verbose);
        quick_sort(arr, pi + 1, high, verbose);
    }
}

int main(int argc, char **argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 2) {
        if (rank == 0) fprintf(stderr, "Usage: %s <size> [--verbose]\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int size = atoi(argv[1]);
    int verbose = (argc == 3 && strcmp(argv[2], "--verbose") == 0);

    int *arr = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) scanf("%d", &arr[i]);

    if (verbose) {
        printf("Initial array:\n");
        print_array(arr, size);
    }

    // Measure execution time
    double start_time = MPI_Wtime();
    quick_sort(arr, 0, size - 1, verbose);
    double end_time = MPI_Wtime();

    if (!verbose) {
        printf("Sorted array:\n");
        print_array(arr, size);
    }

    printf("Execution time: %f seconds\n", end_time - start_time);
    free(arr);
    MPI_Finalize();
    return 0;
}
