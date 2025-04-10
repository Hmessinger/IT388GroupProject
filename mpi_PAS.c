/** FILE: mpi_PAS.c
 *  Description: Here we are adding openMP to the sequential_PAS version of the trapping rainwater problem. This will parallelize the function that
 *               calculates the amount of rainwater that can be trapped. This approach will speedup the time complexity because there will be multiple
 *               threads working on the computations.
 *  Compile:
 *           mpicc -g -Wall -o ppas mpi_PAS.c

 *  Execution:
 *        mpiexec -n <num_threads> ./ppas <size>         

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
    local_size = end - start;

    int *left_max = NULL;
    int *right_max = NULL;
    int *local_water = malloc(local_size * sizeof(int));

    if (rank == 0) {
        left_max = malloc(heightSize * sizeof(int));
        right_max = malloc(heightSize * sizeof(int));

        left_max[0] = height[0];
        for (int i = 1; i < heightSize; i++) {
            left_max[i] = (height[i] > left_max[i - 1]) ? height[i] : left_max[i - 1];
        }

        right_max[heightSize - 1] = height[heightSize - 1];
        for (int i = heightSize - 2; i >= 0; i--) {
            right_max[i] = (height[i] > right_max[i + 1]) ? height[i] : right_max[i + 1];
        }
    }

    if (rank != 0) {
        left_max = malloc(heightSize * sizeof(int));
        right_max = malloc(heightSize * sizeof(int));
    }

    MPI_Bcast(left_max, heightSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(right_max, heightSize, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = start; i < end; i++) {
        int minOfTwo = (left_max[i] < right_max[i]) ? left_max[i] : right_max[i];
        if (minOfTwo > height[i]) {
            local_water[i - start] = minOfTwo - height[i];
        } else {
            local_water[i - start] = 0;
        }
    }

    int *total_water = NULL;
    int *recvcounts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        total_water = malloc(heightSize * sizeof(int));
        recvcounts = malloc(numProcs * sizeof(int));
        displs = malloc(numProcs * sizeof(int));

        for (int i = 0; i < numProcs; i++) {
            int local_start = i * (heightSize / numProcs);
            int local_end = (i == numProcs - 1) ? heightSize : (i + 1) * (heightSize / numProcs);
            recvcounts[i] = local_end - local_start;
            displs[i] = local_start;
        }
    }

    MPI_Gatherv(local_water, local_size, MPI_INT, total_water, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    int total = 0;
    if (rank == 0) {
        for (int i = 0; i < heightSize; i++) {
            total += total_water[i];
        }
        free(total_water);
        free(recvcounts);
        free(displs);
    }

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

    // Start time
    double start_time = MPI_Wtime();

    int total_water = trap(height, heightSize, rank, numProcs);

    // End time
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
