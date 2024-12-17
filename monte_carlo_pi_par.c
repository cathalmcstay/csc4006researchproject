#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Function to calculate Pi using Monte Carlo method
double monte_carlo_pi(int num_samples) {
    int inside_circle = 0;

    // Seed the random number generator for each thread
    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        int local_count = 0;

        #pragma omp for
        for (int i = 0; i < num_samples; i++) {
            // Generate random x and y between -1 and 1
            double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;

            // Check if the point is inside the circle
            if (x * x + y * y <= 1.0) {
                local_count++;
            }
        }

        #pragma omp atomic
        inside_circle += local_count;
    }

    // Calculate Pi approximation
    return (4.0 * inside_circle) / num_samples;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_iterations>\n", argv[0]);
        return 1;
    }

    // Parse the number of iterations from the command line
    int num_samples = atoi(argv[1]);
    if (num_samples <= 0) {
        printf("Error: Number of iterations must be a positive integer.\n");
        return 1;
    }

    // Start timing
    double start = omp_get_wtime();

    // Perform Monte Carlo Pi estimation
    double pi_estimate = monte_carlo_pi(num_samples);

    // End timing
    double end = omp_get_wtime();
    double execution_time = end - start;

    // Print results
    printf("Estimated Pi: %.8f\n", pi_estimate);
    printf("Execution Time: %.2f seconds\n", execution_time);

    return 0;
}
