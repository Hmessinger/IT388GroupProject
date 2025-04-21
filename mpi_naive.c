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

/**
 * Sequential approach to the trapping rainwater problem.
 * Naive approach: O(n^2) double loop
 *
 * Code credit: Geeks For Geeks
 *  https://geeksforgeeks.org/trapping-rain-water/#
 *
 * This sequential code has been modified into a parallel version.
 *
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
        if (argc == 1)
        {
            int buildings[] = {66, 104, 184, 340, 303, 159, 204, 132, 166, 307, 60, 212, 208, 152, 91, 93, 87, 198,
                               234, 161, 96, 87, 120, 150, 66, 60, 84, 117, 121, 55, 145, 102, 62, 150, 62, 119, 152,
                               118, 137, 106, 169, 99, 139, 142, 167, 96, 148, 155, 104, 153, 74, 207, 124, 192, 107,
                               155, 174, 114, 160, 78, 91, 125, 156, 182, 102, 134, 125, 15, 15, 86, 68, 102, 169, 122,
                               139, 278, 55, 128, 30, 99, 132, 194, 249, 257, 179, 151, 126, 130, 124, 164, 130, 166,
                               110, 121, 143, 230, 91, 122, 93, 103, 146, 62, 183, 101, 108, 157, 153, 167, 198, 84,
                               141, 155, 101, 138, 158, 135, 126, 126, 130, 192, 137, 106, 158, 97, 149, 120, 87, 92,
                               86, 82, 87, 118, 99, 95, 113, 157, 137, 140, 140, 140, 140, 151, 148, 111, 138, 118,
                               134, 111, 111, 12, 112, 93, 96, 79, 106, 181, 124, 85, 129, 114, 112, 55, 103, 128, 91,
                               182, 165, 92, 66, 135, 89, 143, 209, 116, 138, 76, 185, 174, 60, 221, 120, 123, 196,
                               87, 79, 177, 152, 142, 78, 50, 20, 140, 195, 6, 4, 86, 13, 5, 10, 12, 4, 5, 6, 6, 4, 3,
                               6, 14, 6, 4, 6, 6, 6, 12, 4, 4, 8, 9, 8, 6, 8, 8, 14, 42, 23, 63, 85, 123, 89, 63, 84,
                               66, 8, 15, 12, 16, 16, 14, 17, 80, 177, 143, 169, 88, 87, 72, 163, 197, 4, 13, 13, 1,
                               132, 260, 94, 143, 10, 45, 76, 125, 222, 174, 98, 150, 229, 98, 179, 443, 11, 81, 96,
                               137, 257, 207, 189, 4, 162, 10, 3, 8, 163, 5, 183, 112, 65, 80, 234, 60, 12, 20, 5, 8,
                               3, 13, 16, 13, 3, 8, 6, 5, 4, 14, 13, 3, 17, 17, 4, 4, 5, 4, 2, 15, 16, 15, 16, 16, 12,
                               12, 15, 35, 3, 13, 7, 12, 15, 7, 5, 5, 4, 4, 4, 30, 20, 20, 20, 50, 49, 50, 54, 255,
                               20, 12, 69, 107, 240, 83, 96, 14, 122, 65, 59, 50, 116, 153, 45, 18, 10, 0, 15, 5, 29,
                               20, 125, 40, 4, 2, 15};

            n = sizeof(buildings) / sizeof(buildings[0]);
            arr = (int *)malloc(n * sizeof(int));
            // Copying the buildings array into arr
            for (int i = 0; i < n; i++)
            {
                arr[i] = buildings[i];
            }
            // Determining how much work each process will do
            work = n / nproc;
        }
        else if (argc == 2)
        {
            n = atoi(argv[1]); // Get array size
            arr = (int *)malloc(n * sizeof(int));
            // Array "heights" will generate between 0 and 15
            for (int i = 0; i < n; i++)
            {
                arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
            }
            // Determining how much work each process will do
            work = n / nproc;
        }
        else
        {
            Usage(argv[0]);
            MPI_Abort(comm, 1);
        }
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
        printf("MPI_naive:\n");
        printf("N size: %d\n", n);
        printf("Number of processes: %d\n", nproc);
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