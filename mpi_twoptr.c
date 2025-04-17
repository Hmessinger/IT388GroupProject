/** FILE: mpi_twoptr.c
 *  Description: Here we are implementing the MPI version of the two pointer approach to the trapping rain water problem. The maxWater
 *               function uses two pointers to calculate the maximum trapped water. This two pointer approach in the function has to work sequentially.
 *               To parallelize this, we are splitting up the sequentially work amongst the processes. In this code, we are finding the largest value in the array
 *               and we are using one process to compute the total trapped rainwater in the first half of the array and another process to find the trapped rainwater
 *               in the second half.
 *
 *  Compile:
 *          mpicc -g -Wall -o mpi_tp mpi_twoptr.c
 *  Execution:
 *          mpiexec -n <number of core = 3> ./mpi_tp <array size>
 *          NOTE: You must use 3 processes
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

/**
 * MPI approach to the trapping rainwater problem.
 * Two Pointer Approach: O(n)
 *
 * Code credit: Geeks For Geeks
 *   https://geeksforgeeks.org/trapping-rain-water/#
 *
 * This sequential code has been modified into a parallel version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// This function uses two pointers to compute the total trapped rainwater in the array
int maxWater(int arr[], int n)
{

    if (n < 3)
        return 0; // No water can be trapped when there is less than 3 values

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

// This function finds the largest value in the array
int largestValue(int arr[], int size)
{
    if (size <= 0)
    {
        return -1;
    }
    int max = arr[0];
    int maxIdx = 0;
    for (int i = 1; i < size; i++)
    {
        if (max < arr[i])
        {
            max = arr[i];
            maxIdx = i;
        }
    }
    return maxIdx;
}

// This function creates a subarray of an initial array
void get_subarray(int arr[], int subarray[], int size, int start_index)
{
    for (int i = 0; i < size; i++)
    {
        subarray[i] = arr[start_index + i];
    }
}

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: mpiexec -n <number of cores = 3> %s <array size> ----\n\n", prog_name);
}

int main(int argc, char *argv[])
{
    int n = 0, rank, nproc;
    int *arr = NULL, *leftarr, *rightarr;
    int max_val_index, leftSize, rightSize;
    int local_result = 0, global_result = 0;
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
            // Array size
            n = atoi(argv[1]);
        }

        // generates the array
        arr = (int *)malloc(n * sizeof(int)); // Allocate memory to the array
        // Generate random array of n integers
        // Array "heights" will generate between 0 and 15
        for (int i = 0; i < n; i++)
        {
            arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
        }

        // Compute max value in the array
        max_val_index = largestValue(arr, n);
        leftSize = max_val_index + 1;
        rightSize = (n - max_val_index);
        // Generating the two subarrays
        leftarr = (int *)malloc(leftSize * sizeof(int));
        rightarr = (int *)malloc(rightSize * sizeof(int));
        get_subarray(arr, leftarr, leftSize, 0);
        get_subarray(arr, rightarr, rightSize, max_val_index);

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
     * Logic for the processes:
     * 1. Find the largest value in the array
     * 2. Split the array and have one process work on the left side and one work on the right side
     *  (The middle, where the array is being split, is the largest values index)
     * 3. The processes will use the maxWater function to determine their local total trapped water
     * 4. The processes will reduce their local values to a final total.
     *
     */

    // Send the subarrays and their sizes to the working processes
    if (rank == 0)
    {
        // Sending the left size and subarray to rank 1
        MPI_Send(&leftSize, 1, MPI_INT, 1, 0, comm);
        MPI_Send(leftarr, leftSize, MPI_INT, 1, 1, comm);

        // Sending the right size and subarray to rank 2
        MPI_Send(&rightSize, 1, MPI_INT, 2, 2, comm);
        MPI_Send(rightarr, rightSize, MPI_INT, 2, 3, comm);
    }

    // Process 1 is receiving the left side subarray
    if (rank == 1)
    {
        MPI_Recv(&leftSize, 1, MPI_INT, 0, 0, comm, MPI_STATUS_IGNORE);
        leftarr = (int *)malloc(leftSize * sizeof(int));
        MPI_Recv(leftarr, leftSize, MPI_INT, 0, 1, comm, MPI_STATUS_IGNORE);
    }

    // Process 2 is receiving the right side subarray
    if (rank == 2)
    {
        MPI_Recv(&rightSize, 1, MPI_INT, 0, 2, comm, MPI_STATUS_IGNORE);
        rightarr = (int *)malloc(rightSize * sizeof(int));
        MPI_Recv(rightarr, rightSize, MPI_INT, 0, 3, comm, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(comm); // sync all processes
    start_time = MPI_Wtime();

    // The processes calculate their local sums
    if (rank == 1)
    {
        local_result = maxWater(leftarr, leftSize);
    }
    if (rank == 2)
    {
        local_result = maxWater(rightarr, rightSize);
    }

    // Reducing to get the total sum
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(comm); // sync again
    elapsed_time = MPI_Wtime() - start_time;

    if (rank == 0)
    {
        printf("Maximum trapped rainwater: %d units\n", global_result);
        printf("Elapsed time: %lf seconds\n", elapsed_time);
    }

    if (arr != NULL)
        free(arr);
    if ((rank == 0 || rank == 1) && leftarr != NULL)
        free(leftarr);
    if ((rank == 0 || rank == 2) && rightarr != NULL)
        free(rightarr);
    MPI_Finalize();
    return 0;
}