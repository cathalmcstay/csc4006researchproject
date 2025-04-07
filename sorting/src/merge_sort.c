#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
}

void merge(int *arr, int l, int m, int r, int verbose) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    if (verbose) {
        printf("Merged: ");
        print_array(arr + l, r - l + 1);
    }
}

void merge_sort(int *arr, int l, int r, int verbose) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m, verbose);
        merge_sort(arr, m + 1, r, verbose);
        merge(arr, l, m, r, verbose);
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
    merge_sort(arr, 0, size - 1, verbose);
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
