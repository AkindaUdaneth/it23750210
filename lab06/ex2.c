#include <stdio.h>
#include <omp.h>

// Parallelized Fibonacci using OpenMP Tasks
int fib(int n) {
    int i, j;
    if (n < 2)
        return n;
    else {
        // Create a task for the first recursive call
        #pragma omp task shared(i)
        i = fib(n - 1);

        // Create a task for the second recursive call
        #pragma omp task shared(j)
        j = fib(n - 2);

        // The parent thread MUST wait here until both child tasks complete
        #pragma omp taskwait
        return i + j;
    }
}

int main() {
    int n = 30; // 30 is large enough to measure time, but won't crash the server
    int result;
    double start_time, end_time;

    printf("Calculating Fibonacci(%d) using OpenMP Tasks...\n", n);

    // Start the timer
    start_time = omp_get_wtime();

    // Create the pool of threads
    #pragma omp parallel
    {
        // Use 'single' so ONLY ONE thread triggers the very first function call.
        // It will then spawn the initial tasks for the other idle threads to pick up.
        #pragma omp single
        {
            result = fib(n);
        }
    }

    // Stop the timer
    end_time = omp_get_wtime();

    printf("Result: %d\n", result);
    printf("Time taken: %f seconds\n", end_time - start_time);

    return 0;
}
