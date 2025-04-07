#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <gmp.h>

void fibonacci_recursive(unsigned int n, mpz_t result) {
    mpz_t a, b;
    mpz_init(a);
    mpz_init(b);

    if (n == 0) {
        mpz_set_ui(result, 0);
    } else if (n == 1) {
        mpz_set_ui(result, 1);
    } else {
        mpz_t temp1, temp2;
        mpz_init(temp1);
        mpz_init(temp2);

        fibonacci_recursive(n - 1, temp1);
        fibonacci_recursive(n - 2, temp2);
        mpz_add(result, temp1, temp2);

        mpz_clear(temp1);
        mpz_clear(temp2);
    }

    mpz_clear(a);
    mpz_clear(b);
}


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    unsigned int n = atoi(argv[1]);

    double start_time = MPI_Wtime();

    if (rank == 0) {
        mpz_t result;
        mpz_init(result);

        fibonacci_recursive(n, result);

        double end_time = MPI_Wtime();
        double exec_time = end_time - start_time;

        gmp_printf("Fibonacci(%u) = %Zd\n", n, result);
        printf("Execution time: %f seconds\n", exec_time);

        mpz_clear(result);
    }

    MPI_Finalize();
    return 0;
}
