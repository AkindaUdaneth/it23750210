# Lab 06: Recursive OpenMP and MPI Communications

## Objectives
* Write Recursive OpenMP programs using task parallelization.
* Write distributed MPI programs handling point-to-point communication.
* Analyze execution time and speedup across multiple processors.
* Identify and resolve MPI deadlocks using buffered sends.

## Files and Exercises
Here is a breakdown of the source files included in this submission:

* **`ex2.c`:** Parallelized Fibonacci sequence calculator using OpenMP `#pragma omp task`.
* **`ex3.c`:** Distributed array sum (1 to 10,000,000) using `MPI_Reduce`.
* **`ex4.c`:** Parallel computation of Pi using the Monte Carlo method (10,000,000 iterations) with `MPI_Reduce`.
* **`plot_graphs.py`:** Python script utilizing `matplotlib` to generate performance graphs based on the execution times of Exercises 2, 3, and 4.
* **`time_vs_processors.png` & `speedup.png`:** The generated performance graphs (Exercise 5).
* **`ex6.cc`:** C++ MPI program demonstrating a deliberate source/destination mismatch, resulting in a system deadlock using standard `MPI_Send` and `MPI_Recv`.
* **`ex7.c`:** Refactored version of Exercise 3 using manual point-to-point communication and the `MPI_ANY_SOURCE` tag instead of collective reduction.
* **`ex8.cc`:** Refactored version of Exercise 6 that resolves the deadlock by utilizing a Buffered Send (`MPI_Bsend`) with manually allocated and attached memory.

## Compilation and Execution
**OpenMP (C):**
```bash
gcc -fopenmp filename.c -o output
export OMP_NUM_THREADS=4
./output
