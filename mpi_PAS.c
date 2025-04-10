/** FILE: mpi_PAS.c
 *  Description: Here we are adding openMP to the sequential_PAS version of the trapping rainwater problem. This will parallelize the function that
 *               calculates the amount of rainwater that can be trapped. This approach will speedup the time complexity because there will be multiple
 *               threads working on the computations.
 *  Compile:
 *          gcc -o ppas mpi_PAS.c
 *  Execution:
 *          ./ppas <array size>
 * 
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int trap(int* height, int heightSize, int rank, int numProcs) {
    int local_size = heightSize / numProcs;
    int start = rank * local_size;
    int end = (rank == numProcs - 1) ? heightSize : (rank + 1) * local_size;

    int *left_max = malloc(local_size * sizeof(int));
    int *right_max = malloc(local_size * sizeof(int));
    int *local_water = malloc(local_size * sizeof(int));

    // Fill left_max array for local portion
    left_max[0] = height[start];
    for (int i = start + 1; i < end; i++) {
        left_max[i - start] = (height[i] > left_max[i - start - 1]) ? height[i] : left_max[i - start - 1];
    }

    // Fill right_max array for local portion
    right_max[end - start - 1] = height[end - 1];
    for (int i = end - 2; i >= start; i--) {
        right_max[i - start] = (height[i] > right_max[i - start + 1]) ? height[i] : right_max[i - start + 1];
    }

    // Calculate local trapped water using prefix and suffix max values
    for (int i = start; i < end; i++) {
        int minOfTwo = (left_max[i - start] < right_max[i - start]) ? left_max[i - start] : right_max[i - start];
        if (minOfTwo > height[i]) {
            local_water[i - start] = minOfTwo - height[i];
        } else {
            local_water[i - start] = 0;
        }
    }

    // Communicate the boundary values (leftmost from the right segment, rightmost from the left segment)
    int left_boundary, right_boundary;
    if (rank == 0) {
        left_boundary = left_max[local_size - 1];  // Rightmost element of rank 0's segment
    }
    if (rank == numProcs - 1) {
        right_boundary = right_max[local_size - 1];  // Leftmost element of last process's segment
    }

    MPI_Bcast(&left_boundary, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&right_boundary, 1, MPI_INT, numProcs - 1, MPI_COMM_WORLD);

    // Adjust the boundary elements' trapped water
    if (rank == 0) {
        local_water[0] += left_boundary - height[start];
    }
    if (rank == numProcs - 1) {
        local_water[local_size - 1] += right_boundary - height[end - 1];
    }

    // Gather results from all processes
    int *total_water = NULL;
    if (rank == 0) {
        total_water = malloc(heightSize * sizeof(int));
    }

    MPI_Gather(local_water, local_size, MPI_INT, total_water, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate total trapped water at root process
    int total = 0;
    if (rank == 0) {
        for (int i = 0; i < heightSize; i++) {
            total += total_water[i];
        }
        free(total_water);
    }

    // Free local arrays
    free(left_max);
    free(right_max);
    free(local_water);

    return total;
}
void printArray(int* arr, int size, const char* label) {
    printf("%s: [", label);
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

int main(int argc, char** argv) {
    int rank, numProcs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    // Get array size from command-line argument
    if (argc < 2) {
        if (rank == 0) {
            printf("Usage: ./ppas <array size>\n");
        }
        MPI_Finalize();
        return 1;
    }

    int heightSize = atoi(argv[1]);
    int *height = malloc(heightSize * sizeof(int));

    // Initialize the height array 
    if (rank == 0) {
        for (int i = 0; i < heightSize; i++) {
            height[i] = rand() % 15;  // Random height between 0 and 14
        }
    }

    // Broadcast the height array to all processes
    MPI_Bcast(height, heightSize, MPI_INT, 0, MPI_COMM_WORLD);

    //start time
    double start_time = MPI_Wtime();

    int total_water = trap(height, heightSize, rank, numProcs);
    //end time
    double end_time = MPI_Wtime();

    if (rank == 0) {

         printArray(height, heightSize, "Array: ");

        printf("Total trapped water: %d\n", total_water);
        printf("Total execution time: %f seconds\n", end_time - start_time);
    }

    free(height);
    MPI_Finalize();
    return 0;
}
