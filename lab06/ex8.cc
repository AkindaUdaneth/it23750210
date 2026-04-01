#include <iostream>
#include <mpi.h>
#include <stdlib.h> // Required for malloc and free

int main(int argc, char** argv) {
    int rank, size;
    int data = 42; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        std::cout << "Please run this with at least 3 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // --- NEW: ALLOCATE AND ATTACH THE BUFFER ---
    // We need enough space for our integer PLUS the hidden MPI overhead data
    int buffer_size = sizeof(int) + MPI_BSEND_OVERHEAD;
    void* buffer = malloc(buffer_size);
    MPI_Buffer_attach(buffer, buffer_size);

    if (rank == 0) {
        std::cout << "Rank 0: Sending data to Rank 1 using BSend..." << std::endl;
        
        // --- NEW: USING BSEND ---
        // Rank 0 instantly dumps the data into the buffer and moves on!
        MPI_Bsend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        std::cout << "Rank 0: Finished sending safely!" << std::endl;
        
    } else if (rank == 1) {
        std::cout << "Rank 1: Waiting to receive data from Rank 2..." << std::endl;
        MPI_Recv(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Rank 1: Received data!" << std::endl;
        
    } else if (rank == 2) {
        std::cout << "Rank 2: I am doing nothing." << std::endl;
    }

    // --- NEW: CLEAN UP THE BUFFER ---
    // Wait until the network actually sends the buffered message before detaching
    MPI_Buffer_detach(&buffer, &buffer_size);
    free(buffer);

    MPI_Finalize();
    return 0;
}
