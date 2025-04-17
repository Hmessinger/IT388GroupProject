/** FILE: mpi_naive.c
 *  Description: Here we are adding MPI to the sequential (naive) version of the trapping rainwater problem. This will parallelize the function that
 *               calculates the amount of rainwater that can be trapped. This approach will speedup the time complexity because there will be multiple
 *               processes working on the distributed data.
 *  Compile:
 *          mpicc -g -Wall -o mpi_naive mpi_naive.c
 *  Execution:
 *          mpiexec -n <number of cores> ./mpi_naive <array size>
 *
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

void Usage(char *prog_name);

int main(int argc, char *argv[])
{
    int n = 0, rank, nproc, work;
    int start, end;
    int *arr;
    double start_time, elapsed_time;

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &nproc);

    if (rank == 0)
    {
        if (argc != 2)
        {
            Usage(argv[0]);
            MPI_Abort(comm, 1);
        }
        else
        {
            n = atoi(argv[1]);
            // Determining how much work each process will do
            work = n / nproc;
        }

        // generates the array
        arr = (int *)malloc(n * sizeof(int)); // Allocate memory to the array
        // Generate random array of n integers
        // Array "heights" will generate between 0 and 15
        for (int i = 0; i < n; i++)
        {
            arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
        }

        // Print the array if the size of it is less than 30
        if (n < 30)
        {
            printf("Array:\n");
            for (int i = 0; i < n; i++)
            {
                printf("[%d] ", arr[i]);
            }
            printf("\n---------------------------------------------\n");
        }
        else
            printf("Array too large to print\n");
    }

    // Broadcasting the total number of values and the work to all of the processes
    MPI_Bcast(&n, 1, MPI_INT, 0, comm);
    MPI_Bcast(&work, 1, MPI_INT, 0, comm);

    if (rank == (nproc - 1))
    {
        start = rank * work + 1;
        end = n - 1; // We end our iterations at n - 1 (this allows the last values that weren't divisible by nproc to be worked on)
    }
    if (rank != (nproc - 1))
    {
        start = rank * work + 1;
        end = start + work;
    }

    // Create arrays for all the other processes
    if (rank != 0)
    {
        arr = (int *)malloc(n * sizeof(int));
    }

    // Broadcast the array from process 0 to all other processes
    MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(comm); // sync all processes
    start_time = MPI_Wtime();

    // Each process
    int local_result = 0;
    // Each process is working on their portion of the rainwater computation
    for (int i = start; i < end; i++)
    {
        // Find max to the left
        int left = arr[i];
        for (int j = 0; j < i; j++)
        {
            if (arr[j] > left)
                left = arr[j];
        }
        // Find max to the right
        int right = arr[i];
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] > right)
                right = arr[j];
        }
        // Update max water result
        local_result += (left < right ? left : right) - arr[i];
    }

    // Reduce all local results to the final answer
    int global_result = 0;
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(comm); // sync again
    elapsed_time = MPI_Wtime() - start_time;

    if (rank == 0)
    {
        printf("Maximum trapped rainwater: %d units\n", global_result);
        printf("Elapsed time: %lf seconds\n", elapsed_time);
    }

    free(arr);
    MPI_Finalize();
    return 0;
}

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: mpiexec -n <number of cores> %s <array size> ----\n\n", prog_name);
}