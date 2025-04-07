#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void bubble_sort(int *arr, int size, int verbose) {
    int step = 1;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                if (verbose) {
                    printf("Step %d: ", step++);
                    print_array(arr, size);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
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

    double start_time = MPI_Wtime();
    bubble_sort(arr, size, verbose);
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
