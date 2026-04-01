#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TOTAL_POINTS 10000000

int main(int argc, char** argv) {
    int rank, size;
    long long local_count = 0, global_count = 0;
    double x, y;
    double start_time, end_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 1. Divide the 10,000,000 darts evenly among the processors
    long long points_per_process = TOTAL_POINTS / size;

    // 2. Seed the random number generator uniquely for each process
    // If we don't do this, every process will throw darts at the exact same spots!
    srand(time(NULL) + rank);

    // Start timer on Master
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // 3. Monte Carlo simulation: Throw the darts
    for (long long i = 0; i < points_per_process; i++) {
        // Generate random x and y between 0.0 and 1.0
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        // Check if the dart landed inside the circle radius
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    // 4. Reduce: Master collects all the "inside circle" counts from the workers
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // 5. Calculate Pi and print results on Master
    if (rank == 0) {
        end_time = MPI_Wtime();
        // The formula for Monte Carlo Pi estimation: Pi = 4 * (points inside / total points)
        double pi_estimate = 4.0 * ((double)global_count / (double)TOTAL_POINTS);
        
        printf("Estimated Pi = %f\n", pi_estimate);
        printf("Time taken with %d processors: %f seconds\n", size, end_time - start_time);
    }

    // Clean up
    MPI_Finalize();
    return 0;
}
