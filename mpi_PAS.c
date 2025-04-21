/** FILE: mpi_PAS.c
 *  Description: Here we are adding openMP to the sequential_PAS version of the trapping rainwater problem. This will parallelize the function that
 *               calculates the amount of rainwater that can be trapped. This approach will speedup the time complexity because there will be multiple
 *               threads working on the computations.
 *  Compile:
 *           mpicc -g -Wall -o ppas mpi_PAS.c
 *
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

int trap(int *height, int heightSize, int rank, int numProcs)
{
    int local_size = heightSize / numProcs;
    int start = rank * local_size;
    int end = (rank == numProcs - 1) ? heightSize : (rank + 1) * local_size;
    local_size = end - start;

    int *left_max = NULL;
    int *right_max = NULL;
    int *local_water = malloc(local_size * sizeof(int));

    if (rank == 0)
    {
        left_max = malloc(heightSize * sizeof(int));
        right_max = malloc(heightSize * sizeof(int));

        left_max[0] = height[0];
        for (int i = 1; i < heightSize; i++)
        {
            left_max[i] = (height[i] > left_max[i - 1]) ? height[i] : left_max[i - 1];
        }

        right_max[heightSize - 1] = height[heightSize - 1];
        for (int i = heightSize - 2; i >= 0; i--)
        {
            right_max[i] = (height[i] > right_max[i + 1]) ? height[i] : right_max[i + 1];
        }
    }

    if (rank != 0)
    {
        left_max = malloc(heightSize * sizeof(int));
        right_max = malloc(heightSize * sizeof(int));
    }

    MPI_Bcast(left_max, heightSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(right_max, heightSize, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = start; i < end; i++)
    {
        int minOfTwo = (left_max[i] < right_max[i]) ? left_max[i] : right_max[i];
        if (minOfTwo > height[i])
        {
            local_water[i - start] = minOfTwo - height[i];
        }
        else
        {
            local_water[i - start] = 0;
        }
    }

    int *total_water = NULL;
    int *recvcounts = NULL;
    int *displs = NULL;

    if (rank == 0)
    {
        total_water = malloc(heightSize * sizeof(int));
        recvcounts = malloc(numProcs * sizeof(int));
        displs = malloc(numProcs * sizeof(int));

        for (int i = 0; i < numProcs; i++)
        {
            int local_start = i * (heightSize / numProcs);
            int local_end = (i == numProcs - 1) ? heightSize : (i + 1) * (heightSize / numProcs);
            recvcounts[i] = local_end - local_start;
            displs[i] = local_start;
        }
    }

    MPI_Gatherv(local_water, local_size, MPI_INT, total_water, recvcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    int total = 0;
    if (rank == 0)
    {
        for (int i = 0; i < heightSize; i++)
        {
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

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: mpiexec -n <number of cores> %s <array size> ----\n\n", prog_name);
}

int main(int argc, char **argv)
{
    int rank, numProcs, heightSize = 0;
    int *height = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

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

            heightSize = sizeof(buildings) / sizeof(buildings[0]);
            height = (int *)malloc(heightSize * sizeof(int));
            // Copying the buildings array into arr
            for (int i = 0; i < heightSize; i++)
            {
                height[i] = buildings[i];
            }
        }
        else if (argc == 2)
        {
            heightSize = atoi(argv[1]); // Get array size
            height = (int *)malloc(heightSize * sizeof(int));
            // Array "heights" will generate between 0 and 15
            for (int i = 0; i < heightSize; i++)
            {
                height[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
            }
        }
        else
        {
            Usage(argv[0]);
            MPI_Abort(comm, 1);
        }
    }

    // Broadcast the height array and heightSize to all processes
    MPI_Bcast(&heightSize, 1, MPI_INT, 0, comm);
    if (rank != 0)
        height = (int *)malloc(heightSize * sizeof(int));
    MPI_Bcast(height, heightSize, MPI_INT, 0, comm);

    MPI_Barrier(comm); // sync all processes
    // Start time
    double start_time = MPI_Wtime();

    int total_water = trap(height, heightSize, rank, numProcs);

    MPI_Barrier(comm); // sync all processes
    // End time
    double end_time = MPI_Wtime();

    if (rank == 0)
    {
        printf("N size: %d\n", heightSize);
        printf("Maximum trapped rainwater: %d units\n", total_water);
        printf("Elapsed time: %lf seconds\n", end_time - start_time);
    }

    free(height);
    MPI_Finalize();
    return 0;
}
