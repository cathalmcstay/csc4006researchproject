#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long monte_carlo_pi(long long num_samples) {
    long long count = 0;
    for (long long i = 0; i < num_samples; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1) {
            count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure argument is provided
    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <total_samples>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    // Parse the number of samples from command-line argument
    long long total_samples = atoll(argv[1]);
    long long local_samples = total_samples / size;

    srand(time(NULL) + rank);  // Ensure different seed per process

    // Start timing
    double start_time = MPI_Wtime();
    long long local_count = monte_carlo_pi(local_samples);
    double end_time = MPI_Wtime();

    // Reduce results
    long long global_count;
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Collect and print execution times
    double local_time = end_time - start_time;
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = (4.0 * global_count) / total_samples;
        printf("Estimated Pi: %f\n", pi_estimate);
        printf("Max execution time: %f seconds\n", max_time);
    }

    printf("Process %d executed in %f seconds\n", rank, local_time);

    MPI_Finalize();
    return 0;
}
