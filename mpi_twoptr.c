/** FILE: mpi_twoptr.c
 *  Description: Here we are implementing the MPI version of the two pointer approach to the catching rain water problem.
 *
 *  Compile:
 *          mpicc -g -Wall -o mpi_tp mpi_naive.c
 *  Execution:
 *          mpiexec -n <number of cores> ./mpi_tp <array size>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

/*
MPI approach to the trapping rainwater problem.
Two Pointer Approach: O(n)

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int maxWater(int arr[], int n)
{

    // TODO:
    //   Implement sequential Two Pointer code,
    //   parallelize at home later
    int left = 1;
    int right = n - 2;
    int result = 0;
    int lMax = arr[left - 1];
    int rMax = arr[right + 1];

    while (left <= right)
    {
        if (rMax <= lMax)
        {
            result += (rMax - arr[right]) > 0 ? (rMax - arr[right]) : 0;
            rMax = rMax > arr[right] ? rMax : arr[right];
            right -= 1;
        }
        else
        {
            result += (lMax - arr[left]) > 0 ? (lMax - arr[left]) : 0;
            lMax = lMax > arr[left] ? lMax : arr[left];
            left += 1;
        }
    }

    return result;
}

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: mpiexec -n <number of cores> %s <array size> ----\n\n", prog_name);
}

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

    /**
     *
     * TODO:
     * Maxwater Method goes here
     * May not be able to parallelize this with MPI, ask Chat
     *
     *
     *
     */

    if (rank == 0)
    {
        printf("Maximum trapped rainwater: %d units\n", result);
        printf("Elapsed time: %lf seconds\n", elapsed_time);
    }

    free(arr);
    MPI_Finalize();
    return 0;
}