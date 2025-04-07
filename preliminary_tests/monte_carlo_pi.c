#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to calculate Pi using Monte Carlo method
double monte_carlo_pi(int num_samples) {
    int inside_circle = 0;

    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < num_samples; i++) {
        // Generate random x and y between -1 and 1
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;

        // Check if the point is inside the circle
        if (x * x + y * y <= 1.0) {
            inside_circle++;
        }
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
    clock_t start = clock();

    // Perform Monte Carlo Pi estimation
    double pi_estimate = monte_carlo_pi(num_samples);

    // End timing
    clock_t end = clock();
    double execution_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Print results
    printf("Estimated Pi: %.8f\n", pi_estimate);
    printf("Execution Time: %.2f seconds\n", execution_time);

    return 0;
}
