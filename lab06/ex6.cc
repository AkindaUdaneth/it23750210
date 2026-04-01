#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int data = 42; // The message we want to send

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 3) {
        std::cout << "Please run this with at least 3 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0) {
        // Rank 0 is sending data specifically to Rank 1
        std::cout << "Rank 0: Sending data to Rank 1..." << std::endl;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Rank 0: Finished sending!" << std::endl;
        
    } else if (rank == 1) {
        // THE MISMATCH: Rank 1 is expecting data from Rank 2, ignoring Rank 0!
        std::cout << "Rank 1: Waiting to receive data from Rank 2..." << std::endl;
        MPI_Recv(&data, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Rank 1: Received data!" << std::endl;
        
    } else if (rank == 2) {
        // Rank 2 just sits here and does nothing, never sending a message
        std::cout << "Rank 2: I am doing nothing." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
