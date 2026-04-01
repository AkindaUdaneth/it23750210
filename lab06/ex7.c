#include <stdio.h>
#include <mpi.h>

#define TOTAL_NUMBERS 10000000

int main(int argc, char** argv) {
    int rank, size;
    long long local_sum = 0;
    long long global_sum = 0;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 1. Divide the work (Same as Ex3)
    long long chunk_size = TOTAL_NUMBERS / size;
    long long start = rank * chunk_size + 1;
    long long end = (rank == size - 1) ? TOTAL_NUMBERS : start + chunk_size - 1;

    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // 2. Calculate the local sum
    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    // 3. Manual Communication using MPI_Send and MPI_Recv
    if (rank != 0) {
        // All worker nodes send their completed local_sum to the Master (Rank 0)
        MPI_Send(&local_sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        // Master node handles its own chunk first
        global_sum = local_sum; 
        
        long long received_sum;
        MPI_Status status;

        // Master loops to receive messages from the exact number of workers (size - 1)
        for (int i = 1; i < size; i++) {
            // THE CRITICAL CHANGE: MPI_ANY_SOURCE
            // Master grabs the first message that arrives over the network, regardless of which rank sent it!
            MPI_Recv(&received_sum, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            
            global_sum += received_sum;
            // Print who sent it to prove ANY_SOURCE is working dynamically
            printf("Master received a partial sum from Rank %d\n", status.MPI_SOURCE);
        }

        end_time = MPI_Wtime();
        printf("Total Sum from 1 to %d: %lld\n", TOTAL_NUMBERS, global_sum);
        printf("Time taken with %d processors: %f seconds\n", size, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
