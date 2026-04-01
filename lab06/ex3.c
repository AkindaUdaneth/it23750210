#include <stdio.h>
#include <mpi.h>

#define TOTAL_NUMBERS 10000000

int main(int argc, char** argv) {
    int rank, size;
    // We use 'long long' because the sum of 1 to 10,000,000 is ~50 trillion,
    // which is way too big for a standard 32-bit 'int' to hold!
    long long local_sum = 0; 
    long long global_sum = 0;
    double start_time, end_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 1. Divide the work
    long long chunk_size = TOTAL_NUMBERS / size;
    long long start = rank * chunk_size + 1;
    // The last process grabs any leftover numbers if it doesn't divide perfectly
    long long end = (rank == size - 1) ? TOTAL_NUMBERS : start + chunk_size - 1;

    // 2. Start the timer (only the Master needs to track the total time)
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // 3. Calculate the local sum for this specific process
    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    // 4. Reduce: Combine all local_sums into the global_sum on the Master (rank 0)
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // 5. Stop the timer and print the results
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Total Sum from 1 to %d: %lld\n", TOTAL_NUMBERS, global_sum);
        printf("Time taken with %d processors: %f seconds\n", size, end_time - start_time);
    }

    // Clean up
    MPI_Finalize();
    return 0;
}
