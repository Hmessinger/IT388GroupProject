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
        }
        else
        {
            Usage(argv[0]);
            MPI_Abort(comm, 1);
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
        printf("MPI_twoptr:\n");
        printf("N size: %d\n", n);
        printf("Number of processes: %d\n", nproc);
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